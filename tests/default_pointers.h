#pragma once
#include <string>
#include <iostream>

namespace default_pointers {

  
    class DefaultClass {
        public:
            inline DefaultClass() : str("DefaultClass") {
                #if DEFAULT_POINTER_TEST_DEBUG
                    std::cout << "Default Class Created!" << std::endl;
                #endif

            };
            inline ~DefaultClass() {};
            std::string str;
    };

    class ExampleClass_1 {
        public:
            inline ExampleClass_1(int x, float y, long z, const std::string&  str ) : x(x), y(y), z(z), str(str) {};
            inline ~ExampleClass_1() {  };
            int x =0;
            float y = 0.f;
            long z = 0;
            std::string str;
    };

    class ExampleClass_2 {
        public:
            inline ExampleClass_2(ExampleClass_1* exClass) : exClass(exClass) {};
            inline ~ExampleClass_2() {  };
            ExampleClass_1* exClass;
            inline void SetVars(int x, float y, long z, const std::string& str) {exClass->x = x; exClass->y = y; exClass->z = z; exClass->str = str;}
    };

    int RunTests();

}