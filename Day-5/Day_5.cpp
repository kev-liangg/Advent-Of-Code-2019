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
 * returns the "program counter increment," the offset to the next opcode
 */
int runOpcode (std::vector<int> &inputVals, int index, int input);

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
    processInput (inputVals, 1);

    printf ("Part 1 Solution: See last nonzero output\n");

}

void processInput (std::vector<int> &inputVals, int input) {
    // iterate through inputs individually; opcodes not at fixed positions
    int i = 0;
    while (i < inputVals.size ()) {
        // execute the opcode with helper function
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

int runOpcode (std::vector<int> &inputVals, int index, int input) {
    // offset program counter, varies from how many inputs used for op
    int offset = 0;
    // get raw opcode from input
    int opcode = inputVals.at (index);
    // obtain param modes and final opcode from parse helper
    int mode1, mode2, mode3;
    parseOpcode (opcode, mode1, mode2, mode3);
    // opcode 99: halt, return size of input to break out of caller while
    if (opcode == 99) {
        return inputVals.size ();
    }
    // valid opcodes from Day 2: 1, 2
    else if (opcode == 1 || opcode == 2) {
        // get value of next 3 params for the operation
        int param1 = inputVals.at (index + 1);
        int param2 = inputVals.at (index + 2);
        int writeIndex = inputVals.at (index + 3);
        // actual values for operation differs by mode: 0->index, 1->immediate
        int val1 = mode1 ? param1 : inputVals.at (param1);
        int val2 = mode2 ? param2 : inputVals.at (param2);
        // opcode 1: add vals; otherwise opcode 2: multiply vals
        int toWrite = (opcode == 1) ? val1 + val2 : val1 * val2;
        // write calculated value to index; always in indexing mode
        inputVals.at (writeIndex) = toWrite;
        offset = 4;
    }
    // new opcode 3: write to position given by only parameter
    else if (opcode == 3) {
        int writeIndex = inputVals.at (index + 1);
        inputVals.at (writeIndex) = input;
        offset = 2;
    }
    // new opcode 4: "outputs" from its only parameter
    else if (opcode == 4) {
        int param1 = inputVals.at (index + 1);
        // value to output differs by mode: 0->index, 1->intermediate
        int valOut = mode1 ? param1 : inputVals.at (param1);
        printf("output: %d\n", valOut);
        offset = 2;
    }
    else {
        printf ("invalid opcode, error in input\n");
    }
    // return program counter to determine next index of opcode for caller
    return offset;
}


