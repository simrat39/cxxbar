CXX = g++
CXXFLAGS = -O2 -std=c++2a -Iinclude/ -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -pthread -I/usr/include/libmount -I/usr/include/blkid
LDLIBS = -pthread -lgio-2.0 -lgobject-2.0 -lglib-2.0

SRC = $(wildcard src/Modules/*.cpp) $(wildcard src/Utils/*.cpp) $(wildcard src/*.cpp)
OBJ := $(SRC:.cpp=.o)
OUT = a.out 

$(OUT): $(OBJ)
	$(CXX) $(LDLIBS) $(OBJ) -o $@

clean:
	rm -f $(OBJ) $(OUT)
