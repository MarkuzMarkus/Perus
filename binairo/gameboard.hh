//Represents the game board for the Binairo game.
// Header file for the GameBoard class, which implements a gameboard for the Binairo game.
// Defines the class GameBoard and its member functions for managing and manipulating the gameboard.

//Name: Markus Tuovinen
//Student number: 151276149
//UserID: csmatu
//E-Mail: markus.tuovinen@tunif.i

#ifndef GAMEBOARD_HH
#define GAMEBOARD_HH

#include <vector>
#include <string>
#include <algorithm>

// Type of the elements in the gameboard.
enum Element_type {ZERO, ONE, EMPTY};

// Constants for the size of the gameboard.
const int NUMBER_OF_SYMBOLS = 3;
const int SIZE = 2 * NUMBER_OF_SYMBOLS;

// Constant for the upper bound of probability distribution.
// Zeros and ones have the same probability, say x, and the estimated
// probability of empties is 6x, whereupon their sum is 8x, and thus,
// the interval must have eigth points, as [0..7].
const int DISTR_UPPER_BOUND = 7;

// Vasemmanpuoleisimman sarakkeen koko, tarvitaan pelilaudan tulostamisessa.
// Width of the left-most column, needed in printing the gameboard.
const unsigned int LEFT_COLUMN_WIDTH = 5;


class GameBoard
{
public:
    // Constructor.
    GameBoard();

    // Prints the gameboard.
    void print() const;

    // Fill the gameboard randomly based on the given seed
    void fill_randomly(int seed);

    // Fill the gameboard based on the provided input string
    bool fill_with_input(const std::string& input);

    bool is_game_over() const;

    // Set the value of an element at a specific position
    bool setElement(unsigned int row, unsigned int col, Element_type value);

    // Getter function for BAD_SEEDS
    const std::vector<unsigned int>& getBadSeeds() const { return BAD_SEEDS; }

    // Getter function for gameboard
    const Element_type (*getGameboard() const)[SIZE] { return gameboard; }

private:
    // Prints a line with the given length and the given character.
    void print_line(unsigned int length, char fill_character) const;

    // Seed values, from [1..50], producing non-solvable gameboard.
    const std::vector<unsigned int> BAD_SEEDS = { 2, 8, 12, 13, 16, 20, 21, 23,
                                                  26, 29, 31, 32, 34, 41, 42,
                                                  43, 44, 46 };

    // Gameboard data structure
    Element_type gameboard[SIZE][SIZE];
};

#endif // GAMEBOARD_HH
