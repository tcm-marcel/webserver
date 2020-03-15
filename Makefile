CC := clang++-9
CFLAGS := -Wall -g --std=gnu++17 # -Werror -Wextra
LDFLAGS := --std=gnu++17
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

