#include "CXML.hpp"
#include "dynamic_pointers.h"
#include "default_pointers.h"
#include <iostream>
#include <unordered_map>

int main() {

	/*
	<ExampleClass_1>
		<constructor>
			<int> 3 </int>
			<int> 2 </int>
			<int> 1 </int>
		</constructor>
		<function>
			<string> add </string>
			<int> 1 </int>
			<int> 2 </int>
		</function>
	</ExampleClass_1>

	std::string className = ExampleClass_1;
	std::string vars = 
	"<int> 3 </int>
	<int> 2 </int>
	<int> 1 </int>"
	int args = 3;

	// USER DEFINED
	void* CreateObject(className, vars) {
		std::vector<int> v = GetVars<int>(vars);
		return CreateClass(className, v[0], v[1], v[2]) {
			return new GetRegisteredClass(className, v[0], v[1], v[2]);
		};
	};

	[](int x, int y, int z) {
	}

	

	*/
	std::unordered_map<std::string, void*> map;

	std::cout << "Running Tests...\n" << std::endl;
	int dynamicptr_tst = dynamic_pointers::RunTests();
	int defaultptr_tst = default_pointers::RunTests();
	std::cout << "\nTests Complete." << std::endl;

	std::cin.get();

	return 0;

}
