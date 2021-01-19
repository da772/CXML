#pragma once
#include <string>

namespace default_pointers {

    static int cleanCounter = 0;

    class ExampleClass_1 {
        public:
            inline ExampleClass_1(int x, float y, long z, const std::string&  str ) : x(x), y(y), z(z), str(str) {};
            inline ~ExampleClass_1() { default_pointers::cleanCounter++; };
            int x =0;
            float y = 0.f;
            long z = 0;
            std::string str;
    };

    class ExampleClass_2 {
        public:
            inline ExampleClass_2(ExampleClass_1* exClass) : exClass(exClass) {};
            inline ~ExampleClass_2() { default_pointers::cleanCounter++; };
            ExampleClass_1* exClass;
    };

    int RunTests();

}