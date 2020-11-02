# (c) Marcel Kost 2020

CC := clang++-9
CFLAGS := -I src -Wall -g --stdlib=libc++ --std=c++2a # -Werror -Wextra
LDFLAGS := --stdlib=libc++ --std=c++2a -lpthread

TARGET := bin/webserver
SRCS := $(wildcard src/*.cpp)
OBJS := $(patsubst %.cpp,bin/%.o,$(SRCS))

TEST_TARGET := bin/tests/tests
TEST_SRCS := $(wildcard tests/*.cpp)
TEST_OBJS := $(patsubst %.cpp,bin/%.o,$(TEST_SRCS))

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
	$(CC) $(CFLAGS) -MD -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJS) $(TEST_TARGET) $(OBJS)
	
.PHONY: all clean
