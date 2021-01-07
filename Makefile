cpp_files := $(wildcard src/*.cpp)
link_files := $(wildcard link/*.dll)
UNAME_S := $(shell uname -s)
make: $(cpp_files)
ifeq ($(UNAME_S),Linux)
	@echo "Compiling for Linux"
	g++ -o main $(cpp_files) -std=c++11 -lX11 -lGL -lGLU -lGLEW -I/usr/include/SDL `sdl-config --cflags --libs` -Dlinux
else
ifeq ($(OS),Windows_NT)
	@echo "Compiling for Windows"
	g++ -o main $(cpp_files) -std=c++11 -lglu32 -lglew32 -lopengl32 -lSDL -static-libstdc++ -Dwindows
endif
endif

run:
	make
ifeq ($(OS),Windows_NT)
	start main
endif
ifeq ($(UNAME_S),Linux)
	./main
endif
