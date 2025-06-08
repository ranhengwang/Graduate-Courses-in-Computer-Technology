cc ?= g++

cflags = -Wall
cppflags = -Wall

ifeq ($(optg), 1)
cflags += -g
cppflags += -g
endif

ifeq ($(debug), 1)
cflags += -DDEBUG
cppflags += -DDEBUG 
endif

cflags += -lpthread
cppflags += -lpthread

# include_path = -I./include
cpp_src = $(wildcard *.cpp)
cpp_objs = $(patsubst %.cpp,%.o,$(cpp_src))
# cpp_objs = consumer.o productor.o thread_params.o main.cpp

.PHONY: all clean

demo: $(cpp_objs)
	echo $(cpp_objs)
	$(cc) -o demo $(cppflags) $(include_path) $(cpp_objs)

%.o: %.cpp
	g++ -c $(cppflags) $(include_path) $< -o $@

clean:
	rm -rf *.o
	rm -rf *demo



