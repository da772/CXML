#!/bin/bash
g++ test/test.cpp -o tests.a -I./include/
chmod +x tests.a
./tests.a

