/*
 * Day 15: use computer code from Day 9 to remotely control a robot in a maze
 * without knowing the location of the walls or target
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <unordered_map>

/*
 * process the inputs given by opcodes and entries within the input values
 * pass vector by reference for performance
 */
long processInput (std::vector<long> &inputVals, long input);

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
 * returns the "program counter increment," the offset to the next opcode
 */
int runOpcode (std::vector<long> &inputVals, int &index, long input,
               long &output, int &relativeBase);

/*
 * recursive backtracking solution to determine minimum steps to maze target
 */
bool runMaze (std::vector<long> &inputVals, int& currSteps, int lastDir);

/*
 * breadth first search to determine maximum depth; call after runMaze to start
 * at target location
 */
int runDepth (std::vector<long> inputVals);

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
    int currSteps = 1;
    runMaze(inputVals, currSteps, -1);
    std::cout << "Part 1 Solution: " << currSteps << std::endl;

    /* Part 2: -------------------------------------------------------------- */
    std::cout << "Part 2 Solution: " << runDepth(inputVals) << std::endl;
}

bool runMaze (std::vector<long> &inputVals, int& currSteps, int lastDir) {
	long output;
	for (int i = 1; i <= 3; i += 2) {	// north and west
		if (i != lastDir) {
			std::vector<long> oldInput = inputVals;
			output = processInput (inputVals, i);
//			std::cout << i << " " << output << " " << currSteps << "\n";
			if (output == 1) {	// last direction given by i + 1
				if (runMaze (inputVals, ++currSteps, i + 1)) {
					return true;
				}
				--currSteps;	// didn't reach target, undo steps
				inputVals = oldInput;
			}
			else if (output == 2) {	// reached target location
				return true;
			}
		}
	}

	for (int i = 2; i <= 4; i += 2) {	// south and east
		if (i != lastDir) {
			std::vector<long> oldInput = inputVals;
			output = processInput (inputVals, i);
//			std::cout << i << " " << output << " " << currSteps << "\n";
			if (output == 1) {	// last direction given by i - 1
				if (runMaze (inputVals, ++currSteps, i - 1)) {
					return true;
				}
				--currSteps;	// didn't reach target, undo steps
				inputVals = oldInput;
			}
			else if (output == 2) {	// reached target location
				return true;
			}
		}
	}

	return false;
}

/*
 * Function class for hash of pairs (x, y) of visited points for BFS
 */
class HashCoords {
public:
	std::size_t operator()(const std::pair<int, int> &p) const {
		return p.first * 31 + p.second;
	}
};

/*
 * Helper function to update coordinates (x, y)
 */
void updatePos (std::pair<int, int> &p, int dir) {
	if (dir == 1) {
		++p.second;
	}
	else if (dir == 2) {
		--p.second;
	}
	else if (dir == 3) {
		--p.first;
	}
	else {	// dir == 4
		++p.first;
	}
}

int runDepth (std::vector<long> inputVals) {
	std::deque<std::pair<std::pair<int, int>, std::vector<long>>> queue;
	std::unordered_map<std::pair<int, int>, int, HashCoords> visited;
	std::pair<int, int> currPos = {0, 0};
	queue.push_back({currPos, inputVals});

	int maxDepth = 0;

	while (queue.size()) {
		currPos = queue.front().first;
		int& currDepth = visited[currPos];

		if (currDepth > maxDepth) {
			maxDepth = currDepth;
		}

		for (int i = 1; i <= 4; ++i) {
			currPos = queue.front().first;
			std::pair<int, int> prevPos = currPos;
			updatePos(currPos, i);
			std::vector<long> currVals = queue.front().second;
			long output = processInput(currVals, i);

			if (output && !visited[currPos]) {
				queue.push_back({currPos, currVals});
				visited[currPos] = visited[prevPos] + 1;
//				std::cout << currPos.first << " " << currPos.second << " " << visited[currPos] << std::endl;
			}
		}
		queue.pop_front();
	}

	return maxDepth;
}

long processInput (std::vector<long> &inputVals, long input) {
    // iterate through inputs individually; opcodes not at fixed positions
    int i = 0;
    int relativeBase = 0;
    long output = -99999;
    while (i < inputVals.size ()) {
        // execute the opcode with helper function
//        printf("running opcode with index %d\n", i);
        int offset = runOpcode (inputVals, i, input, output, relativeBase);
        if (output != -99999) {
            return output;
        }
        i += offset;
    }
    return output;	// -99999
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
//            std::cout << "output: " << val1 << "\n";
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
