cc ?= g++
cflags ?= -Wall
cppflags ?= -Wall

cpp_src = $(wildcard *.cpp)
c_src += $(wildcard *.c)

cpp_objs = $(patsubst %.cpp,%.o,$(cpp_src))

include_path = -I../include

.PHONY: all clean
all: $(cpp_objs)

objs = $(wildcard *.o)
# filesystems.o: $(fs_depency)
# 	g++ -c $(cppflags) $(include_path) $(fs_depency) -o filesystems.o

%.o: %.cpp
	g++ -c $(cppflags) $(include_path) $(objs) $< -o $@

%.o: %.c
	gcc -c $(cflags) $(include_path) $< -o $@

clean:
	rm -rf *.o
	rm -rf demo
