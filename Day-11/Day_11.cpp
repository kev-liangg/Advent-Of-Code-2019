/*
 * Day 11: Use program from Day 9 to control a robot in a 2D space of different
 * colors.
 *
 * Robot can detect color in current space, taking it as input.
 *
 * First output indicates color to paint current location, second output
 * indicates direction of left/right 90 degree turn, then move forward.
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

/*
 * process the inputs given by opcodes and entries within the input values
 * pass vector by reference for performance
 */
int processInput (std::vector<long> &inputVals, long input, int &index);

/*
 * parses the entire input opcode, passed through the first param
 * converts initial param into the 2-digit opcode and updates following params
 * into their modes, all passed by references
 */
void parseOpcode (long &opcode, int &mode1, int &mode2, int &mode3);

/*
 * runs the opcode as specified by index on the machine instructions, updating
 * the entire input representing the indexed memory block
 *
 * part 2: index can be modified by "jumps" within the opcodes
 *
 * returns the "program counter increment," the offset to the next opcode
 */
int runOpcode (std::vector<long> &inputVals, int &index, long input,
               long &output, int &relativeBase);

/*
 * Hash function for pair representing int coordinates to be painted
 */
typedef std::pair<int, int> coord;

class pairHash {
public:
    size_t operator () (const std::pair<int, int> &toHash) const {
        return toHash.first * 100 + toHash.second;
    }
};

/*
 * Run the painter robot from the input of opcode instructions
 */
void runPainter (std::unordered_map<coord, int, pairHash> &paintMap,
                 std::vector<long> &inputVals);

int main () {
    std::ifstream inFile ("input.txt");
    // add each input value to vector for indexed read/write operations
    std::vector<long> inputVals;
    std::string val;
    // use getline for parse, with each input "line" delimited by commas
    while (std::getline (inFile, val, ',')) {
        inputVals.push_back (std::stol (val));
    }
    // obtain deep copy of this original vector for part 2:
    std::vector<long> inputOriginal;
    inputOriginal.assign (inputVals.begin (), inputVals.end ());

    /* Part 1: -------------------------------------------------------------- */

    // track painted coordinates: start at (0, 0), val 0->black, val1->white
    std::unordered_map<coord, int, pairHash> paintMap;
    runPainter (paintMap, inputVals);

    // number of painted squares stored in paintMap
    printf("Part 1 Solution: %d\n", paintMap.size ());
}

void runPainter (std::unordered_map<coord, int, pairHash> &paintMap,
                 std::vector<long> &inputVals) {
    // current position at origin, facing up
    coord currPos ({0, 0});
    int currDir = 0;
    // represent movement with index: 0->up, 1->right, 2->down, 3->left
    coord directions[] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

    paintMap.insert ({currPos, 0});

    int index = 0;
    while (index < inputVals.size ()) {
        // get the color of the current position, default painted black
        int colorInput = 0;
        std::unordered_map<coord, int, pairHash>::iterator search;
        search = paintMap.find (currPos);
        // location already painted, get color as input
        if (search != paintMap.end ()) {
            colorInput = search->second;
        }
        int colorOutput = processInput (inputVals, colorInput, index);
        // location already in painted map, update color
        if (search != paintMap.end ()) {
            search->second = colorOutput;
        }
        // otherwise need to add location to map with painted color
        else {
            paintMap.insert ({currPos, colorOutput});
        }
        // get turn direction: 0 -> left, 1 -> right 90 degrees
        int dir = processInput (inputVals, 1, index);
        if (dir) {
            // increment turn index by one with wrap-around
            currDir = (currDir + 1) % 4;
        }
        else {
            currDir = currDir ? currDir - 1 : 3;
        }
        // move one step forward, using movement array representing dX/dY
        currPos.first += directions[currDir].first;
        currPos.second += directions[currDir].second;
    }
}

int processInput (std::vector<long> &inputVals, long input, int &index) {
    // iterate through inputs individually; opcodes not at fixed positions
    int relativeBase = 0;
    long output = -99999;
    // run up to next output
    while (index < inputVals.size ()) {
        // execute the opcode with helper function
        int offset = runOpcode (inputVals, index, input, output, relativeBase);
        index += offset;
        // return at first output, 0 or 1
        if (output != -99999) {
            break;
        }
    }
    return output;
}

