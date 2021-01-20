#include "config.h"
#include "default_pointers.h"
#include "CXML.hpp"

#define TEST_ASSERT(x) if (x) ; else { std::cout <<  ("default_pointers Test...................... Failed - "+std::string(__FILE__)+"(Line "+std::to_string(__LINE__) +")")<< std::endl; return __LINE__;}
#if DEFAULT_POINTER_TEST_DEBUG
#define DEBUG_OUTPUT
#endif

namespace default_pointers {


// Initalize CXML
static ::CXML CXML;

static int Setup();
static int Process(const std::string& cxml);

int RunTests() {

    /* CXML Example:

1      <ExampleClass_2> 
2         <constructor>
3             <ExampleClass_1>
4                 <constructor>
5                     <int> 33 </int>
6                     <float> 32.12 </float>
7                     <hex> FF </hex>
8                     <string>FOO</string>
9                 </constructor>
10            </ExampleClass_1>
11        </constructor>
12        <function>
13            <SetVars>
14                <int> 22 </int>
15                <float> 55.2123 </float>
16                <hex> EEFF </hex>
17                <string> BAR </string>
18            </SetVars>
19        </function>
20    </ExampleClass_2>
21    <DefaultClass>
22    </DefaultClass>
    */

    std::string cxml = R"(
       <ExampleClass_2>
        <constructor>
            <ExampleClass_1>
                <constructor>
                    <int> 33 </int>
                    <float> 32.12 </float>
                    <hex> FF </hex>
                    <string>FOO</string>
                </constructor>
            </ExampleClass_1>
        </constructor>
        <function>
            <SetVars>
                <int> 22 </int>
                <float> 55.2123 </float>
                <hex> EEFF </hex>
                <string>BAR</string>
            </SetVars>
        </function>
    </ExampleClass_2>
    <DefaultClass>
    </DefaultClass>
    )";
    #ifdef DEBUG_OUTPUT
    std::cout << "\nDefault Pointer Test: \n" << cxml << std::endl;
    #endif
	
    int s = Setup();
    if (s) return s;
    int p = Process(cxml);
    if (p) return p;

    // Clean registry
	CXML.ClearRegistry();

    std::cout << ("default_pointers Test....................... Passed - "+std::string(__FILE__))<< std::endl;
    return 0;

}


static int Process(const std::string& cxml) {
    /*
    * CXML Processing
    */
   
    CXML::CXML_Node n = CXML.GetNext(cxml.c_str()); // Get first node from source
    DefaultClass* defaultClass = nullptr;
    ExampleClass_1* exampleClass1 = nullptr;
    ExampleClass_2* exampleClass2 = nullptr;
    while (CXML.ValidNode(n)) {
        if (n.type == "DefaultClass") {
            #ifdef DEBUG_OUTPUT
            std::cout << "Class: " << n.type << std::endl;
            #endif
            defaultClass = (DefaultClass*)CXML.GetNodeAsClass<void>(n);
            TEST_ASSERT(defaultClass->str == "DefaultClass");
        }
        if (n.type == "ExampleClass_2") {
            #ifdef DEBUG_OUTPUT
            std::cout << "Class: " << n.type << std::endl;
            #endif
            exampleClass2 = CXML.GetNodeAsClass<ExampleClass_2>(n);
            exampleClass1 = exampleClass2->exClass;
            // Manually clean up
            
        }
        n = CXML.GetNext(cxml.c_str(), n.endPos);
    }
    TEST_ASSERT(defaultClass != nullptr && exampleClass1 != nullptr && exampleClass2 != nullptr)
    // Clean manually
    if (defaultClass)
        delete defaultClass;
    if (exampleClass1) {
        delete exampleClass2->exClass;
        delete exampleClass2;
    }
    return 0;
}

