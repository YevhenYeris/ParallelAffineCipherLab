#include <string>
#include <iostream>
#include <chrono>
#include <sstream>
#include <map>

int testsNum = 6;
//std::map<int, char*> testData;
std::map<int, int> serialTest;

char** testData = new char* [testsNum];

int a = 7;
int b = 6;

void encryption(char* m, char*& c, int len) {
    for (int i = 0; i < len; i++) {
        if (m[i] != ' ') {
            c[i] = (char)((((a * (m[i] - 'A')) + b) % 26) + 'A');
        }
        else {
            c[i] = m[i];
        }
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
        if (c[i] != ' ') {
            m[i] = (char)(((a_inverse * ((c[i] + 'A' - b)) % 26)) + 'A');
        }
        else {
            m[i] = c[i];
        }
    }
}

void GetRandWord(char*& word, int len) {

    srand(15);
    for (int i = 0; i < len; ++i) {
        word[i] = rand() % 26 + 'A';
    }
}

void printResult()
{
    for (auto t : serialTest)
    {
        std::cout << t.first << "   " << t.second << std::endl;
    }
}

double encodeDecode(char* word, int len) {

    char* encoded = new char[len];
    char* decoded = new char[len];

    auto start = std::chrono::high_resolution_clock::now();
    encryption(word, encoded, len);
    decryption(encoded, decoded, len);
    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    double time = (double)duration.count() / 1000000;

    std::cout << len << "    " << (double)duration.count() / 1000000 << std::endl;

    delete[] encoded;
    delete[] decoded;

    return time;
}

int main() {

    int len = 24 * 10000000;
    /*for (int k = 1; k < testsNum + 1; ++k) {

        int nlen = len * k;
        char* word = new char[nlen];
        GetRandWord(word, nlen);

        testData[k - 1] = word;
    }*/

    for (int k = 1; k < testsNum + 1; ++k)
    {
        double timeSum = 0;
        int nlen = len * k;
        char* word = new char[nlen];
        GetRandWord(word, nlen);

        for (int i = 0; i < 10; ++i)
        {
            timeSum += encodeDecode(word, nlen);
        }
        std::cout << "AVERAGE " << nlen << "    " << timeSum / 10 << std::endl;
    }

    std::cin.get();
    return 0;
}