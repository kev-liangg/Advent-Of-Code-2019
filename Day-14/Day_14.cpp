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
typedef std::pair<std::string, long> req;

/*
 * hash mapping of each material and its total requirements
 */
typedef std::unordered_map<std::string, std::vector<req>> MatInfo;

/*
 * hash mapping of a material requirement to an individual count
 */
typedef std::unordered_map<std::string, long> ReqCount;

void processLine (std::string &in, std::vector<req> &reqs);

long calcOres (MatInfo &mats, ReqCount &currReqs,
               ReqCount &excesses);

/*
 * Part 2: Calculate the number of ores required for certain amount of fuel
 */
long calcOresFuel (long numFuel, MatInfo &mats, ReqCount excesses);

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

    int totalOres = calcOres (mats, currReqs, excesses);
    std::cout << "Part 1 Solution: " << totalOres << std::endl;

    /* Part 2: -------------------------------------------------------------- */

    // reset excesses, before passing-by-copy
    for (std::pair<const std::string, long> &pair : excesses) {
        pair.second = 0;
    }

    // lower bound: at most 1 trillion divided by previous requirement
    long ores = 1e12;
    long low = ores / totalOres;
    long high = low;
    long result = calcOresFuel (low, mats, excesses);
    // brute force: increment number of fuel required until over 1e12
    while (result < 1e12) {
        high *= 10;
        result = calcOresFuel (high, mats, excesses);
    }

    // binary search
    long mid;
    while (low < high - 1) {
        mid = (high + low) / 2;
        result = calcOresFuel (mid, mats, excesses);
        if (result > 1e12) {
            high = mid;
        }
        else if (result < 1e12) {
            low = mid;
        }
        else {
            break;
        }
    }

    // overshot, last value of fuel was max able to be produced
    std::cout << "Part 2 Solution: " << mid << std::endl;
}

long calcOresFuel (long numFuel, MatInfo &mats, ReqCount excesses) {
    // set up requirements for specified number of fuel
    MatInfo::iterator search = mats.find ("FUEL");
    std::vector<req> fuelReqs = search->second;
    fuelReqs.pop_back ();
    ReqCount currReqs;
    for (req r : fuelReqs) {
        currReqs.insert ({r.first, r.second * numFuel});
    }
    return calcOres (mats, currReqs, excesses);
}

long calcOres (MatInfo &mats, ReqCount &currReqs,
               ReqCount &excesses) {
    // iterative solution
    long ore = 0;
    while (currReqs.size ()) {
        ReqCount::iterator curr = currReqs.begin();
        std::string name = curr->first;
        long numRequired = curr->second;

        // update required with excess
        ReqCount::iterator searchCount = excesses.find (name);
        long& currExcess = searchCount->second;
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
            long output = reqs.back ().second;
            reqs.pop_back ();

            // check if excess generated while calculating scale factor
            long scale = numRequired / output;
            long excess = 0;
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
