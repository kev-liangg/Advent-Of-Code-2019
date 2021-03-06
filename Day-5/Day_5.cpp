/*
 * Day 5: restructured Day 2 code, sequentially execute "machine code"
 * instructions given additional opcodes.
 *
 * representation of memory is continuous, which requires implementation of a
 * program counter.
 *
 * opcodes must also be parsed differently, and support parameters. given input
 * val with 5 digits: [mode2][mode1][mode0][opcode][opcode]
 *   mode[i] = 0 represents value at the index for respective param i
 *   mode[i] = 1 represents immediate value for respective param i.
 */

#include <iostream>
#include <fstream>
#include <vector>

/*
 * process the inputs given by opcodes and entries within the input values
 * pass vector by reference for performance
 *
 * Day 5 update: no longer requires overriding first two positions. Takes a
 * fixed user input for use with new instructions.
 */
void processInput (std::vector<int> &inputVals, int input);

/*
 * parses the entire input opcode, passed through the first param
 * converts initial param into the 2-digit opcode and updates following params
 * into their modes, all passed by references
 */
void parseOpcode (int &opcode, int &mode1, int &mode2, int &mode3);

/*
 * runs the opcode as specified by index on the machine instructions, updating
 * the entire input representing the indexed memory block
 *
 * part 2: index can be modified by "jumps" within the opcodes
 *
 * returns the "program counter increment," the offset to the next opcode
 */
int runOpcode (std::vector<int> &inputVals, int &index, int input);

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
    inputOriginal.assign (inputVals.begin (), inputVals.end ());

    /* Part 1: -------------------------------------------------------------- */

    // initial conditions: input = 1
    processInput (inputVals, 1);

    printf ("Part 1 Solution: See last nonzero output\n");

    /* Part 2: -------------------------------------------------------------- */

    inputVals.assign (inputOriginal.begin (), inputOriginal.end ());

//     initial conditions: input = 5
    processInput (inputVals, 5);

    printf ("Part 2 Solution: See last output\n");
}

void processInput (std::vector<int> &inputVals, int input) {
    // iterate through inputs individually; opcodes not at fixed positions
    int i = 0;
    while (i < inputVals.size ()) {
        // execute the opcode with helper function
//        printf("running opcode with index %d\n", i);
        int offset = runOpcode (inputVals, i, input);
        i += offset;
    }
}

void parseOpcode (int &opcode, int &mode1, int &mode2, int &mode3) {
    // last two digits contain opcode
    int opcodeFinal = opcode % 100;
    opcode = opcode / 100;
    // next least significant digit is mode of param 0, then 1, and so on
    // leading zeros parsed as zeros, as intended as per specifications
    mode1 = opcode % 10;
    opcode = opcode / 10;
    mode2 = opcode % 10;
    opcode = opcode / 10;
    mode3 = opcode % 10;
    opcode = opcode / 10;
    // update initial param to first parsed opcode
    opcode = opcodeFinal;
}

int runOpcode (std::vector<int> &inputVals, int &index, int input) {
    // offset program counter, varies from how many inputs used for op
    int offset = 0;
    // get raw opcode from input
    int opcode = inputVals.at (index);
    // obtain param modes and final opcode from parse helper
    int mode1, mode2, mode3;
    parseOpcode (opcode, mode1, mode2, mode3);

    // assume fetch max of 3 immediate params to simplify following switch cases
    int param [3];
    for (int i = 0; i + index + 1 < inputVals.size() && i < 3; i++) {
        param[i] = inputVals.at (i + index + 1);
    }
    // must access final 3 values based on the utilized params for each case
    int val1, val2, val3;
    switch (opcode) {
        // opcode 99: halt, return size to break out of caller loop
        case 99 :
            offset = inputVals.size ();
            break;
        // opcode 1 and 2 both valid opcodes from Day 2
        case 1 :
        case 2 :
            // val1, val2 used
            val1 = mode1 ? param[0] : inputVals.at (param[0]);
            val2 = mode2 ? param[1] : inputVals.at (param[1]);
            // opcode 1: add vals; otherwise opcode 2: multiply final vals
            inputVals.at (param[2]) = (opcode == 1) ? val1 + val2 : val1 * val2;
            offset = 4;
            break;
        // part 1, opcode 3: write to position given by immediate param
        case 3 :
            inputVals.at (param[0]) = input;
            offset = 2;
            break;
        // part 1, opcode 4: "output" from single param and mode
        case 4 :
            // process and print final value of param[0]
            val1 = mode1 ? param[0] : inputVals.at (param[0]);
            printf("output: %d\n", val1);
            offset = 2;
            break;
        // part 2, opcode 5: if first param nonzero, set pc using second param
        case 5 :
            val1 = mode1 ? param[0] : inputVals.at (param[0]);
            val2 = mode2 ? param[1] : inputVals.at (param[1]);
            // check final value of first param, update pc accordingly
            if (val1 != 0) {
                index = val2;
            }
            else {
                offset = 3;
            }
            break;
        // part 2, opcode 6: if first param zero, set pc using second param
        case 6 :
            val1 = mode1 ? param[0] : inputVals.at (param[0]);
            val2 = mode2 ? param[1] : inputVals.at (param[1]);
            // check final value of first param, update pc accordingly
            if (val1 == 0) {
                index = val2;
            }
            else {
                offset = 3;
            }
            break;
        // part 2, opcode 7: if first param < second param, 1 in third param
        case 7 :
            val1 = mode1 ? param[0] : inputVals.at (param[0]);
            val2 = mode2 ? param[1] : inputVals.at (param[1]);
            // compare final values of first and second params
            if (val1 < val2) {
                inputVals.at (param[2]) = 1;
            }
            else {
                inputVals.at (param[2]) = 0;
            }
            offset = 4;
            break;
        // part 2, opcode 8: if first param == second param, 1 in third param
        case 8 :
            val1 = mode1 ? param[0] : inputVals.at (param[0]);
            val2 = mode2 ? param[1] : inputVals.at (param[1]);
            // compare final values of first and second params
            if (val1 == val2) {
                inputVals.at (param[2]) = 1;
            }
            else {
                inputVals.at (param[2]) = 0;
            }
            offset = 4;
            break;
        default :
            printf ("invalid opcode, error in input\n");
            break;
    }
    // return program counter to determine next index of opcode for caller
    return offset;
}


