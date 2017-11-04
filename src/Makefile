SRC_FILES := $(wildcard *.cpp)
OBJ_FILES := $(patsubst %.cpp, %.o,$(SRC_FILES))
LDFLAGS := -lpthread -lmraa

CluelessCloset: $(OBJ_FILES)
	g++ $(LDFLAGS) -o $@ $^

%.o: %.cpp %.hpp
	g++ $(CPPFLAGS) -c $@ $<