CXML - a header only XML parsing C++11 library ![C/C++ CI](https://github.com/da772/CXML/workflows/C/C++%20CI/badge.svg)
---

CXML was designed as a way to dynamically instantiate C++ classes and types using a single XML file. CXML allows for instantiating classes with custom memory allocation (ex. shared/unique pointer) and instantiating default C++ types (ex. int/long/float). 

Usage
---
Check tests directory for more detailed examples

### Default C++ types
```cpp
#include <CXML/CXML.hpp>

int main() {
  
  std::string cxmlStr = R"(
  <int testtag="my int"> 12 </int>
  <float > 5.5 </float>
  <long> 222 </long>
  <string> Hello World! </string>
  <test> 25 </test>
  )";
  
  CXML cxml;
  CXML::CXML_Node node = cxml.GetNext(cxmlStr.c_str()); // <int testtag="my int"> 12 </int>
  std::string intTag = node.tags["testtag"];
  if (node.type == "int") // check node type
    int i = cxml.GetNodeAsInt(node); // Cast node to int (12)
  node = cxml.GetNext(cxmlStr.c_str(), node); // <float > 5.5 </float>
  float f = cxml.GetNodeAsFloat(node); // Cast node to float (5.5)
  node = cxml.GetNext(cxmlStr.c_str(), node); // <long> 222 </long>
  long l = cxml.GetNodeAsLong(node); // Cast node to long (222)
  node = cxml.GetNext(cxmlStr.c_str(), node); // <string> Hello World! </string>
  std::string str = node.info; // Get node as string (Hello World!)
  node = cxml.GetNext(cxmlStr.c_str(), node); // <test> 25 </test>
  if (node.type == "test") // check node type 
    int test = cxml.GetNodeAsInt(node); // Get node as int (25)
  node = cxml.GetNext(cxmlStr.c_str(), node);
  bool b = CXML::ValidNode(node); // eof returns invalid node
    

  return 0;
}

```
### Custom C++ class (no arguments)
```cpp
#include <CXML/CXML.hpp>

class ExampleClass {
  public:
    ExampleClass() {};
    ~ExampleClass() {};
};

int main() {
  std::string cxmlStr = R"(
  <ExampleClass> </ExampleClass>
  )";
  
  CXML cxml;
  cxml.RegisterClass<ExampleClass>("ExampleClass"); // Register class
  
  ExampleClass* exampleClass = cxml.GetNextAsClass<ExampleClass>(cxmlStr.c_str()); // Get Class
  delete exampleClass;
  
  cxml.ClearRegistry(); // Clean up registry

  return 0;
}
```

Running tests
---
### Linux
    1. Run ./scripts/linuxGenerate.sh
    2. make
### Windows
    1. Run ./scripts/vs19Generate.bat
    2. open CXML.sln

