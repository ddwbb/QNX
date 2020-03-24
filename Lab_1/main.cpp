#include <cstdlib>
#include <iostream>

#include <pthread.h>

#include "LCG.hpp"

using namespace std;

int init(LCG *, int, char * a[]);
void * lcg_processing(void *);

int main(int argc, char * argv[]) {
    LCG lcg;

    if (init(&lcg, argc, argv)) return 0;

    pthread_t lcg_thread;
    pthread_create(&lcg_thread, NULL, lcg_processing, &lcg);

    pthread_join(lcg_thread, NULL);

    cout << "Done!" << endl;

    return 0;
}

int init(LCG * lcg, int argc, char * argv[]) {
    const char * file_name = nullptr;
    if (argc < 6 && argc != 1) {
        cout << "Usage: ./lcg x0 a c m n" << endl;
        return -1;
    }
    if (argc == 7) file_name = argv[6];
    if (argc >= 6) {
        int x = atoi(argv[1]);
        int a = atoi(argv[2]);
        int c = atoi(argv[3]);
        int m = atoi(argv[4]);
        int n = atoi(argv[5]);

        lcg->init(x, a, c, m, n, file_name);
        return 0;
    }

    return 0;
}

void * lcg_processing(void * l) {
    LCG * lcg = (LCG *) l;

    (*lcg)();
    lcg->write();

    return lcg;
}
