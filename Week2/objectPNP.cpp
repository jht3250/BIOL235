#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <random>

int eaten = 0;

class Matrix {
public:
    Matrix(int size) : size(size) {
        grid.resize(size, std::vector<char>(size, '-'));
    }

    void display() {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                std::cout << grid[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    void placePredator(int x, int y) {
        grid[x][y] = 'X';
    }

    void placePrey(int x, int y) {
        grid[x][y] = 'O';
    }

    void movePredator(int x, int y, int newX, int newY) {
        grid[x][y] = '-';
        grid[newX][newY] = 'X';
    }

    void movePrey(int x, int y, int newX, int newY) {
        grid[x][y] = '-';
        grid[newX][newY] = 'O';
    }

    int getSize() const {
        return size;
    }

    char getGridValue(int x, int y) const {
        return grid[x][y];
    }

private:
    int size;
    std::vector<std::vector<char>> grid;
};



class Prey {
public:
    Prey(int x, int y) : x(x), y(y) {
        alive = true;
    }

    void moveRandomly(Matrix& matrix) {
        if (!this->isAlive()) {
            // do nothing
        }
        else {
            int size = matrix.getSize();
            int newX, newY;

            // Generate random new positions until a valid position is found
            do {
                newX = x + (rand() % 3) - 1;
                newY = y + (rand() % 3) - 1;
            } while (newX < 0 || newX >= size || newY < 0 || newY >= size || matrix.getGridValue(newX, newY) != '-');

            if (this->isAlive()) {
                matrix.movePrey(x, y, newX, newY);
                x = newX;
                y = newY;
            }
        }
    }

    int getX() const {
        return x;
    }

    int getY() const {
        return y;
    }

    bool isAlive() const {
        return alive;
    }

    void kill () {
        alive = false;
    }

private:
    int x, y;
    bool alive;
};

std::vector<Prey> preyList;

class Predator {
public:
    Predator(int x, int y) : x(x), y(y) {}

    void searchAndEatPrey(Matrix& matrix) {
        int size = matrix.getSize();
        bool preyFound = false;

        // Search for prey in the neighboring cells
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                int newX = x + i;
                int newY = y + j;

                // Check if the new position is within the matrix boundaries and contains prey
                if (newX >= 0 && newX < size && newY >= 0 && newY < size && matrix.getGridValue(newX, newY) == 'O') {
                    // loop through prey list and kill the prey
                    for (Prey& prey : preyList) {
                        if (prey.getX() == newX && prey.getY() == newY) {
                            prey.kill();
                            eaten++;
                        }
                    }
                    matrix.movePredator(x, y, newX, newY);
                    x = newX;
                    y = newY;
                    preyFound = true;
                    break;
                }
            }
            if (preyFound) {
                break;
            }
        }

        // If no prey is found, move randomly
        if (!preyFound) {
            moveRandomly(matrix);
        }
    }

    void moveRandomly(Matrix& matrix) {
        int size = matrix.getSize();
        int newX, newY;

        // Generate random new positions until a valid position is found
        do {
            newX = x + (rand() % 3) - 1;
            newY = y + (rand() % 3) - 1;
        } while (newX < 0 || newX >= size || newY < 0 || newY >= size || matrix.getGridValue(newX, newY) != '-');

        matrix.movePredator(x, y, newX, newY);
        x = newX;
        y = newY;
    }

    int getX() const {
        return x;
    }

    int getY() const {
        return y;
    }

private:
    int x, y;
};


int main() {
    srand(time(0));

    Matrix matrix(10);
    
    
    // Place prey randomly on the matrix
    for (int i = 0; i < 9; i++) {
        int preyX = rand() % matrix.getSize();
        int preyY = rand() % matrix.getSize();
        while (matrix.getGridValue(preyX, preyY) == 'O') {
            preyX = rand() % matrix.getSize();
            preyY = rand() % matrix.getSize();
        }
        Prey prey(preyX, preyY);
        preyList.push_back(prey);
        matrix.placePrey(prey.getX(), prey.getY());
    }
    // Place predator randomly on matrix in open space
    int predX = rand() % matrix.getSize();
    int predY = rand() % matrix.getSize();
    while (matrix.getGridValue(predX, predY) == 'O') {
        predX = rand() % matrix.getSize();
        predY = rand() % matrix.getSize();
    }
    Predator predator(predX, predY);
    matrix.placePredator(predator.getX(), predator.getY());

    // Run simulation for 100 rounds
    for (int round = 1; round <= 100; round++) {
        std::cout << "Round " << round << std::endl;
        matrix.display();
        std::cout << "Prey eaten: " << eaten << std::endl;
        std::cout << "Press enter to move to the next round.";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        predator.searchAndEatPrey(matrix);
        for (Prey& prey : preyList) {
            if (prey.isAlive()) {
                prey.moveRandomly(matrix);
            }
        }
    }

    return 0;
}
