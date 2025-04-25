/*
Homework: FastQ cleanup
Student: John Treon
Compilation instructions: g++ -o fastQ fastQ.cpp, ./fastQ
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

struct Fragment {
    std::string id;
    std::string sequence;
    std::string plus;
    std::string quality;
} typedef Fragment;

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
    std::vector<Fragment> result;
    std::vector<std::string> sequences;
    while (std::getline(inputFile, line)) {
        // add each line to a new Fragment
        Fragment frag;
        frag.id = line;
        std::getline(inputFile, line);
        frag.sequence = line;
        sequences.push_back(line);
        std::getline(inputFile, line);
        frag.plus = line;
        std::getline(inputFile, line);
        frag.quality = line;
        result.push_back(frag);
    }
    inputFile.close();
    // search through each results if there are any duplicate sequences
    for (int i = 0; i < result.size(); i++) {
        for (int j = i + 1; j < result.size(); j++) {
            if (result[i].sequence != "XXXXXXX") {
                if (result[i].sequence == result[j].sequence) {
                    result[j].sequence = "XXXXXXX";
                    sequences[j] = "XXXXXXX";
                }
            }
        }
    }
    // print out the results that are not duplicates
    for (int i = 0; i < result.size(); i++) {
        if (result[i].sequence != "XXXXXXX") {
            std::cout << result[i].id << std::endl;
            std::cout << result[i].sequence << std::endl;
            std::cout << result[i].plus << std::endl;
            std::cout << result[i].quality << std::endl;
        }
    }

    return 0;
}
