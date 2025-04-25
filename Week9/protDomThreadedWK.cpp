/*
Homework: Protein Domains Threaded
Student: John Treon
Compilation instructions: g++ -o -pthread protDomThreadedWK protDomThreadedWK.cpp, ./protDomThreadedWK
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <pthread.h>

struct Fragment {
    std::string name;
    std::string sequence;
} typedef Fragment;

clock_t beginTime, endTime;
double times[4] = {0.0, 0.0, 0.0, 0.0};

bool isValidAminoAcidSymbol(char symbol) {
    std::string validSymbols = "DERKHNQSTYAGVLIPFMWC";
    return (validSymbols.find(symbol) != std::string::npos);
}

void* searchDomain(void* arg) {
    std::pair<std::string, Fragment>* data = static_cast<std::pair<std::string, Fragment>*>(arg);
    std::string domain = data->first;
    Fragment fragment = data->second;
    
    std::string sequence = fragment.sequence;
    int startPos = sequence.find(domain);
    int pos = startPos;
    if (startPos == std::string::npos) {
        pthread_exit(NULL);
    }
    else {
        while (pos != std::string::npos) {
            int endPos = startPos + domain.length() - 1;
            int strPos = pos + domain.length() - 1;
            //std::cout << domain << " " << fragment.name << " " << pos + 1 << std::endl;
            pos = sequence.find(domain, strPos + 1);
            startPos = startPos + pos;
        }
    }
    pthread_exit(NULL);
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
            while (pos != std::string::npos) {
                int endPos = startPos + domain.length() - 1;
                int strPos = pos + domain.length() - 1;
                std::cout << domain << "\t" << result[i].name << "\t" << pos + 1 << std::endl;
                pos = sequence.find(domain, strPos + 1);
                startPos = startPos + pos;
            }
        }
    }
    endTime = clock();
    times[2] = double(endTime - beginTime) / CLOCKS_PER_SEC;

    // Search for the domain in each sequence using pthreads
    beginTime = clock();
    pthread_t threads[result.size()];
    // Create a vector to store the thread data
    std::vector<std::pair<std::string, Fragment>> threadData;
    for (int i = 0; i < result.size(); i++) {
        threadData.push_back(std::make_pair(domain, result[i]));
    }
    // Create threads and pass the thread data
    int threadCount = 0;
    for (int i = 0; i < result.size(); i++) {
        if (threadCount == 4) {
            for (int j = 0; j < threadCount; j++) {
                pthread_join(threads[j], NULL);
            }
            threadCount = 0;
        }
        pthread_create(&threads[i], NULL, searchDomain, &threadData[i]);
        threadCount++;
    }
    // Wait for all threads to finish
    endTime = clock();
    times[3] = double(endTime - beginTime) / CLOCKS_PER_SEC;
    std::cout << "Time spent reading file: " << times[0] << std::endl;
    std::cout << "Time spent validating domain: " << times[1] << std::endl;
    std::cout << "Time spent searching for domain: " << times[2] << std::endl;
    std::cout << "Time spent searching for domain with threads: " << times[3] << std::endl;
    std::cout << "Speedup: " << (times[2] / times[3]) * 100 << "%" << std::endl;
    return 0;
}