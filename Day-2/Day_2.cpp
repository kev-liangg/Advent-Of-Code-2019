/*
 * Day 2: parse and process an input of integers, separated by commas
 */

#include <iostream>
#include <fstream>
#include <vector>

/*
 * process the inputs given by opcodes and entries within the input values
 * pass vector by reference for performance
 */
void processInput (std::vector<int> &inputVals, int initVal1, int initVal2);

int main () {
    std::ifstream inFile ("input.txt");
    // add each input value to vector for indexed read/write operations
    std::vector<int> inputVals;
    std::string val;
    // use getline for parse, with each input "line" delimited by commas
    while (std::getline (inFile, val, ',')) {
        inputVals.push_back (std::stoi (val));
    }
    // obtain deep copy of this original vector for part 2:
    std::vector<int> inputOriginal;
    inputOriginal.assign(inputVals.begin (), inputVals.end ());

    /* Part 1: -------------------------------------------------------------- */

    // initial conditions: position 1 with val 12, position 2 with val 2
    processInput (inputVals, 12, 2);

    printf ("Part 1 Solution: %d\n", inputVals.at (0));

    /* Part 2: -------------------------------------------------------------- */
    // reset inputVals with deep copy
    inputVals.assign (inputOriginal.begin (), inputOriginal.end ());
    // brute force assign positions 1 and 2 with values to generate 19690720
    int target = 0;
    int initVal1 = 0;
    int initVal2 = 0;
    while (target != 19690720) {
        while (target != 19690720 && initVal2 <= 99) {
            inputVals.assign (inputOriginal.begin (), inputOriginal.end ());
            // inner nested loop: postfix increment initVal2 for after call
            processInput (inputVals, initVal1, initVal2++);
            target = inputVals.at (0);
        }
        if (target != 19690720) {
            // outer loop finished without success, try next initVal1
            initVal1++;
            initVal2 = 0;
        }
    }

    // due to postfix increment, initVal2 is 1 ahead; change in solution
    printf ("Part 2 Solution: %d\n", 100 * initVal1 + initVal2 - 1);
}

void processInput (std::vector<int> &inputVals, int initVal1, int initVal2) {
    // preprocess input: replace position 1 with val 12, position 2 with val 2
    inputVals.at (1) = initVal1;
    inputVals.at (2) = initVal2;

    // iterate through opcodes, every fourth index
    for (unsigned int i = 0; i < inputVals.size (); i += 4) {
        // determine opcode
        int opcode = inputVals.at (i);

        // opcode 99: halt, break out of loop without writing
        if (opcode == 99) {
            break;
        }
        // valid opcodes: 1, 2
        else if (opcode == 1 || opcode == 2) {
            // calculate next 3 inputs for opcode
            int index1 = inputVals.at (i + 1);
            int index2 = inputVals.at (i + 2);
            int writeIndex = inputVals.at (i + 3);
            // access vector given by indices
            int val1 = inputVals.at (index1);
            int val2 = inputVals.at (index2);
            // opcode 1: add vals; otherwise opcode 2: multiply vals
            int toWrite = (opcode == 1) ? val1 + val2 : val1 * val2;
            // write calculated value to index
            inputVals.at (writeIndex) = toWrite;
        }
        else {
            printf ("invalid opcode, error in input\n");
        }
    }
}
