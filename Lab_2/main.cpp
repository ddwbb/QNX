#include "OneTimePad.hpp"

int main(int argc, char * argv[]) {
    OneTimePad otp;
    otp.init(argc, argv);
    otp();
}
