#pragma once
#ifndef XCML_HEADER
#define XCML_HEADER

#include <unordered_map>
#include <functional>
#include <vector>
#if defined(CXML_ASSERT) && !defined(ASSERT)
    #include <cassert>
    #define ASSERT(x) assert(x);
#endif
#include <string.h>

#ifndef ASSERT
    #define ASSERT(x)
#endif

namespace CXML {

extern std::unordered_map<std::string, void*> class_registry;
static size_t strMax = std::numeric_limits<std::size_t>::max(); 

template<typename T, typename... Arguments>
class CXML_Class {
    public:
    void* data;
    inline void* Allocate(Arguments&& ... args) {
        data = allocateFunc(std::forward<Arguments>(args)...);
        return data;
    }
    inline void Process(const char* string) {
        processFunc(string);
    }
    inline void SetProcessFunc(const std::function<void(const char*)>& func) {
        processFunc = func;
    }
    inline void SetAllocateFunc(const std::function<void*(Arguments...)>& func) {
        allocateFunc = func;
    }
    private:
    std::function<void(const char*)> processFunc;
    std::function<void*(Arguments...)> allocateFunc = [](Arguments&& ... args) {
        return (void*) new T(std::forward<Arguments>(args)...);
    };
};

struct CXML_Node {
    std::string type;
    std::string info;
    size_t endPos;
};

template<typename... Arguments>
inline CXML_Class<Arguments...>* RegisterClass(const char* name) {
    ASSERT(class_registry.find(name) != class_registry.end());
    CXML_Class<Arguments...>* s = new CXML_Class<Arguments...>();
    class_registry[name] = (void*)s;
    return s;
}

inline void DeregisterClass(const char* name) {
    ASSERT(class_registry.find(name) != class_registry.end());
    class_registry.erase(name);
}



inline void* GetClass(const char* name) {
    ASSERT(class_registry.find(name) != class_registry.end());
    return class_registry[name];
}

inline void ClearRegistry() {
    for (const std::pair<std::string, void*> pair : class_registry) {
        free(pair.second);
    }
    class_registry.empty();
}

inline bool ValidNode(const CXML_Node& node) {
    return node.type != "\0" && node.info != "\0" && node.endPos != 0;
}

inline CXML_Node GetNext(const char* str, size_t startPos = 0) {
    uint8_t phase = 0;
    size_t lastChar = 0; 
    CXML_Node node = {"\0", "\0", 0};
    int skip = 0;
    
    for (int i = startPos; i < strMax; i++) {
        if (str[i] == '\0') break;
        switch (phase) {
            case 0: {
                if (str[i] == '<' && str[i+1] != '/') {
                    lastChar = i+1;
                    i++;
                    phase = 1;
                }
                break;
            }
            case 1: {
                if (str[i] == '>') {
                    node.type = std::string(&str[lastChar], i-lastChar);
                    lastChar = i;
                    phase = 2;
                }
                break;
            }
            case 2: {
                if (str[i] == '<') {
                    if (str[i+1] == '/') {
                        if (std::string(&str[i+2], node.type.size()) == node.type) {
                            if (!skip) {
                            node.info = std::string(&str[lastChar+1], i-1-lastChar);
                            phase++;
                            node.endPos = i+3+node.type.size();
                            } else {
                                skip--;
                            }
                        }
                    } else if (std::string(&str[i+1], node.type.size()) == node.type ) {
                            skip++;
                    }
                }
                break;
            }
            default:
                break;
        }
        
        if (phase == 3) break;
    }
    return std::move(node);
}


template<typename T>
inline T* GetNextAsClass(const char* info) {
    CXML_Node node = GetNext(info);
    CXML_Class<T>* clss = (CXML_Class<T>*)GetClass(node.type.c_str());
    clss->Process(node.info.c_str());
    return (T*)clss->data;
}

template<typename T>
inline T* GetNodeAsClass(const CXML_Node& node) {
    CXML_Class<T>* clss = (CXML_Class<T>*)GetClass(node.type.c_str());
    clss->Process(node.info.c_str());
    return (T*)clss->data;
}


inline const std::string GetNextAsString(const char* str, size_t startPos = 0,size_t* endPos = nullptr ) {
    std::string i;
    CXML_Node n = GetNext(str, startPos);
    i = n.info;

    if (endPos)
        *endPos = n.endPos;
    return i;
}

inline const std::vector<int> GetNextAsInts(const char* str, int amt, size_t startPos = 0, size_t* endPos = nullptr) {
    std::vector<int> vec;
    CXML_Node node = GetNext(str, startPos);
    vec.push_back(std::stoi(node.info));
    for (int i = 1; i < amt; i++) {
        node = GetNext(str, node.endPos);
        vec.push_back(stoi(node.info));
    }
    if (endPos)
        *endPos = node.endPos;
    return std::move(vec);
}

inline const int GetNextAsInt(const char* str, size_t startPos = 0,size_t* endPos = nullptr ) {
    int i;
    CXML_Node n = GetNext(str, startPos);
    i = std::stoi(n.info);

    if (endPos)
        *endPos = n.endPos;

    return i;
}

inline const std::vector<float> GetNextAsFloats(const char* str, int amt, size_t startPos = 0, size_t* endPos = nullptr) {
    std::vector<float> vec;
    CXML_Node node = GetNext(str, startPos);
    vec.push_back(std::stof(node.info));
    for (int i = 1; i < amt; i++) {
        node = GetNext(str, node.endPos);
        vec.push_back(stof(node.info));
    }
    if (endPos)
        *endPos = node.endPos;
    return std::move(vec);
}

inline const float GetNextAsFloat(const char* str, size_t startPos = 0,size_t* endPos = nullptr ) {
    float i;
    CXML_Node n = GetNext(str, startPos);
    i = std::stof(n.info.c_str());
    if (endPos)
        *endPos = n.endPos;
    return i;
}

inline const std::vector<long> GetNextAsLongs(const char* str, int amt, size_t startPos = 0, size_t* endPos = nullptr) {
    std::vector<long> vec;
    CXML_Node node = GetNext(str, startPos);
    vec.push_back(std::stol(node.info));
    for (int i = 1; i < amt; i++) {
        node = GetNext(str, node.endPos);
        vec.push_back(stol(node.info));
    }
    if (endPos)
        *endPos = node.endPos;
    return std::move(vec);
}

inline const long GetNextAsLong(const char* str, size_t startPos = 0,size_t* endPos = nullptr ) {
    long i;
    CXML_Node n = GetNext(str, startPos);
    i = std::stol(n.info.c_str());
    if (endPos)
        *endPos = n.endPos;
    return i;
}

inline const std::vector<long> GetNextAsHexs(const char* str, int amt, size_t startPos = 0, size_t* endPos = nullptr) {
    std::vector<long> vec;
    CXML_Node node = GetNext(str, startPos);
    vec.push_back(std::stol(node.info,0,16));
    for (int i = 1; i < amt; i++) {
        node = GetNext(str, node.endPos);
        vec.push_back(stol(node.info,0,16));
    }
    if (endPos)
        *endPos = node.endPos;
    return std::move(vec);
}

inline const long GetNextAsHex(const char* str, size_t startPos = 0,size_t* endPos = nullptr ) {
    long i;
    CXML_Node n = GetNext(str, startPos);
    i = std::stol(n.info.c_str(),0,16);
    if (endPos)
        *endPos = n.endPos;
    return i;
}

}

#endif