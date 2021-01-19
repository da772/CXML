#!/bin/bash
here="`dirname \"$0\"`"
cd "$here"
cd ..
./vendor/bin/premake/premake5 gmake2 --os=linux
