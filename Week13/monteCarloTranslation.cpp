/*
Workshop: Monte Carlo Simulation
Student: John Treon
compile: g++ -fopenmp monteCarloTranslation.cpp -o monteCarloTranslation
*/

#include <vector>
#include <random>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <omp.h>

struct Sequence {
    std::string header = "";
    std::string seq = "";
} typedef Sequence;

struct PositionData {
    unsigned int a = 0; 
    unsigned int r = 0; 
    unsigned int n = 0; 
    unsigned int d = 0; 
    unsigned int c = 0; 
    unsigned int e = 0; 
    unsigned int q = 0; 
    unsigned int g = 0; 
    unsigned int h = 0; 
    unsigned int i = 0; 
    unsigned int l = 0; 
    unsigned int k = 0; 
    unsigned int m = 0; 
    unsigned int f = 0; 
    unsigned int p = 0; 
    unsigned int s = 0; 
    unsigned int t = 0; 
    unsigned int w = 0; 
    unsigned int y = 0; 
    unsigned int v = 0; 
    unsigned int mutations = 0;
} typedef PositionData;

struct SimOut {
    std::vector<std::vector<Sequence>> lineages;
    std::vector<PositionData> positionData;
    
    unsigned int totalMutations = 0;
} typedef SimOut;

class Sim {
    private:
        Sequence s;     
        unsigned int N; 
        unsigned int G;
        double R;      
    public:
        Sim(Sequence s, unsigned int N, unsigned int G, double R);
        SimOut start();
        void saveSim(SimOut*);
};

Sim::Sim(Sequence s, unsigned int N, unsigned int G, double R) {
    this->s = s;
    this->N = N;
    this->G = G;
    this->R = R;
}

// Simulate virus lineages over generations with a mutation rate
SimOut Sim::start() { 
    SimOut report;
    std::vector<std::vector<Sequence>> lineages;
    std::vector<PositionData> positionData;

    unsigned int seqLength = (*this).s.seq.length();

    // Set inital values for each lineage
    for (unsigned int lineageIdx = 0; lineageIdx < (*this).N; lineageIdx++) {
        Sequence newSeq;

        std::stringstream newHeader;
        newHeader << (*this).s.header;
        newHeader << " (lineage ";
        newHeader << lineageIdx + 1;
        newHeader << ", gen 0)";

        newSeq.header = newHeader.str();
        newSeq.seq = (*this).s.seq;
        std::vector<Sequence> currLineage;
        currLineage.push_back(newSeq);
        lineages.push_back(currLineage);
    }

    for (unsigned int posIdx = 0; posIdx < (*this).s.seq.length(); posIdx++) {
        PositionData data;
        positionData.push_back(data);
    }

    std::random_device rd;
    std::mt19937 mt(rd());

    // Each generation
    for (unsigned int currGen = 0; currGen < (*this).G; currGen++) {
        // For each lineage
        #pragma omp parallel for
        for (unsigned int currLineage = 0; currLineage < (*this).N; currLineage++) {
            lineages.at(currLineage).push_back(lineages.at(currLineage).at(currGen));

            std::stringstream newHeader;
            newHeader << (*this).s.header;
            newHeader << " (lineage ";
            newHeader << currLineage + 1;
            newHeader << ", gen ";
            newHeader << currGen + 1;
            newHeader << ")";
            
            lineages.at(currLineage).at(currGen+1).header = newHeader.str();

            // For each nucleotide, determine if that nucleotide mutates
            for (unsigned int charIdx = 0; charIdx < seqLength; charIdx++) {
                // Chance current char mutates
                double currentMutationChance = std::generate_canonical<double, 10>(mt);
                // If current char mutates, pick a random alternative nucleotide
                if ((*this).R >= currentMutationChance) {
                    char randChar = ' ';
                    do {
                        switch (mt() % 20)
                        {
                            case 0:
                                randChar = 'A';
                                break;
                            case 1:
                                randChar = 'R';
                                break;
                            case 2:
                                randChar = 'N';
                                break;
                            case 3:
                                randChar = 'D';
                                break;
                            case 4:
                                randChar = 'C';
                                break;
                            case 5:
                                randChar = 'E';
                                break;
                            case 6:
                                randChar = 'Q';
                                break;
                            case 7:
                                randChar = 'G';
                                break;
                            case 8:
                                randChar = 'H';
                                break;
                            case 9:
                                randChar = 'I';
                                break;
                            case 10:
                                randChar = 'L';
                                break;
                            case 11:
                                randChar = 'K';
                                break;
                            case 12:
                                randChar = 'M';
                                break;
                            case 13:
                                randChar = 'F';
                                break;
                            case 14:
                                randChar = 'P';
                                break;
                            case 15:
                                randChar = 'S';
                                break;
                            case 16:
                                randChar = 'T';
                                break;
                            case 17:
                                randChar = 'W';
                                break;
                            case 18:
                                randChar = 'Y';
                                break;
                            case 19:
                                randChar = 'V';
                                break;
                            default:
                                randChar = ' ';
                                break;
                        }
                    } while (randChar == lineages.at(currLineage).at(currGen+1).seq.at(charIdx));

                    lineages.at(currLineage).at(currGen+1).seq.at(charIdx) = randChar;
                }
            }
        }
    }

    // Check to see if each nucelotide in the final generation of each lineage is different than the inital seq
    #pragma omp parallel for
    for (unsigned int currLineage = 0; currLineage < (*this).N; currLineage++) {
        for (unsigned int charIdx = 0; charIdx < seqLength; charIdx++) {
            char currChar = lineages.at(currLineage).at(lineages.at(currLineage).size()-1).seq.at(charIdx);
            char initialChar = lineages.at(currLineage).at(0).seq.at(charIdx);
            switch (currChar) {
                case 'A':
                    #pragma omp atomic
                    positionData.at(charIdx).a++;
                    break;
                case 'R':
                    #pragma omp atomic
                    positionData.at(charIdx).r++;
                    break;
                case 'N':
                    #pragma omp atomic
                    positionData.at(charIdx).n++;
                    break;
                case 'D':
                    #pragma omp atomic
                    positionData.at(charIdx).d++;
                    break;
                case 'C':
                    #pragma omp atomic
                    positionData.at(charIdx).c++;
                    break;
                case 'E':
                    #pragma omp atomic
                    positionData.at(charIdx).e++;
                    break;
                case 'Q':
                    #pragma omp atomic
                    positionData.at(charIdx).q++;
                    break;
                case 'G':
                    #pragma omp atomic
                    positionData.at(charIdx).g++;
                    break;
                case 'H':
                    #pragma omp atomic
                    positionData.at(charIdx).h++;
                    break;
                case 'I':
                    #pragma omp atomic
                    positionData.at(charIdx).i++;
                    break;
                case 'L':
                    #pragma omp atomic
                    positionData.at(charIdx).l++;
                    break;
                case 'K':
                    #pragma omp atomic
                    positionData.at(charIdx).k++;
                    break;
                case 'M':
                    #pragma omp atomic
                    positionData.at(charIdx).m++;
                    break;
                case 'F':
                    #pragma omp atomic
                    positionData.at(charIdx).f++;
                    break;
                case 'P':
                    #pragma omp atomic
                    positionData.at(charIdx).p++;
                    break;
                case 'S':
                    #pragma omp atomic
                    positionData.at(charIdx).s++;
                    break;
                case 'T':
                    #pragma omp atomic
                    positionData.at(charIdx).t++;
                    break;
                case 'W':
                    #pragma omp atomic
                    positionData.at(charIdx).w++;
                    break;
                case 'Y':
                    #pragma omp atomic
                    positionData.at(charIdx).y++;
                    break;
                case 'V':
                    #pragma omp atomic
                    positionData.at(charIdx).v++;
                    break;
                default:
                    break;
            }

            if (currChar != initialChar) {
                #pragma omp atomic
                positionData.at(charIdx).mutations++;
                #pragma omp atomic
                report.totalMutations++;
            }
        }
    }

    report.lineages = lineages;
    report.positionData = positionData;

    return report;
}

