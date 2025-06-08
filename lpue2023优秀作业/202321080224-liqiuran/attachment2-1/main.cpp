#include <cstdint>
#include <iostream>
#include "consumer.h"
#include "lengthdef.h"
#include "macros.h"
#include "productor.h"
#include <unistd.h>
#include <cstring>
#include <time.h>

using namespace std;

int main(int argc, char **args)
{
    if (argc != 3)
    {
        printf("args is invalid, please to check your args: productor_count consumer_count");
        return 1;
    }

    int productor_count = 2;
    int consumer_count = 1;

    productor_count = atoi(args[1]);
    consumer_count = atoi(args[2]);

    printf("productor_count: %d, consumer_count: %d\n", productor_count, consumer_count);
    clock_t start_time = clock();
    clock_t finish_time;


    int pfd[2];
    if (pipe(pfd) < 0)
    {
        ERROR("%s", "failed to open pipe");
    }
    INFO("pfd[0]: %d, pfd[1]: %d", pfd[0], pfd[1]);
    char filename[2][FILENAME_LEN];
    memset(filename[0], 0, sizeof(filename[0]));
    snprintf(filename[0], sizeof(filename[0]) - 1, "%s", "outfile");
    memset(filename[1], 0, sizeof(filename[1]));
    snprintf(filename[1], sizeof(filename[1]) - 1, "%s", "infile");

    init_consumer(consumer_count, pfd[0], filename[0]);
    init_productor(productor_count, pfd[1], filename[1]);

    // sleep(10);

    uninit_productor();
    close(pfd[1]);
    uninit_consumer();
    close(pfd[0]);

    finish_time = clock();
    printf("spend total time: %lf\n", ((double)(finish_time-start_time))/CLOCKS_PER_SEC);

    return 0;
}
