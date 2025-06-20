cc ?= g++

cflags = -Wall
cppflags = -Wall

ifeq ($(debug), 1)
cflags += -DDEBUG
cppflags += -DDEBUG 
endif

ifeq ($(optg), 1)
cflags += -g
cppflags += -g
endif

objs = fs/filesystems.o fs/inode.o fs/super.o fs/bitmap.o \
	   net/stream_reciever.o net/stream_info.o \
	   tools/thread_manager.o

include_path = -I./include

export cc cflags cppflags

.PHONY: all clean fs_make tools_make net_make

all: fs_make tools_make write_file_demo operate_file_demo

write_file_demo: $(objs) write_file.o
	$(cc) -o write_file_demo $(cppflags) $(include_path) $(objs) write_file.o

operate_file_demo: $(objs) operate_file.o
	$(cc) -o operate_file_demo $(cppflags) $(include_path) $(objs) operate_file.o

fs_make:
	$(MAKE) -C fs

tools_make:
	$(MAKE) -C tools

net_make:
	$(MAKE) -C net

%.o: %.cpp
	g++ -c $(cppflags) $(include_path) $< -o $@

clean:
	$(MAKE) clean -C fs
	$(MAKE) clean -C tools
	$(MAKE) clean -C net
	rm -rf *.o
	rm -rf *demo



