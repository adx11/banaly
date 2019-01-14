CXX = clang++
CXXFLAGS = -std=c++11 -g

BIN = loader_demo
LIBS = -lbfd

OS := $(shell uname)
ifeq ($(OS),Darwin)
CPPFLAGS += -I/usr/local/opt/binutils/include
LDFLAGS += -L/usr/local/opt/binutils/lib -L/usr/local/opt/gettext/lib
LIBS += -liberty -lintl -lz
endif


$(BIN): loader_demo.o loader.o
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

clean:
	-$(RM) -f $(BIN) *.o
