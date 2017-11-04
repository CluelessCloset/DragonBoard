SRC_FILES := $(wildcard *.cpp)
OBJ_FILES := $(patsubst %.cpp, %.o,$(SRC_FILES))
LDFLAGS := -lpthread -lmraa -lboost_date_time -lboost_random -lboost_system
CPPFLAGS += -g
C_INCLUDE_PATH += /root/socket.io-client-cpp/build/include
CPLUS_INCLUDE_PATH += /root/socket.io-client-cpp/build/include
LIBRARY_PATH = /root/socket.io-client-cpp/build/lib/Release

.PHONY: clean

.PHONY: clean

CluelessCloset: $(OBJ_FILES)
	g++ $(LDFLAGS) -o $@ $^

%.o: %.cpp %.hpp
	g++ $(CPPFLAGS) -c $< -o $@ 

clean:
	rm *.o