#include "OneTimePad.hpp"

OneTimePad::OneTimePad() {
    initialized = false;
    crypto = new char[FILE_LENGTH + 1];
    text = new char[FILE_LENGTH + 1];

    crypto[FILE_LENGTH] = '\0';
    text[FILE_LENGTH] = '\0';

    output = nullptr;
    barrier = nullptr;
    worker_datas = nullptr;
    worker_threads = nullptr;
}

OneTimePad::~OneTimePad() {
    delete crypto;
    delete text;
    if (output) delete output;

    if (worker_datas) delete worker_datas;
    if (worker_threads) delete worker_threads;
    if (barrier) delete barrier;
}

void OneTimePad::operator() () {
    if (!initialized) {
        cout << "Usage: ./otp /path/to/crypto_file /path/to/text_file /path/to/output_file" << endl;
        cout << "Or Use flags: ./otp -c crypto_file -t text_file -o output_file" << endl;
        return;
    }

    cout << "Input Crypto File: " << crypto_file_name << endl;
    cout << "Input Text File: " << text_file_name << endl;
    cout << "Input Output File: " << output_file_name << endl;

    crypto_file_length = read_file(crypto_file_name, crypto);
    text_file_length = read_file(text_file_name, text);

    if (!crypto_file_length) {
        cout << crypto_file_name << " is empty :(" << endl;
        return;
    }
    if (!text_file_length) {
        cout << text_file_name << " is empty :(" << endl;
        return;
    }

    length = (crypto_file_length < text_file_length) ? crypto_file_length : text_file_length;

    output = new char[length + 1];
    output[length] = '\0';

    cout << "Length of file: " << length << endl;

    worker_count = sysconf(_SC_NPROCESSORS_ONLN);

    barrier = new pthread_barrier_t;
    worker_threads = new pthread_t[worker_count];
    pthread_barrier_init(barrier, NULL, worker_count + 1);

    worker_init();
    thread_join();
}

void OneTimePad::init(int argc, char * argv[]) {
    if (argc != 7) return;
    int c;
    while ((c = getopt(argc, argv, "c:t:o:")) != -1) {
        switch (c) {
            case 'c':
                crypto_file_name = optarg;
                break;
            case 't':
                text_file_name = optarg;
                break;
            case 'o':
                output_file_name = optarg;
                break;
            case '?':
                return;
        }
    }
    initialized = true;
}

int OneTimePad::read_file(const char * file_name, char * buffer) {
    int fd = open(file_name, O_RDONLY, S_IWRITE | S_IREAD);
    if (fd == -1) {
        cout << "Cannot open file: " << file_name << endl;
        return 0;
    }

    int length = read(fd, buffer, FILE_LENGTH);
    close(fd);
    return length;
}

void OneTimePad::worker_init() {
    worker_datas = new struct WorkerData[worker_count];

    int worker_length = length / worker_count;
    for (int i = 0; i < worker_count; i++) {
        worker_datas[i].barrier = barrier;
        worker_datas[i].length = worker_length;
        if (i == (worker_count - 1))
            worker_datas[i].length += length % worker_count;
        worker_datas[i].crypto_buffer = new char[worker_datas[i].length];
        worker_datas[i].text_buffer = new char[worker_datas[i].length];
        worker_datas[i].output_buffer = new char[worker_datas[i].length + 1];
        worker_datas[i].output_buffer[worker_datas[i].length] = '\0';

        memcpy(worker_datas[i].crypto_buffer, &crypto[i * worker_length], worker_datas[i].length);
        memcpy(worker_datas[i].text_buffer, &text[i * worker_length], worker_datas[i].length);

        pthread_create(&worker_threads[i], NULL, worker_processing, &worker_datas[i]);
    }
}

void OneTimePad::thread_join() {
    pthread_barrier_wait(barrier);

    for (int i = 0; i < worker_count; i++)
        strcat(output, worker_datas[i].output_buffer);

    int fd = open(output_file_name, O_WRONLY | O_CREAT, S_IWRITE | S_IREAD);
    if (fd == -1) {
        cout << "Cannot open Output File" << endl;
        return;
    }
    int written =  write(fd, output, length);
    close(fd);

    cout << "Written: " << written << endl;

    pthread_barrier_destroy(barrier);
}

void * worker_processing(void * arg) {
    struct WorkerData * data = (struct WorkerData *) arg;
    for(int i = 0; i < data->length; i++)
        data->output_buffer[i] = (data->crypto_buffer[i] ^ data->text_buffer[i]);
    pthread_barrier_wait(data->barrier);
    return arg;
}
