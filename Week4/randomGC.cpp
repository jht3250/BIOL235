/*
Workshop 4: Sequence Modeling
Student: John Treon
Compilation instructions: g++ -o randomGC randomGC.cpp, ./randomGC
I am so proud of this one
*/

#include <iostream>
#include <string>
#include <random>
#include <fstream>
#include <string>
#include <array>
#include <stack>

int main() {
    // Get user input for sequence length
    int sequenceLength;
    std::cout << "Enter sequence length: ";
    std::cin >> sequenceLength;

    // Get user input for GC content
    double gcContent;
    std::cout << "Enter GC content (between 0 and 1) Ex: 0.4 = 40%: ";
    std::cin >> gcContent;

    // seed random number generator
    std::string sequence;
    std::random_device rd; 
    std::mt19937 gen(rd()); 
    std::uniform_real_distribution<> dis(0.0, 1.0);  
    int gcCount = 0;
    // Generate random nucleotides
    for (int i = 0; i < sequenceLength; i++) {
        double randomNum = dis(gen);
        char nucleotide;

        if (randomNum < gcContent) {
            nucleotide = (randomNum < gcContent / 2) ? 'G' : 'C';
            gcCount++;
        } else {
            nucleotide = (randomNum < (1 - gcContent) / 2 + gcContent) ? 'A' : 'T';
        }
        sequence += nucleotide;
    }

    // Change random ATs to GCs to balance GC content, loop until GC content is reached
    while (static_cast<double>(gcCount) / sequenceLength != gcContent) {
        int randomIndex = dis(gen) * sequenceLength;
        if (static_cast<double>(gcCount) / sequenceLength < gcContent) {
            if (sequence[randomIndex] == 'A' || sequence[randomIndex] == 'T') {
            sequence[randomIndex] = (sequence[randomIndex] == 'A') ? 'G' : 'C';
            gcCount++;
        }
        }
        else if (sequence[randomIndex] == 'G' || sequence[randomIndex] == 'C') {
            sequence[randomIndex] = (sequence[randomIndex] == 'G') ? 'A' : 'T';
            gcCount--;
        }
    }

    // Output the generated sequence and GC content
    std::ofstream file;
    file.open("sequence.txt");
    file << "Generated sequence: " << sequence << std::endl;
    file << "GC content: " << static_cast<double>(gcCount) / sequenceLength << std::endl;
    file <<  std::endl;
    std::cout << "Generated sequence: " << sequence << std::endl;
    std::cout << "GC content: " << static_cast<double>(gcCount) / sequenceLength << std::endl;
    
    // Find ORFs (modified from orfWK.cpp)
    std::string line = sequence;
    std::array<char, 3> arr = {'X', 'X', 'X'};
    int nucleotide = 0;
    int startCodon = 0;
    int stopCodon = 0;
    bool inFrame = false;
    int frameCounter = 0;
    bool orfFound = false;
    std::stack<char> reverseStack;
    // read each character
    for (int i = 0; i < line.length(); i++) {
        if (line[i] != 'A' && line[i] != 'T' && line[i] != 'G' && line[i] != 'C') {
            break;
        }
        // shift the array
        arr[0] = arr[1];
        arr[1] = arr[2];
        arr[2] = line[i];
        reverseStack.push(line[i]);
        if (!inFrame) {
            // check for start codon
            if (arr[0] == 'A' && arr[1] == 'T' && arr[2] == 'G') {
                //std::cout << "Found start codon at index " << nucleotide - 1 << std::endl;
                arr = {'X', 'X', 'X'};
                startCodon = nucleotide - 1;
                inFrame = true;
            }
        }
        else {
            // check for stop codon
            if (frameCounter == 2) {
                if ((arr[0] == 'T' && arr[1] == 'A' && arr[2] == 'A') || (arr[0] == 'T' && arr[1] == 'A' && arr[2] == 'G') || (arr[0] == 'T' && arr[1] == 'G' && arr[2] == 'A')) {
                    //std::cout << "Found stop codon at index " << nucleotide + 1 << std::endl;
                    stopCodon = nucleotide + 1;
                    orfFound = true;
                    frameCounter = 0;
                    inFrame = false;
                    arr = {'X', 'X', 'X'};
                }
                frameCounter = 0;
            }
            else {
                frameCounter++;
            }
        }
        nucleotide++;
        if (orfFound) {
            file << "ORF at " << startCodon << "-" << stopCodon << std::endl;
            orfFound = false;
        }
    }
    inFrame = false;
    while (!reverseStack.empty()) {
        char c = reverseStack.top();
        while (c != 'A' && c != 'T' && c != 'G' && c != 'C') {
            reverseStack.pop();
            c = reverseStack.top();
        }
        // shift the array
        arr[0] = arr[1];
        arr[1] = arr[2];
        arr[2] = c;
        if (!inFrame) {
            // check for start codon
            if (arr[0] == 'A' && arr[1] == 'T' && arr[2] == 'G') {
                //std::cout << "Found start codon at index " << nucleotide - 1 << std::endl;
                arr = {'X', 'X', 'X'};
                startCodon = nucleotide + 2;
                inFrame = true;
            }
        }
        else {
            // check for stop codon
            if (frameCounter == 2) {
                if ((arr[0] == 'T' && arr[1] == 'A' && arr[2] == 'A') || (arr[0] == 'T' && arr[1] == 'A' && arr[2] == 'G') || (arr[0] == 'T' && arr[1] == 'G' && arr[2] == 'A')) {
                    //std::cout << "Found stop codon at index " << nucleotide + 1 << std::endl;
                    stopCodon = nucleotide;
                    orfFound = true;
                    frameCounter = 0;
                    inFrame = false;
                }
                frameCounter = 0;
            }
            else {
                frameCounter++;
            }
        }
        nucleotide--;
        if (orfFound) {
            file << "ORF at " << startCodon << "-" << stopCodon << std::endl;
            orfFound = false;
        }
        reverseStack.pop();
    }
    
    file.close();

    return 0;
}
