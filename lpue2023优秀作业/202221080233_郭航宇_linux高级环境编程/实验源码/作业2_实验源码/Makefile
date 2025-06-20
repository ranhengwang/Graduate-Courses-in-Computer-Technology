CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = -lpthread -lcrypto
# TARGET = mem_file_copier
TARGET = file_copy
SRC = thread_copy_mem.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)
