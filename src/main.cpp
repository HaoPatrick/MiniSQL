#include "BufferManager/Buffer.h"

int main(int argc, char *argv[]) {
    Buffer aa(FILE_PATH);
    aa.write_sample_data();
    aa.read_sample_data(45);
    return 0;
}