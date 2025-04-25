/* 
Homework: Genetic Drift with OpenMP and Fst calculation
Student: John Treon
Compilation instructions: g++ -o geneticDriftOMPHW geneticDriftOMPHW.cpp, ./geneticDriftOMPHW
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

    std::vector<double> alleleFrequencies(P);
    std::vector<double> heterozygosity(P);
    double totalAlleles = 0.0;
    double totalHeterozygosity = 0.0;
    double totalHet = 0.0;

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

            // Calculate allele frequency and heterozygosity
            double alleleFrequency = static_cast<double>(num_a) / (2 * N);
            double hetero = 2 * alleleFrequency * (1 - alleleFrequency);
            alleleFrequencies[i] = alleleFrequency;
            heterozygosity[i] = hetero;

            // Calculate total heterozygosity
            #pragma omp atomic
            totalHet += hetero;
            totalAlleles += alleleFrequency * N;
        }
    }

    // Calculate average heterozygosity
    double averageHeterozygosity = totalHet / P;

    // Calculate total heterozygosity
    totalHeterozygosity = totalAlleles / (N * P);

    // Calculate Fst
    double Fst = (totalHeterozygosity - averageHeterozygosity) / totalHeterozygosity;

    // Output allele frequency and heterozygosity for each population
    for (int i = 0; i < P; i++) {
        std::cout << "Population " << i + 1 << ":\t" << alleleFrequencies[i] << "\t" << heterozygosity[i] << std::endl;
    }

    // Output average heterozygosity, total heterozygosity, and Fst
    std::cout << "Average Heterozygosity:\t" << averageHeterozygosity << std::endl;
    std::cout << "Total Heterozygosity:\t" << totalHeterozygosity << std::endl;
    std::cout << "Fst:\t" << Fst << std::endl;
    
    return 0;
}
