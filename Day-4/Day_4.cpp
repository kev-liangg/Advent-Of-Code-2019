/*
 * Day 4: Given the criteria for a 6-digit password, determine the number of
 * matching passwords between two given passwords, to serve as ranges.
 *
 * Rules: digits never strictly decrease, and two adjacent digits are the same
 *
 * Additional Part 2 Rule: the two adjacent matching digits is not part of
 *   a larger group of matching digits
 */

#include <fstream>

int main () {
    // parse input, determine upper and lower ranges
    std::ifstream fileIn ("input.txt");
    std::string input;
    std::getline (fileIn, input);
    int lower = std::stoi (input.substr (0, 6));
    int upper = std::stoi (input.substr (7));

    /* Part 1: -------------------------------------------------------------- */

    // begin guessing across the range
    int numValid = 0;
    for (int guess = lower; guess <= upper; guess++) {
        // accesses least significant digit
        int prevDigit = guess % 10;
        int currGuess = guess / 10;
        bool consecFound = false;
        // from remaining 5-digit value, access the next least significant
        while (currGuess > 0) {
            int currDigit = currGuess % 10;
            // current digit is leftward of previous; must be smaller or equal
            if (currDigit > prevDigit) {
                break;
            }
            // must be at least two consecutive digits that are equal
            if (currDigit == prevDigit) {
                consecFound = true;
            }
            currGuess = currGuess / 10;
            prevDigit = currDigit;
        }
        // loop didn't early return, guess should be zero; valid password guess
        if (!currGuess && consecFound) {
            numValid++;
        }
    }

    printf("Part 1 solution: %d\n", numValid);

    /* Part 2: -------------------------------------------------------------- */

    // reuse guessing code from Part 1, but add extra condition
    numValid = 0;
    for (int guess = lower; guess <= upper; guess++) {
        // accesses least significant digit
        int prevDigit = guess % 10;
        int currGuess = guess / 10;

        // additional rule: check for larger groups of matching numbers
        bool consecFound = false;
        // track number of consecutive occurrences for any given digit
        int numAdjacent = 0;
        // from remaining 5-digit value, access the next least significant
        while (currGuess > 0) {
            int currDigit = currGuess % 10;
            // current digit is leftward of previous; must be smaller or equal
            if (currDigit > prevDigit) {
                break;
            }
            // check consecutive digits, but only valid if not part of larger
            if (currDigit == prevDigit) {
                numAdjacent++;
            }
            else {
                // found block of only two adjacent; not part of larger group
                if (numAdjacent == 1) {
                    consecFound = true;
                }
                numAdjacent = 0;
            }
            currGuess = currGuess / 10;
            prevDigit = currDigit;
        }
        // loop didn't early return, guess should be zero
        // last two adjacent could have been final 2 digits checked
        if (!currGuess && (consecFound || numAdjacent == 1)) {
            numValid++;
        }
    }

    printf("Part 2 solution: %d\n", numValid);
}
