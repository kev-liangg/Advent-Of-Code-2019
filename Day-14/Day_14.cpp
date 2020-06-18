/*
 * Day 14: Given an input representing various intermediate material formulas,
 * determine the total number of base material required between intermediate
 * steps to form "1 fuel"
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>

void processLine (std::string &,
                  std::unordered_map<std::string, std::vector<std::string>> &);

int main () {
    std::ifstream fileIn ("input.txt");
    std::string line;
    while (std::getline (fileIn, line)) {

    }
}

void processLine (std::string &in,
                  std::unordered_map<std::string, std::vector<std::string>>
                  &reqs) {
    std::stringstream ss (in);
    std::string input;
    std::vector<std::string> temp;

    // get comma delimited input, removing whitespace
    while (std::getline (ss, input, ',')) {
        if (input.at(0) == ' ') {
            input = input.substr (1);
        }
        temp.push_back (input);
    }

    // input contains RHS stored in input
    temp.pop_back ();
    temp.push_back (input.substr (0, input.find (" =>")));
    temp.push_back (input.substr (input.find ("=>") + 3));

    // debug print values
    for (std::string s : temp) {
        std::cout << s << std::endl;
    }
}
