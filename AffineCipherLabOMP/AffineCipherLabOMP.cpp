#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <map>
#include <omp.h>

int testsNum = 6;

int a = 7;
int b = 6;

void encryption(char* m, char*& c, int len, int procNum) {

    int part = len / procNum;

#pragma omp parallel for
    for (int i = 0; i < procNum; ++i)
    {
        int nlen = (i + 1) * part;

        for (int j = i * part; j < nlen; j++) {
            if (m[j] != ' ') {
                c[j] = (char)((((a * (m[i] - 'A')) + b) % 26) + 'A');
            }
            else {
                c[j] = m[j];
            }
        }
    }
}

void decryption(char* c, char*& m, int len, int procNum) {
    int a_inverse = 0;
    int flag = 0;

    for (int i = 0; i < 26; i++) {

        flag = (a * i) % 26;
        if (flag == 1) {
            a_inverse = i;
        }
    }

    int part = len / procNum;

#pragma omp parallel for
    for (int i = 0; i < procNum; ++i)
    {
        int nlen = (i + 1) * part;

        for (int j = i * part; j < nlen; ++j) {
            if (c[j] != ' ') {
                m[j] = (char)(((a_inverse * ((c[j] + 'A' - b)) % 26)) + 'A');
            }
            else {
                m[j] = c[j];
            }
        }
    }
}

void GetRandWord(char*& word, int len)
{
    srand(15);

    for (int i = 0; i < len; ++i) {
        word[i] = rand() % 26 + 'A';
    }
}

int main(int argc, char* argv[]) {

    int procNum = 4;
    std::cout << procNum << std::endl;

    int len = 24 * 10000000;

    for (int k = 1; k < testsNum + 1; ++k)
    {
        double timeSum = 0;
        int nlen = len * k;
        char* word = new char[nlen];
        GetRandWord(word, nlen);

        for (int i = 0; i < 10; ++i)
        {
            char* encoded = new char[len];
            char* decoded = new char[len];

            int part = len / procNum;

            double start = omp_get_wtime();
            encryption(word, encoded, len, procNum);
            decryption(encoded, decoded, len, procNum);
            double stop = omp_get_wtime();

            timeSum += stop - start;

            std::cout << nlen << "     " << stop - start << std::endl;

            delete[] encoded;
            delete[] decoded;
        }
        std::cout << "AVERAGE " << nlen << "    " << timeSum / 10 << std::endl;
    }

    std::cin.get();
    return 0;
}