// Save Sim data to a file
void Sim::saveSim(SimOut *sr) {
    std::ofstream outfile;
    outfile.open("output.txt");
    if (outfile.is_open())
    {
        outfile << std::fixed << std::setprecision(2);
        outfile << "\tA\tR\tN\tD\tC\tE\tQ\tG\tH\tI\tL\tK\tM\tF\tP\tS\tT\tW\tY\tV\tMutations\n";

        unsigned int currPos = 1;
        for (auto p : sr->positionData) {
            outfile << '[' << currPos << ']' << "\t";
            outfile << p.a << "\t";
            outfile << p.r << "\t";
            outfile << p.n << "\t";
            outfile << p.d << "\t";
            outfile << p.c << "\t";
            outfile << p.e << "\t";
            outfile << p.q << "\t";
            outfile << p.g << "\t";
            outfile << p.h << "\t";
            outfile << p.i << "\t";
            outfile << p.l << "\t";
            outfile << p.k << "\t";
            outfile << p.m << "\t";
            outfile << p.f << "\t";
            outfile << p.p << "\t";
            outfile << p.s << "\t";
            outfile << p.t << "\t";
            outfile << p.w << "\t";
            outfile << p.y << "\t";
            outfile << p.v << "\t";
            outfile << p.mutations << "\n";

            currPos++;
        }

        outfile << "\nR' = " << sr->totalMutations * 1.0 / (*this).s.seq.length() << "\n";

        outfile.close();
    } else {
        std::cerr << "Unable to open output file";
        exit(1);
    }
}

// Converts a string to an int
unsigned int toInt(std::string &s);
// Converts a string to a double
double toDouble(std::string &s);

int main() {
    // read in file from user
    std::string fileName;

    std::cout << "File name?: ";
    std::cin >> fileName;

    std::ifstream file(fileName);
    std::string line;

    // Checks to see if file can be opened
    if (file.fail()) {
        std::cerr << "Cannot open file '" << fileName << "'\n";
        exit(1);
    }

    Sequence sequence;

    // Read file line by line
    while (std::getline(file, line)) {
        if (line.at(0) == '>') {
            sequence.header = line.substr(1, line.size()-1);
        } else {
            sequence.seq += line;
        }
    }

    file.close();

    unsigned int N;
    std::string NInput;

    std::cout << "Please enter lineages (N): ";
    std::cin >> NInput;
    N = std::stoi(NInput);

    unsigned int G;
    std::string GInput;

    std::cout << "Please enter generations (G): ";
    std::cin >> GInput;
    G = std::stoi(GInput);

    double R;
    std::string RInput;

    std::cout << "Please enter mutation rate (R) (%): ";
    std::cin >> RInput;
    R = std::stod(RInput) / 100.0;

    if (R > 1.0) {
        std::cerr << "Mutation rate cannot be greater than 100%\n";
        exit(1);
    }

    std::cout << std::endl;

    Sim mySim = Sim(sequence, N, G, R);
    SimOut report = mySim.start();
    mySim.saveSim(&report);

    std::cout << "Check 'output.txt'" << std::endl;

    return 0;
}