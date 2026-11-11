#include "BST.h"
#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <fstream>
#include <string>

using namespace std::chrono;

void runBenchmark(int N, std::ofstream& csv) {
    std::cout << "Running benchmark for N = " << N << "...\n";

    std::vector<int> data(N);
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(1, N * 10);
    
    for (int i = 0; i < N; ++i) {
        data[i] = dist(rng);
    }

    double insert_ms = 0, search_ms = 0, remove_ms = 0;

    {
        lab::BST<int> tree;
        auto start = high_resolution_clock::now();
        for (int i = 0; i < N; ++i) {
            tree.Insert(data[i]);
        }
        auto end = high_resolution_clock::now();
        insert_ms = duration<double, std::milli>(end - start).count();

        start = high_resolution_clock::now();
        for (int i = 0; i < N; ++i) {
            tree.Search(data[i]);
        }
        end = high_resolution_clock::now();
        search_ms = duration<double, std::milli>(end - start).count();

        start = high_resolution_clock::now();
        for (int i = 0; i < N; ++i) {
            tree.Remove(data[i]);
        }
        end = high_resolution_clock::now();
        remove_ms = duration<double, std::milli>(end - start).count();
    }

    std::cout << "  Insert: " << insert_ms << " ms\n";
    std::cout << "  Search: " << search_ms << " ms\n";
    std::cout << "  Remove: " << remove_ms << " ms\n";

    csv << N << "," << insert_ms << "," << search_ms << "," << remove_ms << "\n";
}

int main() {
    std::ofstream csv("../results/tables/bench.csv");
    if (!csv.is_open()) {
        std::cerr << "Cannot open results/tables/bench.csv\n";
        return 1;
    }

    csv << "N,Insert_ms,Search_ms,Remove_ms\n";

    std::vector<int> sizes = {10000, 100000, 1000000};
    
    for (int N : sizes) {
        runBenchmark(N, csv);
    }

    csv.close();
    std::cout << "Results saved to ../results/tables/bench.csv\n";
    return 0;
}