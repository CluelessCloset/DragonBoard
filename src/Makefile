SRC_FILES := $(wildcard *.c)
OBJ_FILES := $(patsubst %.c, %.o,$(SRC_FILES))
LDFLAGS := -lpthread -lmraa

CluelessCloset.exe: $(OBJ_FILES)
	gcc $(LDFLAGS) -o $@ $^

%.o: %.c
	gcc $(CFLAGS) -c -o $@ $<