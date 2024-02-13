/* Binairo
 *
 * Desc:
 *   This program implements a Binairo game with the gameboard of size 6 x 6.
 * Each square in the gameboard has zero, one, or empty. The aim is to add
 * zeros and ones in the empty squares by following the rules:
 * - each horizontal and vertical line can have at most three identical numbers
 * - each horizontal and vertical line can have at most two identical number
 *   sequentially.
 *   At first, the user is asked, if the gameboard will be filled with
 * randomly drawn characters, or with user-given 36 characters. In the first
 * option, a seed value for the random number generator will be asked next.
 * In the latter option, the user is asked to input 36 characters, the
 * validity of which is checked.
 *   On each round, the user is asked for the coordinates and the identity
 * of the character to be added, i.e. three characters. The player wins if
 * the gameboard has been filled following the rules above. The program does
 * not allow additions violating the above rules, but it is possible to end up
 * to a situation where no addition is possible any more.
 *   The program checks the user-given inputs. The character to be added must
 * be zero or one. The coordinates must be inside the gameboard, and the
 * square indicated by them must be empty.
 *
 * Name: Markus Tuovinen
 * Student number: 151276149
 * UserID: csmatu
 * E-Mail: markus.tuovinen@tunif.i
 */

#include "gameboard.hh"
#include <iostream>
#include <sstream>

using namespace std;

// Output messages
const string QUIT = "Quitting ...";
const string OUT_OF_BOARD = "Out of board";
const string INVALID_INPUT = "Invalid input";
const string CANT_ADD = "Can't add";
const string WIN = "You won!";

// Converts the given numeric string to the corresponding integer
// (by calling stoi) and returns the integer.
// If the given string is not numeric, returns zero.
unsigned int stoi_with_check(const string& str)
{
    bool is_numeric = true;
    for(unsigned int i = 0; i < str.length(); ++i)
    {
        if(not isdigit(str.at(i)))
        {
            is_numeric = false;
            break;
        }
    }
    if(is_numeric)
    {
        return stoi(str);
    }
    else
    {
        return 0;
    }
}

// Removes empty characters (such as ' ' etc.) from the given string.
// Returns true if the given string has exactly one non-empty character,
// which is either '0' or '1', otherwise returns false.
bool find_fill_symbol(string& str)
{
    string fill_str = "";
    for(unsigned int i = 0; i < str.size(); ++i)
    {
        if(not isspace(str.at(i)))
        {
            fill_str += str.at(i);
        }
    }
    str = fill_str;
    return (fill_str.size() == 1 and
           (fill_str.at(0) == '0' or fill_str.at(0) == '1'));
}

// Enables the user to play the game, i.e. by repeatedly asking an element
// to be added and its position, until the game is over.
void play_game(GameBoard& board)
{
    board.print();
    while (not board.is_game_over())
    {
        string input;
        cout << "Enter two coordinates and a fill symbol, or q to quit: ";
        getline(cin, input);

        // Check if the user wants to quit
        if (input == "q" || input == "Q") {
            cout << QUIT << endl;
            return;
        }

        stringstream ss(input);
        unsigned int x, y;
        char fill;
        ss >> x >> y >> fill;

        // Check if coordinates are within the board
        if (x < 1 || x > SIZE || y < 1 || y > SIZE) {
            cout << OUT_OF_BOARD << endl;
            continue;
        }

        // Check if fill symbol is valid
        if (fill != '0' && fill != '1') {
            cout << INVALID_INPUT << endl;
            continue;
        }

        // Try to set the element at the specified coordinates
        if (!board.setElement(y - 1, x - 1, (fill == '0' ? ZERO : ONE))) {
            cout << CANT_ADD << endl;
            continue;
        }

        // Print the updated board
        board.print();
    }

    // If the game ends up to a totally filled gameboard, the player won
    cout << WIN << endl;
}




// Gives the user a possibility to select a filling way.
// Returns true if filling succeeded, otherwise returns false.
bool select_start(GameBoard& board)
{
    while(true)
    {
        string choice = "";
        cout << "Select start (R for random, I for input): ";
        getline(cin, choice);
        if(choice != "R" && choice != "r" && choice != "I" && choice != "i")
        {
            continue;
        }
        else if(choice == "R" || choice == "r")
        {
            string seed_string = "";
            cout << "Enter a seed value: ";
            getline(cin, seed_string);

            int seed = stoi(seed_string);
            // Check if the seed is bad, if so, inform the user and repeat the prompt
            if (std::find(board.getBadSeeds().begin(), board.getBadSeeds().end(), seed) != board.getBadSeeds().end()) {
                cout << "Bad seed" << endl;
                continue;
            }

            // Fill the board with the validated seed
            board.fill_randomly(seed);
            break;
        }
        else // if(choice == "I" or choice == "i")
        {
            string input = "";
            cout << "Input: ";
            getline(cin, input);

            // Input with quote marks
            if (!board.fill_with_input(input)) {
                // Failed to fill the board with the given user input
                continue;
            }
            break;
        }
    }

    return true; // Successfully filled the board
}


// Short and simple main function.
int main()
{
    GameBoard board;
    while(not select_start(board));
                                    // no code to be repeated
    play_game(board);
    return 0;
}
