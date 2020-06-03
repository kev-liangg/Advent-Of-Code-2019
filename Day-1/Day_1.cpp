/*
 * Day 1, 2019: Calculate total fuel costs given an input of masses:
 *   cost = mass / 3 - 2
 */

#include <iostream>
#include <fstream>

int main () {
    std::ifstream inFile ("input.txt");
    std::string line;

    /* Part 1: -------------------------------------------------------------- */

    int total = 0;
    while (std::getline (inFile, line)) {
        // parse mass value and add to total
        int mass = std::stoi(line);
        total += mass / 3 - 2;
    }

    printf ("Part 1 solution: %d\n", total);

    /* Part 2: -------------------------------------------------------------- */

    // reset file and totals
    inFile.clear ();
    inFile.seekg (0, std::ios::beg);
    total = 0;

    while (std::getline (inFile, line)) {
        // parse mass value and calculate fuel mass for total
        int mass = std::stoi(line);
        int fuel = mass / 3 - 2;
        total += fuel;

        // calculate fuel cost for this fuel mass, add to total
        while ((fuel = fuel / 3 - 2) > 0) {
            total += fuel;
        }
    }

    printf ("Part 2 solution: %d\n", total);

    return 0;
}
