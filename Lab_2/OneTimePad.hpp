#ifndef _OTP_
#define _OTP_

#include <cstdlib>
#include <iostream>

#include <unistd.h>
#include <getopt.h>
#include <cstring>

#include <fcntl.h>

#include <sys/stat.h>
#include <sys/sysinfo.h>

#include <pthread.h>

#define FILE_LENGTH 10000

using namespace std;

struct WorkerData {
    pthread_barrier_t * barrier;
    char * crypto_buffer;
    char * text_buffer;
    char * output_buffer;
    int length;
};

class OneTimePad {
    pthread_barrier_t * barrier;

    pthread_t * worker_threads;

    const char * crypto_file_name;
    const char * text_file_name;
    const char * output_file_name;

    char * crypto;
    char * text;
    char * output;

    int crypto_file_length;
    int text_file_length;
    int length;

    int worker_count;

    struct WorkerData * worker_datas;

    bool initialized;

    int read_file(const char *, char *);

    void worker_init();
    void thread_join();
public:
    OneTimePad();
    ~OneTimePad();

    void init(int, char * a[]);

    void operator()();
};

void * worker_processing(void *);

#endif
