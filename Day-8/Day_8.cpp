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
    int minZeros = WIDTH * HEIGHT;
    int indexMin = 0;
    for (unsigned int i = 0; i < layers.size (); i++) {
        std::string layer = layers.at (i);
        std::cout << layer << "\n";
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

    // solution: number of 1 digits multiplied by number of 2 digits
    int numOnes = 0;
    int numTwos = 0;
    std::string targetLayer = layers.at (indexMin);
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
}



