#!/bin/bash
g++ -g -c tests/test.cpp -o obj/test.o -Iinclude
g++ -g -c tests/default_pointers.cpp -o obj/default_pointers.o -Iinclude/
g++ -g -c tests/dynamic_pointers.cpp -o obj/dynamic_pointers.o -Iinclude/
g++ -g obj/CXML.o obj/default_pointers.o obj/dynamic_pointers.o obj/test.o -o bin/Tests
chmod +x bin/Tests
./bin/Tests

