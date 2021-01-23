#include "config.h"
#include "default_types.h"
#include "CXML/CXML.hpp"

#include <iostream>

#define TEST_ASSERT(x) if (x) ; else { std::cout <<  ("default_pointers Test...................... Failed - "+std::string(__FILE__)+"(Line "+std::to_string(__LINE__) +")")<< std::endl; return __LINE__;}
#if DEFAULT_TYPE_TEST_DEBUG
#define DEBUG_OUTPUT
#endif


namespace default_types {

    static ::CXML CXML;


    static std::string cxml = R"(
        <int> 1 </int>
        <int> -32767 </int>
        <float> 2.3322 </float>
        <int> 32767 </int>
        <long> 2147483647 </long>
        <float> 5.77423 </float>
        <hex> EE </hex>
        <long> -2147483647 </long>
        <hex> FF </hex>
        <long> 2147483 </long>

    )";

    int cxmlInt[] = { 1, -32767,32767 };
    float cxmlFloat[] = { 2.3322 , 5.77423  };
    long cxmlLong[] = {2147483647, 0xEE, -2147483647, 0xFF, 2147483};

    int RunTests() {

        CXML::CXML_Node n = CXML.GetNext(cxml.c_str());

        int intCounter = 0, floatCounter =0, longCounter =0;
        while (CXML::ValidNode(n)) {
            if (n.type == "int") {
                #ifdef DEBUG_OUTPUT
                    std::cout << "Node (int): " << CXML.GetNodeAsInt(n) << " - Expected: " << cxmlInt[intCounter] << std::endl;
                #endif
                TEST_ASSERT(CXML.GetNodeAsInt(n) == cxmlInt[intCounter++]);
            }
            if (n.type == "long" || n.type == "hex") {
                #ifdef DEBUG_OUTPUT
                    std::cout << "Node (long): " << (n.type == "long" ? CXML.GetNodeAsLong(n) : CXML.GetNodeAsHex(n)) << " - Expected: " << cxmlLong[longCounter] << std::endl;
                #endif
                TEST_ASSERT( (n.type == "long" ? CXML.GetNodeAsLong(n) : CXML.GetNodeAsHex(n)) == cxmlLong[longCounter++]);
            }
            if (n.type == "float") {
                 #ifdef DEBUG_OUTPUT
                    std::cout << "Node (float): " << CXML.GetNodeAsFloat(n) << " - Expected: " << cxmlFloat[floatCounter] << std::endl;
                #endif
                TEST_ASSERT(CXML.GetNodeAsFloat(n) == cxmlFloat[floatCounter++]);
            }

            n = CXML.GetNext(cxml.c_str(), n);
        }


        std::cout << ("default_types Test.......................... Passed - "+std::string(__FILE__))<< std::endl;
        return 0;
    }
    



}