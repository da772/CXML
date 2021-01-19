#include "config.h"
#include "default_pointers.h"
#include "CXML.hpp"
#include <iostream>

#define TEST_ASSERT(x) if (x) ; else { std::cout <<  ("default_pointers Test...................... Failed - "+std::string(__FILE__)+"(Line "+std::to_string(__LINE__) +")")<< std::endl; return __LINE__;}

#ifdef DEFAULT_POINTER_TEST_DEBUG
#define DEBUG_OUTPUT
#endif

namespace default_pointers {

int RunTests() {

    /* CXML Example:

    <ExampleClass_2>
        <ExampleClass_1>
            <int> 33 </int>
            <float> 32.12 </int>
            <hex> FF </hex>
            <string>FOO</string>
        </ExampleClass_1>
    </ExampleClass_2>

    */

    
    std::string cxml = "<ExampleClass_2>\n\t<ExampleClass_1>\n\t\t<int> 33 </int>\n\t\t<float> 32.12 </float>\n\t\t<hex> FF </hex>\n\t\t<string>FOO</string>\n\t</ExampleClass_1>\n</ExampleClass_2>";
    #ifdef DEBUG_OUTPUT
    std::cout << "Default Pointer Test: \n" << cxml << std::endl;
    #endif
	
    CXML::CXML_Class<ExampleClass_2, ExampleClass_1*>* cls2 = CXML::RegisterClass<ExampleClass_2, ExampleClass_1*>("ExampleClass_2");

    CXML::CXML_Class<ExampleClass_1, int,float,long, const std::string&>* cls1 = CXML::RegisterClass<ExampleClass_1, int, float, long, const std::string&>("ExampleClass_1");
	
	cls1->SetAllocateFunc([](int x, float y, long z, const std::string& str) {
		return (void*)new ExampleClass_1(x,y,z,str);
	});

	cls1->SetProcessFunc([cls1](const char* c) {
		size_t endPos;
        int i = CXML::GetNextAsInt(c, 0, &endPos);
        float f = CXML::GetNextAsFloat(c, endPos, &endPos);
        long l = CXML::GetNextAsHex(c, endPos, &endPos);
        std::string s = CXML::GetNextAsString(c, endPos, &endPos);
		cls1->Allocate(i, f, l, s);
	});



	cls2->SetProcessFunc([cls2](const char* c) {
		ExampleClass_1* ex1 = CXML::GetNextAsClass<ExampleClass_1>(c);
		cls2->Allocate(ex1);
	});

	cls2->SetAllocateFunc([](ExampleClass_1* c){
		return (void*)new ExampleClass_2(c);
	});

	

	CXML::CXML_Node n = CXML::GetNext(cxml.c_str());
    TEST_ASSERT(n.type == "ExampleClass_2");
	if (n.type == "ExampleClass_2") {
        #ifdef DEBUG_OUTPUT
        std::cout << "Class: " << n.type << std::endl;
        #endif
		ExampleClass_2* clss = CXML::GetNodeAsClass<ExampleClass_2>(n);
        #ifdef DEBUG_OUTPUT
        std::cout << n.type << "->ExampleClass_1 = " << "( " << clss->exClass->x << ", " << clss->exClass->y << ", " << clss->exClass->z << ", " << clss->exClass->str << " )" << std::endl; 
        #endif
        // Clean
        delete clss->exClass;
        delete clss;
	}


	CXML::ClearRegistry();
    std::cout << ("default_pointers Test....................... Passed - "+std::string(__FILE__))<< std::endl;


    return 0;

}
}