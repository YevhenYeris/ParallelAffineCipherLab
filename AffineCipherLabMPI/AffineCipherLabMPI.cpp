#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <exception>
#include <map>
#include "mpi.h"

int ProcNum;
int ProcRank;
int a = 7;
int b = 6;

void encryption(char* m, char*& c, int len) {
    for (int i = 0; i < len; i++) {
        if (m[i] != ' ')
            c[i] = (char)((((a * (m[i] - 'A')) + b) % 26) + 'A');
        else
            c[i] = m[i];
    }
}

void decryption(char* c, char*& m, int len) {
    int a_inverse = 0;
    int flag = 0;
    for (int i = 0; i < 26; i++) {
        flag = (a * i) % 26;
        if (flag == 1) {
            a_inverse = i;
        }
    }

    for (int i = 0; i < len; i++) {
        if (c[i] != ' ')
            m[i] = (char)(((a_inverse * ((c[i] + 'A' - b)) % 26)) + 'A');
        else
            m[i] = c[i];
    }
}

void GetRandWord(char*& word, int len)
{
    srand(15);

    for (int i = 0; i < len; ++i) {
        word[i] = rand() % 26 + 'A';
    }
}

char* word;
char* encoded;
char* decoded;

int main(int argc, char* argv[]) {

    std::stringstream str(argv[argc - 1]);
    int k;
    str >> k;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    int len = 24 * k * 10000000;

    if (ProcNum == 0)
    {
        word = new char[len];
        encoded = new char[len];
        decoded = new char[len];
        GetRandWord(word, len);
    }

    int part = len / ProcNum;

    char* bufWord = new char[part];
    char* bufEncoded = new char[part];
    char* bufDecoded = new char[part];

    if (ProcNum == 0)
    {
        MPI_Scatter(word, part, MPI_CHAR, bufWord, part, MPI_CHAR, 0, MPI_COMM_WORLD);
    }

    double start = MPI_Wtime();
    encryption(bufWord, bufEncoded, part);
    decryption(bufEncoded, bufDecoded, part);
    double stop = MPI_Wtime();

    if (ProcNum == 0)
    {
        MPI_Gather(bufEncoded, part, MPI_CHAR, encoded, part, MPI_CHAR, 0, MPI_COMM_WORLD);
        MPI_Gather(bufWord, part, MPI_CHAR, word, part, MPI_CHAR, 0, MPI_COMM_WORLD);
        MPI_Gather(bufDecoded, part, MPI_CHAR, decoded, part, MPI_CHAR, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    if (ProcRank == 0) {
        std::cout << len << "     " << stop - start << std::endl;
    }

    return 0;
}