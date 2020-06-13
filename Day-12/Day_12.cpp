/*
 * Day 12: Simulate the motion of 4 moons given initial positions and formulas
 * for gravity, motion, and energy at each step
 */

#include <cmath>

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
void stepVel (Moon base, Moon ref);
void stepPos (Moon base);

/*
 * calculate and return the energy of an individual moon, the sum of the
 * absolute values of its position and velocity values
 */
int calcEnergy (Moon base);

int main () {
    // input not consistently formatted for starting points, enter manually
}

void stepVel (Moon base, Moon ref) {
    // iterate through x, y, z fields
    for (int i = 0; i < 3; i++) {
        // update velocities, based on difference in axes
        int delta = 0;
        if (base.pos[i] > ref.pos[i]) {
            delta = 1;
        }
        else if (base.pos[i] < ref.pos[i]) {
            delta = -1;
        }
        base.vel[i] += delta;
    }
}

void stepPos (Moon base) {
    // iterate through x, y, z fields
    for (int i = 0; i < 3; i++) {
        base.pos[i] += base.vel[i];
    }
}

int calcEnergy (Moon base) {
    int total = 0;
    // iterate through x, y, z fields
    for (int i = 0; i < 3; i++) {
        total += std::abs (base.pos[i]) + std::abs (base.vel[i]);
    }
    return total;
}
