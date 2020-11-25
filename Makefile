# (c) Marcel Kost 2020

CC = clang++-9
CXXFLAGS = -I src -fPIE --stdlib=libc++ --std=c++2a -flto=thin
LDFLAGS = --stdlib=libc++ --std=c++2a -lpthread -flto=thin

TARGET = bin/webserver
SRCS = $(wildcard src/*.cpp)
OBJS = $(patsubst %.cpp,bin/%.o,$(SRCS))

TEST_TARGET = bin/unit_tests
TEST_SRCS = $(wildcard tests/*.cpp)
TEST_OBJS = $(patsubst %.cpp,bin/%.o,$(TEST_SRCS))

RELEASE ?= 0
ifeq ($(RELEASE), 1)
  CXXFLAGS += -O3 -DNDEBUG
else
	CXXFLAGS += -ggdb -gsplit-dwarf -Wall -Werror -Wextra -Wno-unused-private-field -Wshadow
endif

SANITIZER ?= 0
ifeq ($(SANITIZER), ADDRESS)
	CXXFLAGS += -fsanitize=address
	LDFLAGS += -fsanitize=address
endif
ifeq ($(SANITIZER), MEMORY)
	CXXFLAGS += -fsanitize=memory
	LDFLAGS += -fsanitize=memory
endif

all: $(TARGET) tests
  $(info    SRCS: is $(SRCS))
  $(info    OBJS: is $(OBJS))

tests: $(TEST_TARGET)
	$(info    TEST_SRCS: is $(TEST_SRCS))
  $(info    TEST_OBJS: is $(TEST_OBJS))

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -MD -o $@ $^

$(TEST_TARGET): $(TEST_OBJS) $(filter-out bin/src/main.o, $(OBJS))
	$(CC) $(LDFLAGS) -MD -o $@ $^

bin/%.o: %.cpp
	@mkdir -p $(@D)
	$(CC) $(CXXFLAGS) -MD -c -o $@ $<

clean:
	rm -rf bin
	
.PHONY: all clean
