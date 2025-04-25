/* 
Workshop: Genetic Drift Threaded
Student: John Treon
Compilation instructions: g++ -o geneticDriftThreadedWK geneticDriftForkedThreaded.cpp, ./geneticDriftForkedWK
Questions: How do the results compare to the non-threaded version? Actually really similar. 
           My histogram for this program is submitted as histogram.png. Running the old program produced
           similar results. I think the results are similar because the threads are not doing any work differently.
*/

#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <array>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int popId = 1;

void simulateGeneticDrift(int N, int G, double f, int p) {
    int num_a = f * (2 * N);
    int num_A = (2 * N) - num_a;

    std::vector<char> alleles(2 * N);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (int i = 0; i < G; i++) {
        for (int j = 0; j < 2 * N; j++) {
            double randomNum = dis(gen);
            if (randomNum < static_cast<double>(num_a) / (2 * N)) {
                alleles[j] = 'a';
            } else {
                alleles[j] = 'A';
            }
        }

        num_a = 0;
        num_A = 0;
        for (int j = 0; j < 2 * N; j++) {
            if (alleles[j] == 'a') {
                num_a++;
            } else {
                num_A++;
            }
        }
    }

    double final_f = static_cast<double>(num_a) / (2 * N);
    std::cout << "Population " << p << ": " << final_f << std::endl;
}

int main() {
    int N, G, P;
    double f;

    while (true) {
        std::cout << "Enter Populations no larger than 4 (P): ";
        std::cin >> P;
        if (P <= 4) {
            break;
        }
        std::cout << "Please enter a number no larger than 4." << std::endl;
    }
    std::cout << "Enter Population Size (N): ";
    std::cin >> N;

    std::cout << "Enter Number of Generations (G): ";
    std::cin >> G;
    
    std::cout << "Enter Initial Allele Frequency (f): ";
    std::cin >> f;
    std::vector<pid_t> childPids;

    for (int i = 0; i < P; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            simulateGeneticDrift(N, G, f, i + 1);
            exit(0);
        } else if (pid > 0) {
            childPids.push_back(pid);
        } else {
            std::cerr << "Fork failed." << std::endl;
            return 1;
        }
    }
    for (auto pid : childPids) {
        int status;
        waitpid(pid, &status, 0);
    }

    return 0;
}
