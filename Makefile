#Thanks for the sample makefile:  https://sites.google.com/site/michaelsafyan/software-engineering/how-to-write-a-makefile
program_NAME = ClosetManager
program_NAME_TEST = tests_ClosetManager
program_SRCS =  src/*.c
program_SRCS_TESTS = tests/*.c
program_OBJS = src/*.o
program_OBJS_TESTS = tests/*.o
program_INCLUDE_DIRS = src/ tests/
program_LIBRARY_DIRS =
program_LIBRARIES = pthreads

CPPFLAGS += $(foreach includedir,$(program_INCLUDE_DIRS),-I$(includedir))
LDFLAGS += $(foreach librarydir,$(program_LIBRARY_DIRS),-L$(librarydir))
LDFLAGS += $(foreach library,$(program_LIBRARIES),-l$(library))

.PHONY: all test clean distclean

all: $(program_NAME)

test: $(program_NAME_TEST)

$(program_NAME): $(program_OBJS)
	$(LINK.cc) $(program_OBJS) -o $(program_NAME)

$(program_NAME_TEST): $(program_OBJS_TESTS)
	$(LINK.cc) $(program_OBJS_TESTS) -o $(program_NAME_TEST)

clean:
	@- $(RM) $(program_NAME)
	@- $(RM) $(program_OBJS)
	@	 $(RM) $(program_OBJS_TESTS)

distclean: clean