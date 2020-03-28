#include "include/LCG.hpp"

LCG::LCG() {
    start_value = 1;
    a = 4081;
    c = 25673;
    m = 121500;
    count = 100;

    prn = new int[count];
    file_name = FILE_NAME;
}

LCG::LCG(int x, int _a, int _c, int _m, int n, const char * str) {
    start_value = x;
    a = _a;
    c = _c;
    m = _m;
    count = n;

    prn = new int[count];
    if (str) file_name = str;
    else file_name = FILE_NAME;
}

LCG::~LCG() {
    delete prn;
}

void LCG::init(int x, int _a, int _c, int _m, int n, const char * str) {
    start_value = x;
    a = _a;
    c = _c;
    m = _m;
    count = n;

    if (prn) delete prn;
    prn = new int[count];
    if (str) file_name = str;
    else file_name = FILE_NAME;
}

int * LCG::operator()() {
    prn[0] = start_value;
    for (int i = 1; i < count; i++) {
        prn[i] = (a * prn[i - 1] + c) % m % 127;
    }
    return prn;
}

void LCG::write() {
    file.open(file_name);
    if (file.is_open()) {
        for (int i = 0; i < count; i++) {
            file << prn[i] << endl;
        }
    }
    else {
        return;
    }
    file.close();
}
