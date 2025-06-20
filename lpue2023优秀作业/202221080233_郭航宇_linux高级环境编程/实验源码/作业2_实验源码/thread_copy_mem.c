#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <openssl/md5.h>
#include <time.h>

#define BLOCK_SIZE (1024 * 1024) // 1MB
#define QUEUE_SIZE 10

typedef struct
{
    char data[BLOCK_SIZE];
    off_t offset;
    int size;
} DataBlock;

typedef struct
{
    DataBlock queue[QUEUE_SIZE];
    int front;
    int rear;
    int count;
    int finish;
    pthread_mutex_t mutex;
    pthread_cond_t notEmpty;
    pthread_cond_t notFull;
} CircularQueue;

CircularQueue buffer;
char *inputFileName;
char *outputFileName;
int nConsumers;

void initQueue(CircularQueue *q)
{
    q->front = 0;
    q->rear = 0;
    q->count = 0;
    q->finish = 0;
    pthread_mutex_init(&q->mutex, NULL);
    pthread_cond_init(&q->notEmpty, NULL);
    pthread_cond_init(&q->notFull, NULL);
}

void enqueue(CircularQueue *q, DataBlock block)
{
    pthread_mutex_lock(&q->mutex);
    while (q->count == QUEUE_SIZE)
    {
        pthread_cond_wait(&q->notFull, &q->mutex);
    }
    memcpy(&q->queue[q->rear], &block, sizeof(DataBlock));
    q->rear = (q->rear + 1) % QUEUE_SIZE;
    q->count++;
    pthread_cond_signal(&q->notEmpty);
    pthread_mutex_unlock(&q->mutex);
}

int dequeue(CircularQueue *q, DataBlock *block)
{
    pthread_mutex_lock(&q->mutex);
    while (q->count == 0 && q->finish == 0)
    {
        pthread_cond_wait(&q->notEmpty, &q->mutex);
    }
    if (q->count == 0 && q->finish == 1)
    {
        pthread_cond_signal(&q->notEmpty);
        pthread_mutex_unlock(&q->mutex);
        return 0;
    }
    memcpy(block, &q->queue[q->front], sizeof(DataBlock));
    q->front = (q->front + 1) % QUEUE_SIZE;
    q->count--;
    pthread_cond_signal(&q->notFull);
    pthread_mutex_unlock(&q->mutex);
    return 1;
}

void *producer(void *arg)
{
    int fd = open(inputFileName, O_RDONLY);
    if (fd == -1)
    {
        perror("Failed to open input file");
        exit(EXIT_FAILURE);
    }

    off_t offset = 0;
    DataBlock block;
    while (1)
    {
        block.size = pread(fd, block.data, BLOCK_SIZE, offset);
        if (block.size <= 0)
        {
            pthread_mutex_lock(&buffer.mutex);
            buffer.finish = 1;
            pthread_mutex_unlock(&buffer.mutex);
            break; // End of file or error
        }
        block.offset = offset;
        enqueue(&buffer, block);
        offset += block.size;
    }

    close(fd);
    return NULL;
}

void *consumer(void *arg)
{
    int fd = open(outputFileName, O_WRONLY);
    if (fd == -1)
    {
        perror("Failed to open output file");
        exit(EXIT_FAILURE);
    }

    DataBlock block;
    while (1)
    {
        if (dequeue(&buffer, &block) == 0)
            break;
        pwrite(fd, block.data, block.size, block.offset);
    }

    close(fd);
    return NULL;
}

char *computeMD5(const char *filename)
{
    int fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        perror("Failed to open file for MD5 computation");
        exit(EXIT_FAILURE);
    }

    unsigned char c[MD5_DIGEST_LENGTH];
    char *hash = (char *)malloc(2 * MD5_DIGEST_LENGTH + 1);
    if (!hash)
    {
        perror("Failed to allocate memory for MD5 hash");
        exit(EXIT_FAILURE);
    }

    unsigned char *fileContents;
    struct stat st;
    fstat(fd, &st);
    fileContents = mmap(0, st.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (fileContents == MAP_FAILED)
    {
        perror("Failed to mmap file for MD5 computation");
        close(fd);
        exit(EXIT_FAILURE);
    }

    MD5(fileContents, st.st_size, c);
    munmap(fileContents, st.st_size);
    close(fd);

    for (int i = 0; i < MD5_DIGEST_LENGTH; i++)
    {
        sprintf(&hash[i * 2], "%02x", c[i]);
    }
    return hash;
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Usage: %s <input_file> <output_file> <number_of_consumer_threads>\n", argv[0]);
        return EXIT_FAILURE;
    }

    clock_t start_time, finish_time;
    double total_time;
    start_time = clock();

    inputFileName = argv[1];
    outputFileName = argv[2];
    nConsumers = atoi(argv[3]);

    int src_fd = open(inputFileName, O_RDONLY);
    struct stat st;
    if (fstat(src_fd, &st) != 0)
    {
        perror("Failed to get size of source file");
        exit(EXIT_FAILURE);
    }
    close(src_fd);

    int dest_fd = open(outputFileName, O_RDWR | O_CREAT, 0666);
    if (dest_fd == -1)
    {
        perror("Failed to open or create destination file");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(dest_fd, st.st_size) != 0)
    {
        perror("Failed to set size of destination file");
        exit(EXIT_FAILURE);
    }
    close(dest_fd);

    initQueue(&buffer);

    pthread_t prodThread;
    pthread_create(&prodThread, NULL, producer, NULL);

    pthread_t *consThreads = (pthread_t *)malloc(nConsumers * sizeof(pthread_t));
    for (int i = 0; i < nConsumers; i++)
    {
        pthread_create(&consThreads[i], NULL, consumer, NULL);
    }

    pthread_join(prodThread, NULL);
    for (int i = 0; i < nConsumers; i++)
    {
        pthread_join(consThreads[i], NULL);
    }

    finish_time = clock();
    total_time = ((double)(finish_time - start_time)) / CLOCKS_PER_SEC;
    printf("%f seconds\n", total_time);

    char *srcMD5 = computeMD5(inputFileName);
    char *dstMD5 = computeMD5(outputFileName);

    printf("Source File MD5: %s\n", srcMD5);
    printf("Target File MD5: %s\n", dstMD5);

    if (strcmp(srcMD5, dstMD5) == 0)
    {
        printf("File copied successfully. MD5 hashes match.\n");
    }
    else
    {
        printf("MD5 hashes do not match. File copy might be corrupted.\n");
    }

    free(srcMD5);
    free(dstMD5);
    free(consThreads);

    return EXIT_SUCCESS;
}
