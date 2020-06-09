/*
 * Day 7: modify input instructions from Day 5 code, still sequentially
 * executing the same "machine code" instructions.
 *
 * Requires processing the initial input five times based on a combination of
 * five unique codes. Each code combination generates a final output, which
 * should be maximized.
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

/*
 * process the inputs given by opcodes and entries within the input values
 * pass vector by reference for performance
 *
 * Day 7 update: no inter takes a single input. After the first input
 * instruction is processed, following inputs will switch to a second input
 * value.
 *
 * Day 7 Part 2: For feedback loop mode, add start/stop functionality when
 * waiting for input from previous output
 *
 * Returns the value of the output, rather than printing it
 */
int processInput (std::vector<int> &inputVals, int input1, int input2);
int processInputLoop (std::vector<int> &inputVals, int input1, int input2,
                       int &writeCount);

/*
 * Run the sequence of five execution sequences and outputs the final result
 */
int runSequence (int sequence[], std::vector<int> &inputFixed);

/*
 * Part 2: runs according to the feedback loop
 */
int runLoop (int sequence[], std::vector<int> &inputFixed);

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
 * Day 7 update: flag to switch after the first input instruction.
 *
 * returns the "program counter increment," the offset to the next opcode
 */
int runOpcode (std::vector<int> &inputVals, int &index, int input,
               int &writeCount, bool &waitFlag, int &output);

int main () {
    std::ifstream inFile ("input.txt");
    // add each input value to vector for indexed read/write operations
    std::vector<int> inputVals;
    std::string val;
    // use getline for parse, with each input "line" delimited by commas
    while (std::getline (inFile, val, ',')) {
        inputVals.push_back (std::stoi (val));
    }

//    /* Part 1: -------------------------------------------------------------- */
    int sequence[] = {0, 1, 2, 3, 4};
    int maxOutput = 0;
    // check all permutations of sequence
    do {
        int result = runSequence (sequence, inputVals);
        if (result > maxOutput) {
            maxOutput = result;
        }
    } while (std::next_permutation (sequence, sequence + 5));

    printf ("Part 1 Solution: %d\n", maxOutput);

    /* Part 2: -------------------------------------------------------------- */

    int sequence2[] = {5, 6, 7, 8, 9};
    maxOutput = 0;
    // check all permutations of sequence

    do {
        int result = runLoop (sequence2, inputVals);
        if (result > maxOutput) {
            maxOutput = result;
        }
    } while (std::next_permutation (sequence2, sequence2 + 5));

    printf ("Part 2 Solution: %d\n", maxOutput);
}

int runSequence (int sequence[], std::vector<int> &inputFixed) {
    std::vector<int> inputVals;

    // second input starts at 0 for the first iteration
    int input2 = 0;
    for (int i = 0; i < 5; i++) {
        // reset the memory input
        inputVals.assign (inputFixed.begin (), inputFixed.end ());
        // the result of processing the input will be used for next iteration
        input2 = processInput (inputVals, sequence[i], input2);
    }
    // finished iterations, last result stored in input 2
    return input2;
}

int runLoop (int sequence[], std::vector<int> &inputFixed) {
    // initialize vector to store the input vals of each module
    std::vector<std::vector<int>> inputValsAll;
    for (int i = 0; i < 5; i++) {
        std::vector<int> toAdd (inputFixed.begin (), inputFixed.end ());
        toAdd.push_back (0);
        inputValsAll.push_back (toAdd);
    }
    // each module can execute only two write instructions, then must wait
    int writeCounts[5] = {0, 0, 0, 0, 0};

    // second input starts at 0 for the first iteration
    int input2 = 0;
    int i = 0;
    int numFinished = 0;
    while (numFinished < 5) {

        // the result of processing the input will be used for next iteration
        input2 = processInputLoop (inputValsAll.at (i % 5), sequence[i % 5],
                                   input2, writeCounts[i % 5]);
        // check if amplifier step finished by checking for magic number
        if (inputValsAll.at (i % 5).back () == 99999) {
            numFinished++;
        }

        i++;
    }
    // finished iterations, last result stored in input 2
    return input2;
}

int processInputLoop (std::vector<int> &inputVals, int input1, int input2,
                      int &writeCount) {
    // pass by reference the flag tracking first input instruction and output
    int output = -99999;

    int oldWriteCount = writeCount;
    // iterate through inputs individually; opcodes not at fixed positions
    int index = inputVals.back ();
    // need to stop at first case of writing input
    bool waitFlag = false;
    while (index < inputVals.size ()) {
        // on very first run, use input1 given by signal
        int input = writeCount == 0 ? input1 : input2;

        // check if input instruction ran
        if (writeCount > 1 && oldWriteCount != writeCount) {
            // stop the current module at next input instruction
            waitFlag = true;
        }

        // not past write limit, execute the opcode with helper function
        int offset = runOpcode (inputVals, index, input, writeCount,
                                waitFlag, output);
        index += offset;

        // if reached write limit, break out of loop to get last output
        if (offset == 0 && writeCount > 1) {
            break;
        }

    }
    // store index at end of memory index, unused space
    if (inputVals.back () != 99999) {
        inputVals.back () = index;
    }
    // return output value, passed by reference
    return output;
}

int processInput (std::vector<int> &inputVals, int input1, int input2) {
    // pass by reference the flag tracking first input instruction and output
    int output = 0;

    // iterate through inputs individually; opcodes not at fixed positions
    int i = 0;
    int input = input1;
    // unused variables for forward compatability with part 2
    int writeCount = 0;
    bool tempFlag = false;
    while (i < inputVals.size ()) {
        // execute the opcode with helper function
        int offset = runOpcode (inputVals, i, input, writeCount,
                                tempFlag, output);
        i += offset;
        // check if input instruction ran, firstFlag should be set false
        if (writeCount > 0) {
            // use second input for remaining calculations
            input = input2;
        }
    }
    // return output value, passed by reference
    return output;
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

int runOpcode (std::vector<int> &inputVals, int &index, int input,
               int &writeCount, bool &waitFlag, int &output) {
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
            // no more code to execute, write magic number at end of inputs
            inputVals.push_back (99999);
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
            if (!waitFlag) {
                // record the input write instruction
                writeCount++;
                inputVals.at (param[0]) = input;
                offset = 2;
            }
            break;
        // part 1, opcode 4: "output" into param reference
        case 4 :
            // process and print final value of param[0]
            val1 = mode1 ? param[0] : inputVals.at (param[0]);
            output = val1;
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


