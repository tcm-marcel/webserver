CC := clang++-9
CFLAGS := -Wall -g --stdlib=libc++ --std=c++2a # -Werror -Wextra
LDFLAGS := --stdlib=libc++ --std=c++2a
TARGET := webserver

SRCS := $(wildcard src/*.cpp)
OBJS := $(patsubst src/%.cpp,obj/%.o,$(SRCS))

all: bin/$(TARGET)
  $(info    SRCS: is $(SRCS))
  $(info    OBJS: is $(OBJS))

bin/$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^
	
obj/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<
	
clean:
	rm -f $(TARGET) $(OBJS)
	
.PHONY: all clean

