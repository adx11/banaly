CXX = g++
CXXFLAGS = -std=c++11 -g

BIN = loader_demo
LIBS = -lbfd

$(BIN): loader_demo.o loader.o
	$(CXX) -o $@ $^ $(LIBS)

clean:
	-$(RM) -f $(BIN) *.o