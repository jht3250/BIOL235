/* 
Workshop: Genetic Drift with OpenMP
Student: John Treon
Compilation instructions: g++ -o geneticDriftOMPWK geneticDriftOMPWK.cpp, ./geneticDriftOMPWK
*/
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <array>
#include <omp.h>

int main() {
    int N, G, P;
    double f;

    // Take user input for Populations (P), Population Size (N), Number of Generations (G), and initial allele frequency (f)
    std::cout << "Enter Populations (P): ";
    std::cin >> P;

    std::cout << "Enter Population Size (N): ";
    std::cin >> N;

    std::cout << "Enter Number of Generations (G): ";
    std::cin >> G;

    std::cout << "Enter Initial Allele Frequency (f): ";
    std::cin >> f;
    std::array<int, 10> arr = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    #pragma omp parallel num_threads(P)
    {
        int num_a, num_A;
        std::vector<char> alleles(2 * N);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 1.0);  

        #pragma omp for
        for (int i = 0; i < P; i++) {
            num_a = f * (2 * N);
            num_A = (2 * N) - num_a;

            // Simulate genetic drift for each generation
            for (int j = 0; j < G; j++) {
                for (int k = 0; k < 2 * N; k++) {
                    // Generate a random number between 0 and 1
                    double randomNum = dis(gen);
                    // If the random number is less than the allele frequency, the allele is 'a'
                    if (randomNum < static_cast<double>(num_a) / (2 * N)) {
                        alleles[k] = 'a';
                    } else {
                        alleles[k] = 'A';
                    }
                }

                // Update the allele frequencies for the next generation
                num_a = 0;
                num_A = 0;
                for (int k = 0; k < 2 * N; k++) {
                    if (alleles[k] == 'a') {
                        num_a++;
                    } else {
                        num_A++;
                    }
                }
            }

            // Print the final allele frequencies
            double final_f = static_cast<double>(num_a) / (2 * N);
            #pragma omp critical
            {
                if (final_f >= 0 && final_f < 0.1) {
                    arr[0]++;
                } 
                else if (final_f >= 0.1 && final_f < 0.2) {
                    arr[1]++;
                } 
                else if (final_f >= 0.2 && final_f < 0.3) {
                    arr[2]++;
                } 
                else if (final_f >= 0.3 && final_f < 0.4) {
                    arr[3]++;
                } 
                else if (final_f >= 0.4 && final_f < 0.5) {
                    arr[4]++;
                } 
                else if (final_f >= 0.5 && final_f < 0.6) {
                    arr[5]++;
                } 
                else if (final_f >= 0.6 && final_f < 0.7) {
                    arr[6]++;
                } 
                else if (final_f >= 0.7 && final_f < 0.8) {
                    arr[7]++;
                } 
                else if (final_f >= 0.8 && final_f < 0.9) {
                    arr[8]++;
                } 
                else if (final_f >= 0.9 && final_f <= 1) {
                    arr[9]++;
                }
            }
        }
    }

    std::cout << "[0, 0.1)    " << arr[0] << std::endl;
    std::cout << "[0.1, 0.2)  " << arr[1] << std::endl;
    std::cout << "[0.2, 0.3)  " << arr[2] << std::endl;
    std::cout << "[0.3, 0.4)  " << arr[3] << std::endl;
    std::cout << "[0.4, 0.5)  " << arr[4] << std::endl;
    std::cout << "[0.5, 0.6)  " << arr[5] << std::endl;
    std::cout << "[0.6, 0.7)  " << arr[6] << std::endl;
    std::cout << "[0.7, 0.8)  " << arr[7] << std::endl;
    std::cout << "[0.8, 0.9)  " << arr[8] << std::endl;
    std::cout << "[0.9, 1]    " << arr[9] << std::endl;
    return 0;
}
