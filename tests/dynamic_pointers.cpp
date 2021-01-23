#include "config.h"
#include "dynamic_pointers.h"
#include "CXML/CXML.hpp"
#include <iostream>

namespace dynamic_pointers {



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
       <ExampleClass_2 name="ExampleClass2">
        <constructor>
            <ExampleClass_1 name="ExampleClass1">
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
    <DefaultClass name=DefaultClass>
    </DefaultClass>
    )";


    #ifdef DEBUG_OUTPUT
    std::cout << "Dynamic Pointer Test: \n" << cxml << std::endl;
    #endif

    int s = Setup();
    if (s) return 1;
    int p = Process(cxml);
    if (p) return 1;

    // Clean up registry
	CXML.ClearRegistry();
    std::cout << ("dynamic_pointers Test....................... Passed - "+std::string(__FILE__))<< std::endl;

    return 0;

}

static int Process(const std::string& cxml) {
    CXML::CXML_Node n = CXML.GetNext(cxml.c_str()); // Get first node from source
    std::shared_ptr<DefaultClass> defaultClass = nullptr;
    std::shared_ptr<ExampleClass_1> exampleClass1 = nullptr;
    std::shared_ptr<ExampleClass_2> exampleClass2 = nullptr;
    while (CXML::ValidNode(n)) {
        if (n.type == "DefaultClass") {
            #ifdef DEBUG_OUTPUT
            std::cout << "Class: " << n.type << std::endl;
            #endif
            defaultClass = CXML.GetNodeAsClass<DynamicPointer_Wrapper<DefaultClass>>(n)->GetPtr();
            TEST_ASSERT(n.tags["name"] == "DefaultClass");
            TEST_ASSERT(defaultClass->str == "DefaultClass");
        }
        if (n.type == "ExampleClass_2") {
            #ifdef DEBUG_OUTPUT
            std::cout << "Class: " << n.type << std::endl;
            #endif
            TEST_ASSERT(n.tags["name"] == "ExampleClass2");
            exampleClass2 = CXML.GetNodeAsClass<DynamicPointer_Wrapper<ExampleClass_2>>(n)->GetPtr();
            exampleClass1 = exampleClass2->exClass;
            // Manually clean up
            
        }
        n = CXML.GetNext(cxml.c_str(), n.endPos);
    }
    
    TEST_ASSERT(defaultClass != nullptr && exampleClass1 != nullptr && exampleClass2 != nullptr)

    defaultClass = nullptr;
    exampleClass2 = nullptr;
    exampleClass1 = nullptr;
    #ifdef DEBUG_OUTPUT
    std::cout << "ALLOC COUNTER: " << AllocCounter << std::endl;;
    #endif
    TEST_ASSERT(AllocCounter == 0);

    return 0;
}


static int Setup() {
    CXML::CXML_Data<DefaultClass>* defClass = CXML.RegisterClass<DefaultClass>("DefaultClass");
    CXML::CXML_Data<ExampleClass_2, std::shared_ptr<ExampleClass_1>>* cls2 = CXML.RegisterClass<ExampleClass_2, std::shared_ptr<ExampleClass_1>>("ExampleClass_2");

    CXML::CXML_Data<ExampleClass_1, int,float,long, const std::string&>* cls1 = CXML.RegisterClass<ExampleClass_1, int, float, long, const std::string&>("ExampleClass_1");
	

    defClass->SetAllocateFunc([](){
        DynamicPointer_Wrapper<DefaultClass>* wrapper = new DynamicPointer_Wrapper<DefaultClass>();
        wrapper->ptr = std::make_shared<DefaultClass>();
		return (void*)wrapper;
    });

	cls1->SetAllocateFunc([](int x, float y, long z, const std::string& str) {
		DynamicPointer_Wrapper<ExampleClass_1>* wrapper = new DynamicPointer_Wrapper<ExampleClass_1>();
		wrapper->ptr = std::make_shared<ExampleClass_1>(x,y,z, str);
		return (void*)wrapper;
	});

    cls2->SetAllocateFunc([](std::shared_ptr<ExampleClass_1> clss) {
		DynamicPointer_Wrapper<ExampleClass_2>* wrapper = new DynamicPointer_Wrapper<ExampleClass_2>();
		wrapper->ptr = std::make_shared<ExampleClass_2>(clss);
		return (void*)wrapper;
	});
    
	cls1->SetProcessFunc([cls1](const char* c) {
		size_t endPos;
        CXML::CXML_Node n = CXML.GetNext(c);
        if (n.type == "constructor") // Check if source has constructor and set source to its info (important if using functions)
            c = n.info.c_str();
        int i = CXML.GetNextAsInt(c, 0, &endPos);
        float f = CXML.GetNextAsFloat(c, endPos, &endPos);
        long l = CXML.GetNextAsHex(c, endPos, &endPos);
        std::string s = CXML.GetNextAsString(c, endPos, &endPos);
		cls1->Allocate(i,f,l, s);
	});

	cls2->SetProcessFunc([cls2](const char* c) {
        CXML::CXML_Node n = CXML.GetNext(c); // Get line 2
        DynamicPointer_Wrapper<ExampleClass_1>* ex1; // Reference to constructor arg
        DynamicPointer_Wrapper<ExampleClass_2>* ex2; // Reference to created object
        while (CXML::ValidNode(n)) { // Check if current node is valid
            #ifdef DEBUG_OUTPUT
            std::cout << "Node Loop: " << n.type << std::endl;
            #endif
            if (n.type == "constructor") { // Check if node is constructor for ExampleClass_2 (line 2)
                
                ex1 = CXML.GetNextAsClass<DynamicPointer_Wrapper<ExampleClass_1>>(n.info.c_str()); // Create ExampleClass_1 from next node at line 3
                n = CXML.GetNext(c, n.endPos); // Set next node to be out of the constructor (line 12)
                ex2 = (DynamicPointer_Wrapper<ExampleClass_2>*)cls2->Allocate(ex1->GetPtr());
                ex1 = nullptr;
                TEST_ASSERT(ex2->ptr->exClass->x == 33 && ex2->ptr->exClass->y == 32.12f && ex2->ptr->exClass->z == 0xFF && ex2->ptr->exClass->str == "FOO");
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
                    ex2->ptr->SetVars(i,f,hex,str);
                    TEST_ASSERT(ex2->ptr->exClass->x == 22 && ex2->ptr->exClass->y == 55.2123f && ex2->ptr->exClass->z == 0xEEFF && ex2->ptr->exClass->str == "BAR");
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
