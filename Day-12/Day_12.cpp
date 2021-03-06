/*
 * Day 12: Simulate the motion of 4 moons given initial positions and formulas
 * for gravity, motion, and energy at each step
 */

#include <cmath>
#include <vector>
#include <fstream>
#include <iostream>
#include <numeric>

struct Moon {
    // position: x, y, z
    int pos[3];
    // velocity: dX, dY, dZ
    int vel[3];
};

/*
 * step functions to calculate gravity, velocity, and update position
 *
 * first function updates the velocity while preserving original positions,
 * requires a reference to another moon pairing.
 *
 * when all velocities calculated, safe to update position of each moon using
 * the second function.
 */
void stepVel (Moon &base, Moon &ref);
void stepPos (Moon &base);

/*
 * wrapper function for both step commands, steps by fixed number of steps
 */
void step (std::vector<Moon> &moons, int numSteps);

/*
 * calculate and return the energy of an individual moon.
 * get both absolute sums of its position and velocity, then multiply the sums
 */
int calcEnergy (Moon base);

/*
 * Part 2: check equality between two moon positions to identify repeat
 */
bool compare (Moon m1, Moon m2, int indexField);

int main () {
    // input not consistently formatted for starting points, enter manually
    //         x    y    z    velocity starts 0
    Moon m1 { {-7,  -8,  9}, {0, 0, 0} };
    Moon m2 { {-12, -3, -4}, {0, 0, 0} };
    Moon m3 { { 6, -17, -9}, {0, 0, 0} };
    Moon m4 { { 4, -10, -6}, {0, 0, 0} };

    std::vector<Moon> moons;
    moons.push_back (m1);
    moons.push_back (m2);
    moons.push_back (m3);
    moons.push_back (m4);

    /* Part 1: -------------------------------------------------------------- */

    // step 1000 times, then calculate total energy
    step (moons, 1000);

    int total = 0;
    for (Moon &moon : moons) {
        total += calcEnergy (moon);
    }
    printf ("Part 1 Solution: %d\n", total);

    /* Part 2: -------------------------------------------------------------- */

    moons.clear ();
    moons.push_back (m1);
    moons.push_back (m2);
    moons.push_back (m3);
    moons.push_back (m4);

    // calculate number of steps for each position to repeat: x, y, and z
    // step one at a time, and monitor for repeats on x, y, and z

    int cycles[] = {0, 0, 0};
    int numSteps = 0;
    // finished when all cycle lengths assigned a value
    while (!(cycles[0] && cycles[1] && cycles[2])) {
        step (moons, 1);
        numSteps++;
        // check x, y, and z fields for repeat
        for (int i = 0; i < 3; i++) {
            // compare all the moons to their starting positions
            if (compare (moons.at (0), m1, i) &&
                compare (moons.at (1), m2, i) &&
                compare (moons.at (2), m3, i) &&
                compare (moons.at (3), m4, i) &&
                !cycles[i]) {
                // record the cycle for corresponding field
                cycles[i] = numSteps;
            }
        }
    }

    printf ("Part 2 Solution: LCM of %d, %d, %d\n",
            cycles[0], cycles[1], cycles[2]);
}

void step (std::vector<Moon> &moons, int numSteps) {
    // iterate for numSteps
    for (int num = 0; num < numSteps; num++) {
        // call velocity step on all pairs of moons
        for (int i = 0; i < (int)moons.size(); i++) {
            for (int j = 0; j < (int)moons.size(); j++) {
                Moon &base = moons.at (i);
                Moon &ref = moons.at (j);
                stepVel (base, ref);
            }
        }

        // call position step on all moons
        for (Moon &moon : moons) {
            stepPos (moon);
        }
    }
}

void stepVel (Moon &base, Moon &ref) {
    // iterate through x, y, z fields
    for (int i = 0; i < 3; i++) {
        // update velocities, based on difference in axes
        int delta = 0;
        if (base.pos[i] > ref.pos[i]) {
            delta = -1;
        }
        else if (base.pos[i] < ref.pos[i]) {
            delta = 1;
        }
        base.vel[i] += delta;
    }
}

void stepPos (Moon &base) {
    // iterate through x, y, z fields
    for (int i = 0; i < 3; i++) {
        base.pos[i] += base.vel[i];
    }
}

int calcEnergy (Moon base) {
    int totalPos = 0;
    int totalVel = 0;
    // iterate through x, y, z fields
    for (int i = 0; i < 3; i++) {
        totalPos += std::abs (base.pos[i]);
        totalVel += std::abs (base.vel[i]);
    }
    return totalPos * totalVel;
}

bool compare (Moon m1, Moon m2, int indexField) {
    if (m1.pos[indexField] != m2.pos[indexField]) {
        return false;
    }
    if (m1.vel[indexField] != m2.vel[indexField]) {
        return false;
    }
    return true;
}
