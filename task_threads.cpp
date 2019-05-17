#include <ctime>
#include <cstdlib>
#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <vector>
#include <thread>
#include <math.h>

using namespace std;

void display(vector <int> &mat) {
    int size = mat.size();
    int rows = (int)sqrt(size);

    cout << "Output Matrix:" << endl;

    for (int i = 0; i < size; i++) {
        cout << mat[i] << " ";

        if ((i + 1) % rows == 0) {
            cout << endl;   
        }
    }
}

void matMulThread(vector<int> &result, vector <int> &mat1, vector <int> &mat2, int thread_number) {
    int size = mat1.size();
    int rows = (int)sqrt(size);

    for (int j = 0; j < rows; j++) {
        int temp_value = 0;

        for (int k = 0; k < rows; k++) {
            temp_value += mat1[thread_number * rows + k] * mat2[k * rows + j];
        }

        result[thread_number * rows + j] = temp_value;
    }
}

void fillMat(vector <int> &mat) {
    int size = mat.size();

    for (int i = 0; i < size; i++) {
        mat[i] = (rand() % 20) + 1;    
    }
}

int main(int argc, char* argv[]) {
    // TODO get shape from params
    srand(time(0));
    unsigned int size = atoi(argv[1]);
    vector <int> mat1(size * size);
    vector <int> mat2(size * size);
    vector <int> result(size * size, 0);

    unsigned int cores = std::thread::hardware_concurrency();
    std::cout << "Number of cores: " << cores << endl;

    std::thread workers[size];

    // Fill matrix with random numbers
    fillMat(mat1);
    fillMat(mat2);

    auto start = std::chrono::high_resolution_clock::now();

    // Create one thread from every column.
    for (unsigned int i = 0; i < size; i++) {
        workers[i] = std::thread(matMulThread, std::ref(result), std::ref(mat1), std::ref(mat2), i);
    }

    for (auto & t : workers) {
        t.join();
    }

    display(mat1);
    display(mat2);

    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> elapsed = finish - start;

    display(result);

    cout << size << " : " << elapsed.count() << endl;

    return 0;
}