void parseOpcode (long &opcode, int &mode1, int &mode2, int &mode3) {
    // last two digits contain opcode
    long opcodeFinal = opcode % 100;
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

long accessInput (std::vector<long> &inputVals, int index) {
    // bounds checking: index cannot be negative
    if (index < 0) {
        return -99999;
    }
    // higher than given input, resize and return 0
    else if (index >= inputVals.size ()) {
        inputVals.resize (index);
        return 0;
    }
    // within bounds
    else {
        return inputVals.at (index);
    }
}

// helper function to return the actual value for a parameter access
long getVal (std::vector<long> &inputVals, long param, int mode,
             int &relativeBase) {
    // immediate mode
    if (mode == 1) {
        return param;
    }
    // position mode and relative mode
    else if (mode == 0 || mode == 2) {
        // calculate index to access, mode 0 absolute position
        int indexAccess = !mode ? param : param + relativeBase;
        return accessInput (inputVals, indexAccess);
    }

    // invalid mode
    else {
        printf ("invalid input encountered\n");
        return -99999;
    }
}

int runOpcode (std::vector<long> &inputVals, int &index, long input,
               long &output, int &relativeBase) {
    // offset program counter, varies from how many inputs used for op
    int offset = 0;
    // get raw opcode from input
    long opcode = inputVals.at (index);
    // obtain param modes and final opcode from parse helper
    int mode1, mode2, mode3, writeIndex;
    parseOpcode (opcode, mode1, mode2, mode3);
    // assume fetch max of 3 immediate params to simplify following switch cases
    long param [3];
    for (int i = 0; i + index + 1 < inputVals.size() && i < 3; i++) {
        param[i] = inputVals.at (i + index + 1);
    }
    // must access final 3 values based on the utilized params for each case
    long val1 = getVal (inputVals, param[0], mode1, relativeBase);
    long val2 = getVal (inputVals, param[1], mode2, relativeBase);
    long val3 = getVal (inputVals, param[2], mode3, relativeBase);
    switch (opcode) {
        // opcode 99: halt, return size to break out of caller loop
        case 99 :
            offset = inputVals.size ();
            break;
        // opcode 1 and 2 both valid opcodes from Day 2
        case 1 :
        case 2 :
            // determine write index if in relative mode
            writeIndex = mode3 == 2 ? param[2] + relativeBase : param[2];
            // opcode 1: add vals; otherwise opcode 2: multiply final vals
            inputVals.at (writeIndex) = (opcode == 1) ? val1 + val2 : val1 * val2;
            offset = 4;
            break;
        // part 1, opcode 3: write to position given by immediate param
        case 3 :
            // determine write index if in relative mode
            writeIndex = mode1 == 2 ? param[0] + relativeBase : param[0];
            inputVals.at (writeIndex) = input;
            offset = 2;
            break;
        // part 1, opcode 4: "output" from single param and mode
        case 4 :
            output = val1;
            offset = 2;
            break;
        // part 2, opcode 5: if first param nonzero, set pc using second param
        case 5 :
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
            // determine write index if in relative mode
            writeIndex = mode3 == 2 ? param[2] + relativeBase : param[2];
            // compare final values of first and second params
            if (val1 < val2) {
                inputVals.at (writeIndex) = 1;
            }
            else {
                inputVals.at (writeIndex) = 0;
            }
            offset = 4;
            break;
        // part 2, opcode 8: if first param == second param, 1 in third param
        case 8 :
            // determine write index if in relative mode
            writeIndex = mode3 == 2 ? param[2] + relativeBase : param[2];
            // compare final values of first and second params
            if (val1 == val2) {
                inputVals.at (writeIndex) = 1;
            }
            else {
                inputVals.at (writeIndex) = 0;
            }
            offset = 4;
            break;
        // Day 9: adjusts the relative base from value of only parameter
        case 9 :
            relativeBase += val1;
            offset = 2;
            break;
        default :
            printf ("invalid opcode, error in input\n");
            break;
    }
    // return program counter to determine next index of opcode for caller
    return offset;
}
