/*
Homework: Protein Domains (optimized)
Student: John Treon
Compilation instructions: g++ -o protDomHW protDomHW.cpp, ./protDomHW
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>

struct Fragment {
    std::string name;
    std::string sequence;
} typedef Fragment;

clock_t beginTime, endTime;
double times[3] = {0.0, 0.0, 0.0};

bool isValidAminoAcidSymbol(char symbol) {
    // Add your validation logic here
    // Return true if the symbol is a valid amino acid symbol, otherwise false
    // Example validation logic:
    std::string validSymbols = "DERKHNQSTYAGVLIPFMWC";
    return (validSymbols.find(symbol) != std::string::npos);
}

int main() {
    beginTime = clock();
    std::string filePath;
    std::cout << "Enter the file path: ";
    std::getline(std::cin, filePath);

    std::ifstream inputFile(filePath);
    if (!inputFile) {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }

    std::string line;
    std::vector<Fragment> result;
    std::vector<std::string> sequences;
    while (std::getline(inputFile, line)) {
        if (line[0] == '>') {
            Fragment frag;
            frag.name = line;
            result.push_back(frag);
            sequences.push_back("");
        }
        else {
            result.back().sequence.append(line);
            sequences.back().append(line);
        }
    }
    inputFile.close();
    endTime = clock();
    times[0] = double(endTime - beginTime) / CLOCKS_PER_SEC;
    std::string domain;
    std::cout << "Enter the protein domain sequence: ";
    std::getline(std::cin, domain);

    // Validate the domain sequence
    beginTime = clock();
    for (char symbol : domain) {
        if (!isValidAminoAcidSymbol(symbol)) {
            std::cerr << "Invalid amino acid symbol in the domain sequence." << std::endl;
            return 1;
        }
    }
    endTime = clock();
    times[1] = double(endTime - beginTime) / CLOCKS_PER_SEC;
    // Search for the domain in each sequence
    // Note: I AM SO PROUD OF THIS AND HOW IT GOES UNTIL IT FINDS ALL OF THE DOMAINS ON EACH SEQUENCE
    beginTime = clock();
    for (int i = 0; i < sequences.size(); i++) {
        std::string sequence = sequences[i];
        int startPos = sequence.find(domain);
        int pos = startPos;
        if (startPos == std::string::npos) {
            continue;
        }
        else {
            std::cout << "Sequence " << i + 1 << std::endl;
            while (pos != std::string::npos) {
                int endPos = startPos + domain.length() - 1;
                int strPos = pos + domain.length() - 1;
                std::cout << "\t" << pos + 1  << " " << strPos + 1 << std::endl;
                pos = sequence.find(domain, strPos + 1);
                startPos = startPos + pos;
            }
        }
    }
    endTime = clock();
    times[2] = double(endTime - beginTime) / CLOCKS_PER_SEC;
    std::cout << "Time spent reading file: " << times[0] << std::endl;
    std::cout << "Time spent validating domain: " << times[1] << std::endl;
    std::cout << "Time spent searching for domain: " << times[2] << std::endl;
    std::cout << "Total time spent: " << times[0] + times[1] + times[2] << std::endl;
    return 0;
}
