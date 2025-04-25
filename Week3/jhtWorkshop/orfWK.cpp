/*
Workshop 3: Finding ORFs
Student: John Treon
Compilation instructions: g++ -o orfWK orfWK.cpp, ./orfWK
*/


#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <stack>

int main() {
    std::string filePath;
    std::cout << "Enter the file path: ";
    std::getline(std::cin, filePath);
    

    std::ifstream inputFile(filePath);
    if (!inputFile) {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }
    
    std::string line;
    std::array<char, 3> arr = {'X', 'X', 'X'};
    int nucleotide = 0;
    int startCodon = 0;
    int stopCodon = 0;
    bool inFrame = false;
    int frameCounter = 0;
    bool orfFound = false;
    std::stack<char> reverseStack;
    while (std::getline(inputFile, line)) {
        // read each character
        for (int i = 0; i < line.length(); i++) {
            if (line[i] != 'A' && line[i] != 'U' && line[i] != 'G' && line[i] != 'C') {
                break;
            }
            // shift the array
            arr[0] = arr[1];
            arr[1] = arr[2];
            arr[2] = line[i];
            reverseStack.push(line[i]);
            if (!inFrame) {
                // check for start codon
                if (arr[0] == 'A' && arr[1] == 'U' && arr[2] == 'G') {
                    //std::cout << "Found start codon at index " << nucleotide - 1 << std::endl;
                    arr = {'X', 'X', 'X'};
                    startCodon = nucleotide - 1;
                    inFrame = true;
                }
            }
            else {
                // check for stop codon
                if (frameCounter == 2) {
                    if ((arr[0] == 'U' && arr[1] == 'A' && arr[2] == 'A') || (arr[0] == 'U' && arr[1] == 'A' && arr[2] == 'G') || (arr[0] == 'U' && arr[1] == 'G' && arr[2] == 'A')) {
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
                std::cout << "ORF at " << startCodon << "-" << stopCodon << std::endl;
                orfFound = false;
            }
        }
    }
    inFrame = false;
    while (!reverseStack.empty()) {
        char c = reverseStack.top();
        while (c != 'A' && c != 'U' && c != 'G' && c != 'C') {
            reverseStack.pop();
            c = reverseStack.top();
        }
        // shift the array
        arr[0] = arr[1];
        arr[1] = arr[2];
        arr[2] = c;
        if (!inFrame) {
            // check for start codon
            if (arr[0] == 'A' && arr[1] == 'U' && arr[2] == 'G') {
                //std::cout << "Found start codon at index " << nucleotide - 1 << std::endl;
                arr = {'X', 'X', 'X'};
                startCodon = nucleotide + 2;
                inFrame = true;
            }
        }
        else {
            // check for stop codon
            if (frameCounter == 2) {
                if ((arr[0] == 'U' && arr[1] == 'A' && arr[2] == 'A') || (arr[0] == 'U' && arr[1] == 'A' && arr[2] == 'G') || (arr[0] == 'U' && arr[1] == 'G' && arr[2] == 'A')) {
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
            std::cout << "ORF at " << startCodon << "-" << stopCodon << std::endl;
            orfFound = false;
        }
        reverseStack.pop();
    }
    inputFile.close();

    return 0;
}
