# credit to: https://gist.github.com/Wenchy/64db1636845a3da0c4c7

CC := g++
CFLAGS := -Wall -g
TARGET := run

# $(wildcard *.cpp /xxx/xxx/*.cpp): get all .cpp files from the current directory and dir "/xxx/xxx/"
SRCS := $(wildcard *.cpp)
# $(patsubst %.cpp,%.o,$(SRCS)): substitute all ".cpp" file name strings to ".o" file name strings
OBJS := $(patsubst %.cpp,%.o,$(SRCS))

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) -o $@ $^
	rm -f *.o *~ 
%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(TARGET) *.o
	
.PHONY: all clean