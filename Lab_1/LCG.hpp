#ifndef _LCG_
#define _LCG_

#include <cstdlib>
#include <fstream>

#define FILE_NAME "PRN_OUT"

using namespace std;

class LCG {
    int start_value;
    int a;
    int c;
    int m;
    int count;

    int * prn;

    ofstream file;
    const char * file_name;
public:
    LCG();
    LCG(int, int, int, int, int, const char * s = nullptr);

    void init(int, int, int, int, int, const char * s = nullptr);

    ~LCG();

    void operator()();
    void write();
};

#endif
