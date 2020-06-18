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

/*
 * the name of the requirement and number required
 */
typedef std::pair<std::string, int> req;

/*
 * hash mapping of each material and its total requirements
 */
typedef std::unordered_map<std::string, std::vector<req>> MatInfo;

void processLine (std::string &in, std::vector<req> &reqs);

int calcOres (std::string &name, MatInfo &mats, int num);

int main () {
    std::ifstream fileIn ("inputSmall.txt");
    std::string line;
    MatInfo mats;

    while (std::getline (fileIn, line)) {
        std::vector<req> reqs;
        processLine (line, reqs);
        // add to map
        std::string name = reqs.back ().first;
        mats.insert ({name, reqs});
    }

    /* Part 1: -------------------------------------------------------------- */
    MatInfo::iterator search = mats.find ("FUEL");
    std::vector<req> fuelReqs = search->second;
    // caller: calculate requirement for every component of fuel
    for (req r : fuelReqs) {
        std::cout << r.first << " " << r.second << "\n";
    }
}

int calcOres (std::string &name, MatInfo &mats, int num) {
    MatInfo::iterator search = mats.find (name);
    std::vector<req> reqs = search->second;
    for (req r : reqs) {

    }
}

void processLine (std::string &in, std::vector<req> &reqs) {
    std::stringstream ss (in);
    std::string input;
    std::vector<std::string> temp;

    // get comma delimited input
    while (std::getline (ss, input, ',')) {
        temp.push_back (input);
    }

    // input contains RHS stored in input
    temp.pop_back ();
    temp.push_back (input.substr (0, input.find (" =>")));
    temp.push_back (input.substr (input.find ("=>") + 3));

    // push temp values to requirement vector
    for (std::string s : temp) {
        ss.str (s);
        ss.clear ();
        std::string name;
        int num;
        ss >> num >> name;
        reqs.push_back ({name, num});
    }
}
