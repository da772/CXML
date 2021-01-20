#pragma once
#include <memory>
#include <string>

namespace dynamic_pointers {

    static int AllocCounter = 0;

        class DefaultClass {
        public:
            inline DefaultClass() : str("DefaultClass") {dynamic_pointers::AllocCounter++;};
            inline ~DefaultClass() { dynamic_pointers::AllocCounter--; };
            std::string str;
    };

    class ExampleClass_1 {
        public:
            inline ExampleClass_1(int x, float y, long z, const std::string&  str ) : x(x), y(y), z(z), str(str) {dynamic_pointers::AllocCounter++;};
            inline ~ExampleClass_1() { dynamic_pointers::AllocCounter--; };
            int x =0;
            float y = 0.f;
            long z = 0;
            std::string str;
    };

    class ExampleClass_2 {
        public:
            inline ExampleClass_2(std::shared_ptr<ExampleClass_1> exClass) : exClass(exClass) {dynamic_pointers::AllocCounter++;};
            inline ~ExampleClass_2() { dynamic_pointers::AllocCounter--; };
            std::shared_ptr<ExampleClass_1> exClass;
            inline void SetVars(int x, float y, long z, const std::string& str) {exClass->x = x; exClass->y = y; exClass->z = z; exClass->str = str;}
    };

    template<typename T>
    class DynamicPointer_Wrapper {
        public:
            inline DynamicPointer_Wrapper() {dynamic_pointers::AllocCounter++;};
            
            inline std::shared_ptr<T> GetPtr() {
                std::shared_ptr<T> _ptr = ptr;
                delete this;
                return _ptr;
            }
            std::shared_ptr<T> ptr;
        private:
            inline ~DynamicPointer_Wrapper() {
                dynamic_pointers::AllocCounter--;
            };
    };

    int RunTests();

}