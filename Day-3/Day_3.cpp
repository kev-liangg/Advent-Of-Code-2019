/*
 * Day 3: Given the magnitudes of two wire paths from the same origin point,
 * determine the Manhattan distance between the origin point and the nearest
 * point of intersection between the paths.
 */

#include <fstream>
#include <sstream>
#include <iostream>
#include <limits>
#include <vector>

/*
 * Represent the position of each vertex in the path, will always be a corner
 * in the grid in this case. Store address of next vertex in the path.
 */
struct Vertex {
    int x;
    int y;
    Vertex *next;
};

/*
 * Get the next vertex from the input element, containing direction and velocity
 */
Vertex* getNextVertex (std::string &inputElem, Vertex *vertexLast);

/*
 * Determine intersection between two line segments given by begin and end
 * vertices, and return the Manhattan distance from the origin (0, 0)
 */
int intersectDistance (Vertex *b1, Vertex *e1, Vertex *b2, Vertex *e2);

int main () {
    std::ifstream inFile ("input.txt");
    // Two paths in input case, but generalize for any number of paths
    // store pointers to the start of each path parsed from input
    std::vector<Vertex*> vertexStart;
    std::string inputLine;
    while (std::getline (inFile, inputLine)) {
        // initialize temporary vertex for starting point
        Vertex *vertexTemp = new Vertex {0, 0, NULL};
        vertexStart.push_back (vertexTemp);

        // separate each input elem, delimited by commas
        std::string inputElem;
        std::istringstream inStream (inputLine);
        while (std::getline (inStream, inputElem, ',')) {
            // create new vertex and link to last temporary vertex
            Vertex *vertexNew = getNextVertex (inputElem, vertexTemp);
            printf("(%d, %d)\n", vertexNew->x, vertexNew->y);
            vertexTemp->next = vertexNew;
            vertexTemp = vertexNew;
        }
    }

    /* Part 1: -------------------------------------------------------------- */
    // double nested loop across first two paths to determine all intersects
    Vertex *vertex1 = vertexStart.at (0);
    Vertex *vertex2 = vertexStart.at (1);
    int minDistance = std::numeric_limits<int>::max ();
    bool first = true;
    while (vertex1->next != NULL) {
        vertex2 = vertexStart.at (1);
        while (vertex2->next != NULL) {
            int result = intersectDistance (vertex1, vertex1->next,
                                            vertex2, vertex2->next);
            // dirty fix: ignore 0 result from initial intersect
            if (!first && result < minDistance) {
                minDistance = result;
            }
            vertex2 = vertex2->next;
            first = false;
        }
        vertex1 = vertex1->next;
    }

    printf ("Part 1 Solution: %d\n", minDistance);

    /* Part 2: -------------------------------------------------------------- */
    // reuse code from part 1, but early return at "first" intersect
    // this will be the earliest point, as all vertices stored in relative order
    minDistance = std::numeric_limits<int>::max ();
    first = true;
    bool done = false;
    vertex1 = vertexStart.at (0);
    // track displacement for path 1
    int totalSteps1 = 0;
    while (vertex1->next != NULL && !done) {
        // track displacement for path 2
        int totalSteps2 = 0;
        vertex2 = vertexStart.at (1);
        while (vertex2->next != NULL && !done) {
            int result = intersectDistance (vertex1, vertex1->next,
                                            vertex2, vertex2->next);
            // dirty fix: ignore 0 result from initial intersect
            if (!first && result < minDistance) {
                // case 1: path 1 is vertical. path 1 gives x, path 2 gives y
                if (vertex1->y == vertex1->next->y) {
                    totalSteps2 += std::abs (vertex1->x - vertex2->x);
                    totalSteps1 += std::abs (vertex2->y - vertex1->y);
                }
                // case 2: path 2 is vertical. path 2 gives x, path 1 gives y
                else {
                    totalSteps1 += std::abs (vertex2->x - vertex1->x);
                    totalSteps2 += std::abs (vertex1->y - vertex2->y);
                }
                // combine the steps for path 1 and 2, for solution
                printf("total steps path 1: %d\n", totalSteps1);
                totalSteps1 += totalSteps2;
                done = true;
            }
            printf("total steps path 2: %d\n", totalSteps2);
            if (!done) {
                // update displacement before moving to next vertex
                totalSteps2 += std::abs (vertex2->x - vertex2->next->x);
                totalSteps2 += std::abs (vertex2->y - vertex2->next->y);

                vertex2 = vertex2->next;
                first = false;
            }
        }
        if (!done) {
            totalSteps1 += std::abs (vertex1->x - vertex1->next->x);
            totalSteps1 += std::abs (vertex1->y - vertex1->next->y);
            vertex1 = vertex1->next;
        }
    }

    printf ("Part 2 Solution: %d\n", totalSteps1);

    /* Both parts completed: Clean up allocated memory ---------------------- */
}

Vertex* getNextVertex (std::string &inputElem, Vertex *vertexLast) {
    // parse direction and magnitude from input elem
    char direction = inputElem [0];
    int magnitude = std::stoi (inputElem.substr (1));

    // update x and y from directions: (U)p, (D)own, (L)eft, (R)ight
    int newX = vertexLast->x;
    int newY = vertexLast->y;
    if (direction == 'U') {
        newY += magnitude;
    }
    else if (direction == 'D') {
        newY -= magnitude;
    }
    else if (direction == 'L') {
        newX -= magnitude;
    }
    else if (direction == 'R') {
        newX += magnitude;
    }
    else {
        printf("Invalid direction, input may be invalid!\n");
    }

    // allocate new vertex and return
    return new Vertex {newX, newY, NULL};
}

// helper function to determine if valTest is between val1 and val2
bool between (int valTest, int val1, int val2) {
    int max = std::max(val1, val2);
    int min = std::min(val1, val2);
    return (unsigned)(valTest - min) <= (max - min);
}

int intersectDistance (Vertex *b1, Vertex *e1, Vertex *b2, Vertex *e2) {
    // path 1 horizontal, path 2 vertical
    if (b1->y == e1->y && b2->x == e2->x) {
        // vertical x values must be between two horizontal x values, vice versa
        if (between (b2->x, b1->x, e1->x) && between (b1->y, b2->y, e2->y)) {
            printf("intersect: %d, %d\n", b2->x, b1->y);
            printf("path 1 horizontal, begin x %d\n", b1->x);
            printf("path 2 vertical, begin y %d\n", b2->y);
            // intersect point given by path 1 y coord, path 2 x coord
            return std::abs (b2->x) + std::abs (b1->y);
        }
    }
    // path 2 horizontal, path 1 vertical
    else if (b2->y == e2->y && b1->x == e1->x) {
        // vertical x values must be between two horizontal x values, vice versa
        if (between (b1->x, b2->x, e2->x) && between (b2->y, b1->y, e1->y)) {
            printf("intersect: %d, %d\n", b1->x, b2->y);
            // intersect point given by path 2 y coord, path 1 x coord
            return std::abs (b2->y) + std::abs (b1->x);
        }
    }
    // did not early return with a distance, return max int value
    return std::numeric_limits<int>::max ();
}
