/*
 * Day 10: Calculate the line of sight between objects based on position
 * and angles of every object
 */

#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <map>
#include <math.h>

#define PI 3.14159265

/*
 * Coordinate point representing an object's position
 */
struct Point {
    int x;
    int y;
};

/*
 * count the visible asteroids in line of sight from location at origin
 */
int countVisible (std::vector<Point> &coords, Point &origin);

/*
 * destroy the asteroids in clockwise order and line of sight
 */
void destroyAsteroids (std::vector<Point> coords, Point &origin);

int main () {
    std::vector<Point> coords;
    std::ifstream inFile ("input.txt");
    // parse input into points
    std::string line;
    int row = 0;
    while (std::getline (inFile, line)) {
        for (int col = 0; col < (int)line.size (); col++) {
            char obj = line[col];
            if (obj == '#') {
                coords.push_back (Point {col, row});
            }
        }
        row++;
    }

    /* Part 1: -------------------------------------------------------------- */

    // record the max number of visible objects from each object
    int currMax = 0;
    Point bestLocation;
    for (Point origin : coords) {
        int result = countVisible (coords, origin);
        if (result > currMax) {
            currMax = result;
            bestLocation = origin;
        }
    }

    printf ("Part 1 Solution: %d\n", currMax);

    /* Part 2: -------------------------------------------------------------- */

    // "destroy" the asteroids in clockwise order, determine the 200th location
    destroyAsteroids (coords, bestLocation);
}

int countVisible (std::vector<Point> &coords, Point &origin) {
    // record the slopes, along with a separate flag to distinguish magnitude
    std::unordered_set<float> angles;
    for (Point target : coords) {
        int diffX = target.x - origin.x;
        int diffY = target.y - origin.y;
        // check if comparing same point with each other; both can't be zero
        if (diffX || diffY) {
            // calculate angle
            float angle = atan2 (diffY, diffX);
            angles.insert (angle);
        }
    }
    return angles.size ();
}

// helper function to destroy asteroids by initializing sorted set of angles
void setAngles (std::vector<Point> &coords, Point &origin,
                std::map<float, std::vector<Point>> &angles) {
    for (Point target : coords) {
        int diffX = target.x - origin.x;
        int diffY = target.y - origin.y;
        // check if comparing same point with each other; both can't be zero
        if (diffX || diffY) {
            // calculate angle and normalize to degrees 0-360, 0 up
            float angle = atan2 (diffY, diffX) * 180 / PI + 90.1;
            // negative case, wrap around 360 degrees
            if (angle < 0) {
                angle += 360;
            }
            // insert into map
            std::vector<Point> toAdd;
            std::pair<std::map<float, std::vector<Point>>::iterator, bool> ret;
            ret = angles.insert (std::pair<float, std::vector<Point>>
                                 (angle, toAdd));
            // increment number of points at that angle
            ret.first->second.push_back (target);
        }
    }
}

// helper function to get distance between two points
float getDistance (Point &p1, Point &p2) {
    return sqrt (((p2.x - p1.x)^2) + ((p2.y - p1.y)^2));
}

void destroyAsteroids (std::vector<Point> coords, Point &origin) {
    // sorted map to iterate through angles, and number of points at angle
    std::map<float, std::vector<Point>> angles;
    setAngles (coords, origin, angles);

    // keep track of the number of destroyed asteroids
    int n = 0;
    while (n < coords.size ()) {
        // in-order iteration from 0 degrees to 360
        for (std::pair<float, std::vector<Point>> entry : angles) {
            if (entry.second.size () > 0) {
                // remove the closest point
                int minDistance = 99999;
                int indexMin = 0;
                for (int i = 0; i < (int)entry.second.size (); i++) {
                    Point target = coords.at (i);
                    float distance = getDistance (origin, target);
                    if (distance < minDistance) {
                        minDistance = distance;
                        indexMin = i;
                    }
                }
                n++;
                if (n == 200) {
                    Point answer = entry.second.at (indexMin);
                    printf ("Part 2 Solution: %d\n", answer.x * 100 + answer.y);
                }
                entry.second.erase (entry.second.begin () + indexMin);
            }

        }
    }
}
