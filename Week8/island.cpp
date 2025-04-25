#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <random>
#include <algorithm>

struct Coordinate {
    unsigned int x = 0;
    unsigned int y = 0;

    bool operator==(const Coordinate& a) const
    {
        return (x == a.x && y == a.y);
    }
};

struct Island {
    unsigned int rows = 0;
    unsigned int columns = 0;
    std::vector<std::vector<std::string>> map;
    std::vector<Coordinate> land_coords;
};

struct CustomReturn {
    unsigned int unsi = 0;
    bool error = false;
    Island island;
};

// Converts a string to an unsigned int
CustomReturn to_unsigned_int(std::string &s);
CustomReturn read_island_file(std::string &file_name, char delimiter);
void display_island(Island &island);
void simulate(Island &island, unsigned int num_species, double land_prob[], unsigned int land_counter[]);

int main() {
    bool run = true;
    Island island;
    // number of species
    unsigned int s;

    // Get island information
    if (run) {
        // path to file where "island" is stored
        std::string file_name;

        std::cout << "Please enter your island filepath: ";
        std::cin >> file_name;

        CustomReturn temp = read_island_file(file_name, ',');

        if (!temp.error) {
            island = temp.island;
        } else {
            run = false;
        }
    }

    // Get the number of species
    if (run) {
        std::string s_input;

        std::cout << "Please enter the number of species (N): ";
        std::cin >> s_input;

        CustomReturn temp;
        temp = to_unsigned_int(s_input);

        if (!temp.error) {
            s = temp.unsi;
        } else {
            run = false;
        }
    }
    
    // ----------------------------------------------------------------
    
    // Monte Carlo Model for Island Migration
    if (run) {
        display_island(island);

        // All probabilities being tested
        double land_prob[] = {
            0.05, 0.10, 0.15, 0.20, 0.25, 
            0.30, 0.35, 0.40, 0.45, 0.50, 
            0.55, 0.60, 0.65, 0.70, 0.75, 
            0.80, 0.85, 0.90, 0.95
        };

        unsigned int land_counter[19] = {0};

        simulate(island, s, land_prob, land_counter);

        std::cout << "[0.0, 0.1) " << land_counter[0] << std::endl;
        std::cout << "[0.1, 0.2) " << land_counter[1] + land_counter[2] << std::endl;
        std::cout << "[0.2, 0.3) " << land_counter[3] + land_counter[4] << std::endl;
        std::cout << "[0.3, 0.4) " << land_counter[5] + land_counter[6]  << std::endl;
        std::cout << "[0.4, 0.5) " << land_counter[7] + land_counter[8] << std::endl;
        std::cout << "[0.5, 0.6) " << land_counter[9] + land_counter[10] << std::endl;
        std::cout << "[0.6, 0.7) " << land_counter[11] + land_counter[12] << std::endl;
        std::cout << "[0.7, 0.8) " << land_counter[13] + land_counter[14] << std::endl;
        std::cout << "[0.8, 0.9) " << land_counter[15] + land_counter[16] << std::endl;
        std::cout << "[0.9, 1.0] " << land_counter[17] + land_counter[18] << std::endl;

    }
    
    return 0;
}

CustomReturn read_island_file(std::string &file_name, char delimiter) {
    CustomReturn n;

    Island new_island;
    unsigned int rows = 0;
    unsigned int columns = 0;
    unsigned int total_cells = 0;
    std::vector<std::vector<std::string>> map;
    std::vector<Coordinate> land_coords;

    // island file
    std::ifstream f;
    f.open(file_name);

    // see if file exists
    if (!f.good()) {
        std::cout << "ERROR: Could not read file '" << file_name << "'" << std::endl;
        n.error = true;
    } else {
        // holds a line of the island file
        std::string curr_row;

        // read file contents
        while (f.good()) {
            std::vector<std::string> new_row;
            std::string curr_element;
            getline(f, curr_row);

            unsigned int prev_element_index = 0;

            // Check to see if the line does not have only whitespaces
            if (curr_row.find_first_not_of("\t\n\v\f\r") != std::string::npos) {
                columns = 0;
                for (int c = 0; c < curr_row.length(); ++c) {
                    if (curr_row.at(c) == delimiter) {
                        curr_element = curr_row.substr(prev_element_index, c - prev_element_index);
                        prev_element_index = c + 1;
                        new_row.push_back(curr_element);

                        if (curr_element == "1") {
                            Coordinate curr_cord = {.x = rows, .y = columns};
                            land_coords.push_back(curr_cord);
                        }

                        ++total_cells;
                        ++columns;
                    }
                }

                // for last element of every row
                curr_element = curr_row.substr(prev_element_index, curr_row.length() - prev_element_index);
                new_row.push_back(curr_element);

                if (curr_element == "1") {
                    Coordinate curr_cord = {.x = rows, .y = columns};
                    land_coords.push_back(curr_cord);
                }

                ++total_cells;
                ++rows;
                ++columns;
            }

            map.push_back(new_row);
        }

        columns = total_cells / rows;

        new_island.rows = rows;
        new_island.columns = columns;
        new_island.map = map;
        new_island.land_coords = land_coords;
    }

    n.island = new_island;
    return n;
}

void display_island(Island &island) {
    for (auto r : island.map) {
        for (auto c : r) {
            if (c == "1") {
                // land
                std::cout << "\x1B[31m" << c << "\033[0m ";
            } else {
                // water
                std::cout << "\x1B[34m" << c << "\033[0m ";
            }
        }

        std::cout << std::endl;
    }
}

void simulate(Island &island, unsigned int num_species, double land_prob[], unsigned int land_counter[]) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 1.0);

    for (int prob = 0; prob < 19; ++prob) {
        for (int s = 0; s < num_species; ++s) {
            for (unsigned int i = 0; i < island.rows; ++i) {
                for (unsigned int j = 0; j < island.columns; ++j) {
                    if (island.map[i][j] == "1" && dis(gen) < land_prob[prob]) {
                        ++land_counter[prob];
                    }
                }
            }
        }
    }
}

CustomReturn to_unsigned_int(std::string &s) {
    CustomReturn n;
    try {
        // Convert string to int
        n.unsi = std::stoi(s);
    }
    catch (...)
    {
        std::cout << "ERROR: Could not convert user input to unsigned int" << std::endl;
        n.error = true;
    }

    return n;
}