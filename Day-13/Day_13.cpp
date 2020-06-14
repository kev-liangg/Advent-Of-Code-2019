/*
 * Day 11: Use programs from Day 9 and 11 to set up the tiles for a game in 2D
 * space.
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

/*
 * process the inputs given by opcodes and entries within the input values
 * pass vector by reference for performance
 *
 * need to pass index and relativeBase as references to store outside of caller
 * function in order to handle start/stop at each output
 */
int processInput (std::vector<long> &inputVals, long input, int &index,
                  int &relativeBase);

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
 * Hash function for pair representing int coordinates for each tile
 */
typedef std::pair<int, int> coord;

class pairHash {
public:
    size_t operator () (const std::pair<int, int> &toHash) const {
        return toHash.first * 100 + toHash.second;
    }
};

/*
 * Set up the tiles of the game from the input
 */
void setupTiles (std::unordered_map<coord, int, pairHash> &tileMap,
                 std::vector<long> &inputVals);

void printTiles (std::unordered_map<coord, int, pairHash> &paintMap);

int main () {
    std::ifstream inFile ("input.txt");
    // add each input value to vector for indexed read/write operations
    std::vector<long> inputVals;
    std::string val;
    // use getline for parse, with each input "line" delimited by commas
    while (std::getline (inFile, val, ',')) {
        inputVals.push_back (std::stol (val));
    }
    // obtain deep copy of this original vector
    std::vector<long> inputOriginal;
    inputOriginal.assign (inputVals.begin (), inputVals.end ());

    /* Part 1: -------------------------------------------------------------- */

    // track tile coordinates: positive int value denotes the tile ID
    std::unordered_map<coord, int, pairHash> tileMap;
    setupTiles (tileMap, inputVals);

    // count number of "ID 2" tiles
    int numBlocks = 0;
    for (std::pair<coord, int> entry : tileMap) {
        if (entry.second == 2) {
            numBlocks ++;
        }
    }
    // number of painted squares stored in paintMap
    printf("Part 1 Solution: %d\n", numBlocks);

    /* Part 2: -------------------------------------------------------------- */
}

void setupTiles (std::unordered_map<coord, int, pairHash> &tileMap,
                 std::vector<long> &inputVals) {
    // every 3 outputs: x, y, then tile type
    int index = 0;
    int relativeBase = 0;
    while (index < inputVals.size ()) {
        int x = processInput (inputVals, 0, index, relativeBase);
        int y = processInput (inputVals, 0, index, relativeBase);
        int type = processInput (inputVals, 0, index, relativeBase);

        tileMap.insert ({{x, y}, type});
    }

}

void printTiles (std::unordered_map<coord, int, pairHash> &paintMap) {
    // find top-left and bottom-right points to print
    int minX = 99999;
    int minY = 99999;
    int maxX = -99999;
    int maxY = -99999;
    for (std::pair<coord, int> entry : paintMap) {
        coord currPos = entry.first;
        if (minX > currPos.first) {
            minX = currPos.first;
        }
        if (minY > currPos.second) {
            minY = currPos.second;
        }
        if (maxX < currPos.first) {
            maxX = currPos.first;
        }
        if (maxY < currPos.second) {
            maxY = currPos.second;
        }
    }

    // starting at top-left point, print the map
    for (int y = minY; y <= maxY; y++) {
        for (int x = maxX; x >= minX; x--) {
            std::unordered_map<coord, int, pairHash>::iterator search;
            coord currPos = {x, y};
            search = paintMap.find (currPos);
            // point found, check if painted white
            if (search != paintMap.end () && search->second == 1) {
                std::cout << "#";
            }
            else {
                std::cout << ".";
            }
        }
        std::cout << "\n";
    }
}


int processInput (std::vector<long> &inputVals, long input, int &index,
                  int &relativeBase) {
    // iterate through inputs individually; opcodes not at fixed positions
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
        printf ("accessing out of bounds %d\n", index);
        return -99999;
    }
    // higher than given input, resize and return 0
    else if (index >= inputVals.size ()) {
        inputVals.resize (index + 1);
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

// helper function to write the val at the index, which can be above bounds
void writeVal (std::vector<long> &inputVals, long toWrite, int index) {
    if (index >= inputVals.size ()) {
        inputVals.resize (index + 1);
    }
    inputVals.at (index) = toWrite;
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
    long val1, val2;
//    printf ("running opcode %d at %d\n", opcode, index);
    switch (opcode) {
        // opcode 99: halt, return size to break out of caller loop
        case 99 :
            offset = inputVals.size ();
            break;
        // opcode 1 and 2 both valid opcodes from Day 2
        case 1 :
        case 2 :
            // determine write index if in relative mode
            val1 = getVal (inputVals, param[0], mode1, relativeBase);
            val2 = getVal (inputVals, param[1], mode2, relativeBase);
            writeIndex = mode3 == 2 ? param[2] + relativeBase : param[2];
            // opcode 1: add vals; otherwise opcode 2: multiply final vals
            if (opcode == 1) {
                writeVal (inputVals, val1 + val2, writeIndex);
            }
            else {
                writeVal (inputVals, val1 * val2, writeIndex);
            }
            offset = 4;
            break;
        // part 1, opcode 3: write to position given by immediate param
        case 3 :
            // determine write index if in relative mode
            writeIndex = mode1 == 2 ? param[0] + relativeBase : param[0];
            writeVal (inputVals, input, writeIndex) ;
            offset = 2;
            break;
        // part 1, opcode 4: "output" from single param and mode
        case 4 :
            val1 = getVal (inputVals, param[0], mode1, relativeBase);
            output = val1;
            offset = 2;
            break;
        // part 2, opcode 5: if first param nonzero, set pc using second param
        case 5 :
            val1 = getVal (inputVals, param[0], mode1, relativeBase);
            val2 = getVal (inputVals, param[1], mode2, relativeBase);
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
            val1 = getVal (inputVals, param[0], mode1, relativeBase);
            val2 = getVal (inputVals, param[1], mode2, relativeBase);
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
            val1 = getVal (inputVals, param[0], mode1, relativeBase);
            val2 = getVal (inputVals, param[1], mode2, relativeBase);
            // determine write index if in relative mode
            writeIndex = mode3 == 2 ? param[2] + relativeBase : param[2];
            // compare final values of first and second params
            if (val1 < val2) {
                writeVal (inputVals, 1, writeIndex);
            }
            else {
                writeVal (inputVals, 0, writeIndex);
            }
            offset = 4;
            break;
        // part 2, opcode 8: if first param == second param, 1 in third param
        case 8 :
            val1 = getVal (inputVals, param[0], mode1, relativeBase);
            val2 = getVal (inputVals, param[1], mode2, relativeBase);
            // determine write index if in relative mode
            writeIndex = mode3 == 2 ? param[2] + relativeBase : param[2];
            // compare final values of first and second params
            if (val1 == val2) {
                writeVal (inputVals, 1, writeIndex);
            }
            else {
                writeVal (inputVals, 0, writeIndex);
            }
            offset = 4;
            break;
        // Day 9: adjusts the relative base from value of only parameter
        case 9 :
            val1 = getVal (inputVals, param[0], mode1, relativeBase);
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

