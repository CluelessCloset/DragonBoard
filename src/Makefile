SRC_FILES := $(wildcard *.cpp)
OBJ_FILES := $(patsubst %.cpp, %.o,$(SRC_FILES))
LDFLAGS := -lpthread -lmraa

CluelessCloset.exe: $(OBJ_FILES)
	gpp $(LDFLAGS) -o $@ $^

%.o: %.cpp
	gpp $(CFLAGS) -c -o $@ $<