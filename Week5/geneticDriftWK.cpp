/*
Workshop 5: Genetic Drift Simulation
Student: John Treon
Compilation instructions: g++ -o geneticDriftWK geneticDriftWK.cpp, ./geneticDriftWK
*/

#include <iostream>
#include <random>
#include <vector>
#include <algorithm>

int main() {
    int N, G;
    double f;

    // Take user input for Population Size (N), Number of Generations (G), and initial allele frequency (f)
    std::cout << "Enter Population Size (N): ";
    std::cin >> N;

    std::cout << "Enter Number of Generations (G): ";
    std::cin >> G;

    std::cout << "Enter Initial Allele Frequency (f): ";
    std::cin >> f;

    // Calculate the number of alleles of each type
    int num_a = f * (2 * N);
    int num_A = (2 * N) - num_a;

    // Seed the random number generator
    std::vector<char> alleles(2 * N);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);  

    // Simulate genetic drift for each generation
    for (int i = 0; i < G; i++) {
        for (int j = 0; j < 2 * N; j++) {
            // Generate a random number between 0 and 1
            double randomNum = dis(gen);
            // If the random number is less than the allele frequency, the allele is 'a'
            if (randomNum < static_cast<double>(num_a) / (2 * N)) {
                alleles[j] = 'a';
            } else {
                alleles[j] = 'A';
            }
        }

        // Update the allele frequencies for the next generation
        num_a = 0;
        num_A = 0;
        for (int j = 0; j < 2 * N; j++) {
            if (alleles[j] == 'a') {
                num_a++;
            } else {
                num_A++;
            }
        }
        // Print the allele frequencies for the current generation
        double f = static_cast<double>(num_a) / (2 * N);
        std::cout << "Generation " << i + 1 << " Allele Frequency: " << f << std::endl;
    }

    // Print the final allele frequencies
    double final_f = static_cast<double>(num_a) / (2 * N);
    std::cout << "Final Allele Frequency: " << final_f << std::endl;

    return 0;
}
