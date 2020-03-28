#include "include/config.h"
#include <iostream>

#include <cstdlib>
#include <unistd.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <devctl.h>

using namespace std;

//int read_crypto();
int read_file(const char *, char data[LENGTH]);
void write_file(const char *, char data[LENGTH]);

int main(int argc, char * argv[]) {
	if (argc != 3) {
		cout << "Usage: ./client text_file output_file" << endl;
		exit(EXIT_FAILURE);
	}

	OtpContext otp;
	for (int i = 0; i < LENGTH - 1; i++)
		otp.output[i] = 0;
	otp.output[1023] = '\0';
	otp.lcg = {1, 4081, 25673, 121500, LENGTH};

	int text_length = read_file(argv[1], otp.text);

	int dd = open(DEVICE_NAME, O_RDWR);
	if (dd == -1) {
		cout << "Error in open() device" << endl;
		exit(EXIT_FAILURE);
	}

	int status = devctl(dd, CYPHER_TEXT, &otp, sizeof(otp), NULL);
	if (status) {
		cout << "Something wrong! " << status << endl; //enotty
		exit(EXIT_FAILURE);
	}

	write_file(argv[2], otp.output);

	close(dd);

	cout << "Done!" << endl;
}

int read_file(const char * file_name, char buffer[LENGTH]) {
    int fd = open(file_name, O_RDONLY, S_IWRITE | S_IREAD);
    if (fd == -1) {
        cout << "Cannot open file: " << file_name << endl;
        return 0;
    }

    int length = read(fd, buffer, LENGTH);
    close(fd);
    return length;
}

void write_file(const char * output_file_name, char output[LENGTH]) {
	int fd = open(output_file_name, O_WRONLY | O_CREAT, S_IWRITE | S_IREAD);
    if (fd == -1) {
        cout << "Cannot open Output File" << endl;
        return;
    }
    int written =  write(fd, output, LENGTH);
    cout << "Written: " << written << endl;
    close(fd);
}
