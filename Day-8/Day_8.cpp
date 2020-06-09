/*
 * Day 8: Process an input representing the layers of an image.
 *
 * Inputs: Image is 25 pixels wide, 6 pixels tall
 */

#include <iostream>
#include <fstream>
#include <vector>

#define WIDTH 25
#define HEIGHT 6

/*
 * find the index/layer with the fewest number of zeros
 */
int indexFewestZeros (std::vector<std::string> layers);

int main () {
    std::ifstream inFile ("input.txt");
    std::string input;
    std::getline (inFile, input);

    // separate input into layers, each 25 x 6 entries: total 150 each
    std::vector<std::string> layers;
    for (unsigned int i = 0; i < input.size (); i += WIDTH * HEIGHT) {
        layers.push_back (input.substr (i, WIDTH * HEIGHT));
    }

    /* Part 1: -------------------------------------------------------------- */

    // find layer with fewest 0 digits
    int index = indexFewestZeros (layers);

    // solution: number of 1 digits multiplied by number of 2 digits
    int numOnes = 0;
    int numTwos = 0;
    std::string targetLayer = layers.at (index);
    for (unsigned int i = 0; i < targetLayer.size (); i++) {
        int entry = (int)targetLayer[i] - 48;
        if (entry == 1) {
            numOnes ++;
        }
        else if (entry == 2) {
            numTwos ++;
        }
    }

    printf ("Part 1 Solution: %d\n", numOnes * numTwos);

    /* Part 2: -------------------------------------------------------------- */

   // iterate across every position to determine output message
    std::string output;
    for (unsigned int i = 0; i <= WIDTH * HEIGHT; i++) {
        if (i % WIDTH == 0) {
            output.append ("\n");
        }
        // determine topmost value: iterate down from top layer, given by j = 0
        char topValue = '2';
        for (int j = 0; j < layers.size(); j++) {
            // 2 is transparent, find first occurrence of 0 or 1
            std::string currLayer = layers.at (j);
            if (((int)currLayer[i] - 48) != 2) {
                topValue = currLayer[i] == '1' ? '#' : '.';
                break;
            }
        }
        output.append (1, topValue);
    }

    std::cout << "\Part 2 Solution:\n" << output << "\n";

}

int indexFewestZeros (std::vector<std::string> layers) {
    int minZeros = WIDTH * HEIGHT;
    int indexMin = 0;
    for (unsigned int i = 0; i < layers.size (); i++) {
        std::string layer = layers.at (i);
        int numZeros = 0;
        for (unsigned int j = 0; j < layer.size (); j++) {
            if (!((int)layer[j] - 48)) {
                numZeros++;
            }
        }
        // update stored minimum
        if (numZeros < minZeros) {
            minZeros = numZeros;
            indexMin = i;
        }
    }
    return indexMin;
}



