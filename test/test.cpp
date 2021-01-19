#include "CXML.hpp"

#include <iostream>
#include <unordered_map>
#include <string>  
#include <vector>
#include <cassert>


std::unordered_map<std::string, void*> map;

class ExampleClass_1 {
	public:
		ExampleClass_1(int x, int y, int z ) : x(x), y(y), z(z) {};
		~ExampleClass_1() {};
		int x =0,y=0,z = 0;
};


int main() {


	//RegisterClass<ExampleClass_1, int, int, int>("ExampleClass_1");

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

	CXML::CXML_Class<ExampleClass_1, int,int,int>* s = CXML::RegisterClass<ExampleClass_1, int, int, int>("ExampleClass_1");


/*
	s->AllocateFunc = [](int x, int y, int z) {
		return new ExampleClass_1(x,y,z);
	};
*/
	std::string cxml = "<ExampleClass_1> <int> 33 </int> <int> 24 </int> <ExampleClass_1> </ExampleClass_1> <int> 99</int> </ExampleClass_1> <hex> ff </hex> ";
	CXML::CXML_Node node = CXML::GetNext(cxml.c_str());

	
	s->SetAllocateFunc([](int x, int y, int z) {
		return (void*)new ExampleClass_1(x,y,z);
	});

	s->SetProcessFunc([s](const char* c) {
		size_t endPos;
		const std::vector<int> v = CXML::GetNextAsInts(c, 2, 0, &endPos);
		CXML::CXML_Node trashNode = CXML::GetNext(c, endPos);
		int z = CXML::GetNextAsInt(c, trashNode.endPos);
		s->Allocate((int)v[0],(int)v[1], (int)z);
	});
	CXML::CXML_Class<ExampleClass_1>* _class =
	 (CXML::CXML_Class<ExampleClass_1>*) CXML::GetClass(node.type.c_str());
	_class->Process(node.info.c_str());
	ExampleClass_1* ex = (ExampleClass_1*)_class->data;

	std::cout << ex->x << "," << ex->y << "," << ex->z << std::endl;

	delete ex;

	assert(node.type == "ExampleClass_1");

	std::cout << node.type << '\n' << node.info << '\n' << node.endPos << std::endl;

	if (node.type == "ExampleClass_1") {
		CXML::CXML_Class<ExampleClass_1>* nodeClass 
			= (CXML::CXML_Class<ExampleClass_1>*)CXML::GetClass(node.type.c_str());
		nodeClass->Process(node.info.c_str());
		ExampleClass_1* _ex = (ExampleClass_1*)_class->data;
		std::cout << _ex->x << "," << _ex->y << "," << _ex->z << std::endl;
	}

	CXML::CXML_Node node1 = CXML::GetNext(node.info.c_str());
	std::cout << node1.type << '\n' << std::stoi(node1.info) << '\n' << node1.endPos << std::endl;
	CXML::CXML_Node node2 = CXML::GetNext(node.info.c_str(), node1.endPos);
	std::cout << node2.type << '\n' << std::stoi(node2.info) << '\n' << node2.endPos << std::endl;
	CXML::CXML_Node node3 = CXML::GetNext(node.info.c_str(), node2.endPos);
	std::cout << node3.type << '\n' << node3.info << '\n' << node3.endPos << std::endl;
	CXML::CXML_Node node4 = CXML::GetNext(node.info.c_str(), node3.endPos);
	std::cout << node4.type << '\n' << std::stoi(node4.info) << '\n' << node4.endPos << std::endl;

	std::vector<int> mInts = CXML::GetNextAsInts(node.info.c_str(), 2);

	for (int i = 0; i < mInts.size(); i++) {
		std::cout << mInts[i] << ", ";
	}

	size_t pos;
	std::string f = CXML::GetNextAsString(cxml.c_str(), node.endPos, &pos);
	std::cout << f << std::endl;

	assert(!CXML::ValidNode(CXML::GetNext(cxml.c_str(), pos)));

	CXML::FreeRegistry();

	return 0;

}