static int Setup() {
    /*
     * Class Registration
    */ 

    // Register Default Class
    CXML::CXML_Data<DefaultClass>* defClass = CXML.RegisterClass<DefaultClass>("DefaultClass");
    // Register ExampleClass_1
    CXML::CXML_Data<ExampleClass_1, int,float,long, const std::string&>* cls1 = CXML.RegisterClass<ExampleClass_1, int, float, long, const std::string&>("ExampleClass_1");
    // Register ExampleClass_2
    CXML::CXML_Data<ExampleClass_2, ExampleClass_1*>* cls2 = CXML.RegisterClass<ExampleClass_2, ExampleClass_1*>("ExampleClass_2");
    

    // Processing for ExampleClass_1
	cls1->SetProcessFunc([cls1](const char* c) {
        // c contains lines 4-9
        const char* _c = c;
        CXML::CXML_Node n = CXML.GetNext(c); // Get next node to determine if using constructor tag or not
        if (n.type == "constructor") // Check if source has constructor and set source to its info (important if using functions)
            _c = n.info.c_str();
        size_t endPos = 0; // store endPos of previous node
        // Get constructor arguments
        int i = CXML.GetNextAsInt(_c, endPos, &endPos); 
        float f = CXML.GetNextAsFloat(_c, endPos, &endPos);
        long l = CXML.GetNextAsHex(_c, endPos, &endPos);
        std::string s = CXML.GetNextAsString(_c, endPos, &endPos);
        // Create ExampleClass_1
		ExampleClass_1* exampleClass1 = (ExampleClass_1*)cls1->Allocate(i, f, l, s);
	});

    // Processing for ExampleClass_2
	cls2->SetProcessFunc([cls2](const char* c) {
        // c contains lines 2-19
        CXML::CXML_Node n = CXML.GetNext(c); // Get line 2
        ExampleClass_1* ex1; // Reference to constructor arg
        ExampleClass_2* ex2; // Reference to created object
        while (CXML.ValidNode(n)) { // Check if current node is valid
            #ifdef DEBUG_OUTPUT
            std::cout << "Node Loop: " << n.type << std::endl;
            #endif
            if (n.type == "constructor") { // Check if node is constructor for ExampleClass_2 (line 2)
                ex1 = CXML.GetNextAsClass<ExampleClass_1>(n.info.c_str()); // Create ExampleClass_1 from next node at line 3
                n = CXML.GetNext(c, n.endPos); // Set next node to be out of the constructor (line 12)
                ex2 = (ExampleClass_2*)cls2->Allocate(ex1); // Allocate ExampleClass_2 using args from constructor
                TEST_ASSERT(ex2->exClass->x == 33 && ex2->exClass->y == 32.12f && ex2->exClass->z == 0xFF && ex2->exClass->str == "FOO");
                continue;
            }
            if (n.type == "function") { // Check if node is function for ExampleClass_2 (line 12)
                CXML::CXML_Node _n = CXML.GetNext(n.info.c_str()); // Get function node (line 13)
                if (_n.type == "SetVars") { // Check function type
                    size_t endPos = 0; // store endPos of previous node
                    // Get args for function
                    int i = CXML.GetNextAsInt(_n.info.c_str(), endPos, &endPos); 
                    float f = CXML.GetNextAsFloat(_n.info.c_str(), endPos, &endPos);
                    long hex = CXML.GetNextAsHex(_n.info.c_str(), endPos, &endPos);
                    std::string str = CXML.GetNextAsString(_n.info.c_str(), endPos, &endPos);
                    // Call function
                    ex2->SetVars(i,f,hex,str);
                    TEST_ASSERT(ex2->exClass->x == 22 && ex2->exClass->y == 55.2123f && ex2->exClass->z == 0xEEFF && ex2->exClass->str == "BAR");
                }
                n = CXML.GetNext(c, n.endPos); // Set node to outside of current function (line 19) should be invalid
                continue;
            }
            n = CXML.GetNext(n.info.c_str(), n.endPos); // Get next node
        }
	});

    return 0;
}

}

