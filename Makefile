CROSSCXX := x86_64-w64-mingw32-g++

main:
	$(CROSSCXX) src/main.cpp src/ui.cpp src/wordparser.cpp -Wall -Wno-sign-compare -O2 -o main.exe
