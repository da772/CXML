#pragma once
#include <memory>
#include <string>

namespace dynamic_pointers {

    static int cleanCounter = 0;

    class ExampleClass_1 {
        public:
            inline ExampleClass_1(int x, float y, long z, const std::string&  str ) : x(x), y(y), z(z), str(str) {};
            inline ~ExampleClass_1() { dynamic_pointers::cleanCounter++; };
            int x =0;
            float y = 0.f;
            long z = 0;
            std::string str;
    };

    class ExampleClass_2 {
        public:
            inline ExampleClass_2(std::shared_ptr<ExampleClass_1> exClass) : exClass(exClass) {};
            inline ~ExampleClass_2() { dynamic_pointers::cleanCounter++; };
            std::shared_ptr<ExampleClass_1> exClass;
    };

    template<typename T>
    class DynamicPointer_Wrapper {
        public:
            inline DynamicPointer_Wrapper() {};
            
            inline std::shared_ptr<T> GetPtr() {
                std::shared_ptr<T> _ptr = ptr;
                delete this;
                return _ptr;
            }
            std::shared_ptr<T> ptr;
        private:
            inline ~DynamicPointer_Wrapper() {
                dynamic_pointers::cleanCounter++;
            };
    };

    int RunTests();

}