appname := app

CXX := g++
CXXFLAGS := -Wall -O3
LDFLAGS := -lglfw3

srcfiles := $(shell find . -maxdepth 1 -name "*.cpp")
objects := $(patsubst %.cpp, build/%.o, $(srcfiles:./%.cpp=%.cpp)) build/glad.o

all: build $(appname)

build:
	mkdir -p build

$(appname): $(objects)
	$(CXX) $(CXXFLAGS) -o $(appname) $(objects) $(LDFLAGS)

build/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/glad.o: glad.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

depend: .depend

.depend: $(srcfiles)
	rm -f ./.depend
	$(CXX) $(CXXFLAGS) -MM $^ | sed 's|^\(.*\)\.o:|build/\1.o:|' > ./.depend

clean:
	rm -f build/*.o
	rm -f $(appname)
	
distclean: clean
	rm -rf build
	rm -f .depend

include .depend
