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

/*
 * hash mapping of a material requirement to an individual count
 */
typedef std::unordered_map<std::string, int> ReqCount;

void processLine (std::string &in, std::vector<req> &reqs);

int calcOres (MatInfo &mats, ReqCount &currReqs,
              ReqCount &excesses);

int main () {
    std::ifstream fileIn ("input.txt");
    std::string line;
    MatInfo mats;

    // track excess materials, map all to zero during input parse
    ReqCount excesses;
    while (std::getline (fileIn, line)) {
        std::vector<req> reqs;
        processLine (line, reqs);
        // add to map
        std::string name = reqs.back ().first;
        mats.insert ({name, reqs});
        excesses.insert ({name, 0});
    }

    /* Part 1: -------------------------------------------------------------- */
    MatInfo::iterator search = mats.find ("FUEL");
    std::vector<req> fuelReqs = search->second;
    fuelReqs.pop_back ();

    ReqCount currReqs;
    for (req r : fuelReqs) {
        currReqs.insert ({r.first, r.second});
    }
    // run calculation function
    int totalOres = calcOres (mats, currReqs, excesses);
    std::cout << "Part 1 Solution: " << totalOres << std::endl;
}

int calcOres (MatInfo &mats, ReqCount &currReqs,
              ReqCount &excesses) {
    // iterative solution
    int ore = 0;
    while (currReqs.size ()) {
        ReqCount::iterator curr = currReqs.begin();
        std::string name = curr->first;
        int numRequired = curr->second;

        // update required with excess
        ReqCount::iterator searchCount = excesses.find (name);
        int& currExcess = searchCount->second;
        if (currExcess >= numRequired) {
            currExcess -= numRequired;
            currReqs.erase (name);
        }
        else {
            numRequired -= currExcess;
            currExcess = 0;
            // get total materials needed to produce current requirement
            MatInfo::iterator searchReqs = mats.find (name);
            std::vector<req> reqs = searchReqs->second;
            // determine total number material output required, scale factor
            int output = reqs.back ().second;
            reqs.pop_back ();

            // check if excess generated while calculating scale factor
            int scale = numRequired / output;
            int excess = 0;
            if (numRequired % output) {
                scale += 1;
                excess = output * scale - numRequired;
            }

            // remove current requirement and update excess
            currReqs.erase (name);
            searchCount->second = searchCount->second + excess;

            for (req r : reqs) {
                // base case: check for ore requirement
                if (r.first == "ORE") {
                    ore += scale * r.second;
                }
                // otherwise add to map of current requirements
                else {
                    searchCount = currReqs.find (r.first);
                    if (searchCount == currReqs.end ()) {
                        currReqs.insert ({r.first, scale * r.second});
                    }
                    else {
                        searchCount->second += scale * r.second;
                    }
                }
            }
        }
    }
    return ore;
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
