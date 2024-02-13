// Implementation file for the GameBoard class.
// Contains the implementations of member functions defined in the GameBoard class declaration in "gameboard.hh".
//Name: Markus Tuovinen
//Student number: 151276149
//UserID: csmatu
//E-Mail: markus.tuovinen@tunif.i

#include "gameboard.hh"
#include <iostream>
#include <random>
#include <algorithm>
#include <chrono>
#include <random>

GameBoard::GameBoard()
{
    // Initialize the gameboard with EMPTY elements
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            gameboard[i][j] = EMPTY;
        }
    }
}

//Prints the current state of the game board.
void GameBoard::print() const
{
    // Printing upper border
    print_line(5 + 1 + 2 * SIZE + 1, '=');

    // Printing title row
    std::cout << "|   | ";
    for (unsigned int i = 0; i < SIZE; ++i)
    {
        std::cout << i + 1 << " ";
    }
    std::cout << "|" << std::endl;

    // Printing line after the title row
    print_line(5 + 1 + 2 * SIZE + 1, '-');

    // Printing the actual content of the gameboard
    for (unsigned int i = 0; i < SIZE; ++i)
    {
        std::cout << "| " << i + 1 << " | ";
        for (unsigned int j = 0; j < SIZE; ++j)
        {
            // Print the gameboard element (i, j)
            switch (gameboard[i][j])
            {
                case ZERO:
                    std::cout << "0 ";
                    break;
                case ONE:
                    std::cout << "1 ";
                    break;
                case EMPTY:
                    std::cout << "  ";
                    break;
            }
        }
        std::cout << "|" << std::endl;
    }

    // Printing lower border
    print_line(5 + 1 + 2 * SIZE + 1, '=');
}

//Helper function to print a line with a specified length and fill character.
void GameBoard::print_line(unsigned int length, char fill_character) const
{
    for (unsigned int i = 0; i < length; ++i)
    {
        std::cout << fill_character;
    }
    std::cout << std::endl;
}

//Fills the game board randomly based on the given seed value.
void GameBoard::fill_randomly(int seed)
{
    // Check the seed isint bad
    if (std::find(BAD_SEEDS.begin(), BAD_SEEDS.end(), seed) != BAD_SEEDS.end()) {
        std::cerr << "Bad seed" << std::endl;
        return;
    }

    // use seed for random generation
    std::default_random_engine generator(seed);
    // Alusta tasaisesti jakautunut kokonaislukujen jakauma välillä [0, DISTR_UPPER_BOUND]
    std::uniform_int_distribution<int> distribution(0, DISTR_UPPER_BOUND);

    // Täytä pelilauta satunnaisilla arvoilla
    for (unsigned int i = 0; i < SIZE; ++i) {
        for (unsigned int j = 0; j < SIZE; ++j) {
            int random_value = distribution(generator);
            if (random_value < 2) {
                // esimerkiksi alustamaan pelilautaa Element_type-luokan jäseniin liittyen
                gameboard[i][j] = static_cast<Element_type>(random_value);
            } else {
                // Voi täyttää pelilaudan tyhjillä tarvittaessa
                gameboard[i][j] = EMPTY;
            }
        }
    }
}

//Fills the game board based on the provided input string.
bool GameBoard::fill_with_input(const std::string& input)
{
    // Check if the input size is correct
    if (input.size() != SIZE * SIZE + 2) { // Adding 2 for the additional characters at the start and end
        std::cerr << "Wrong size of input" << std::endl;
        return false;
    }

    // Fill the gameboard based on the provided input
    for (unsigned int i = 0; i < SIZE; ++i) {
        for (unsigned int j = 0; j < SIZE; ++j) {
            char symbol = input[i * SIZE + j + 1]; // Starting from index 1 to skip the additional character at the start
            if (symbol != '0' && symbol != '1' && symbol != ' ') { // Allowing space as well
                std::cerr << "Wrong character" << std::endl;
                return false;
            }
            if (symbol == '0') {
                gameboard[i][j] = ZERO;
            } else if (symbol == '1') {
                gameboard[i][j] = ONE;
            } else {
                gameboard[i][j] = EMPTY; // Filling empty spaces
            }
        }
    }
    return true;
}

//Checks if the game is over by determining if there are any empty squares left on the game board.
bool GameBoard::is_game_over() const
{
    // Check if there are any empty squares left on the gameboard
    for (unsigned int i = 0; i < SIZE; ++i) {
        for (unsigned int j = 0; j < SIZE; ++j) {
            if (gameboard[i][j] == EMPTY) {
                return false; // Game is not over yet
            }
        }
    }
    return true; // If no empty squares are found, game is over
}

bool GameBoard::setElement(unsigned int row, unsigned int col, Element_type value)
{
    // Check if coordinates are within the board
    if (row >= SIZE || col >= SIZE) {
        return false;
    }

    // Check if the square is already filled
    if (gameboard[row][col] != EMPTY) {
        return false;
    }

    // Count how many zeros and ones are already in the row and column
    int countZerosRow = 0;
    int countOnesRow = 0;
    int countZerosCol = 0;
    int countOnesCol = 0;
    for (int i = 0; i < SIZE; ++i) {
        if (gameboard[row][i] == ZERO) {
            countZerosRow++;
        } else if (gameboard[row][i] == ONE) {
            countOnesRow++;
        }
        if (gameboard[i][col] == ZERO) {
            countZerosCol++;
        } else if (gameboard[i][col] == ONE) {
            countOnesCol++;
        }
    }

    // Check if adding this value violates the rules
    if ((value == ZERO && countZerosRow >= 3) || (value == ONE && countOnesRow >= 3) ||
        (value == ZERO && countZerosCol >= 3) || (value == ONE && countOnesCol >= 3)) {
        return false;
    }

    // Check if adding this value violates the sequential rule
    if ((value == ZERO && col >= 2 && gameboard[row][col - 1] == ZERO && gameboard[row][col - 2] == ZERO) ||
        (value == ONE && col >= 2 && gameboard[row][col - 1] == ONE && gameboard[row][col - 2] == ONE) ||
        (value == ZERO && row >= 2 && gameboard[row - 1][col] == ZERO && gameboard[row - 2][col] == ZERO) ||
        (value == ONE && row >= 2 && gameboard[row - 1][col] == ONE && gameboard[row - 2][col] == ONE)) {
        return false;
    }

    // Update the gameboard with the new value
    gameboard[row][col] = value;
    return true;
}



