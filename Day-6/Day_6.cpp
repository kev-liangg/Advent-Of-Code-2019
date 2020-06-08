/*
 * Day 6: Construct a tree-like structure to represent a map of direct and
 * indirect orbits of objects orbiting objects
 *
 * If A orbits B, and B orbits C, then A also orbits C. Part 1: Output the total
 * number of pairwise orbits within the total structure.
 */

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>
#include <fstream>
#include <limits>

/*
 * each object contains a list of all the other objects that directly orbit it.
 * an object also stores the object that it directly orbits.
 */
struct Object {
    std::vector<Object*> orbits;
    Object *orbiting;
    // part 2: searching across objects, needs flag to check if already searched
    bool searched;
};

/*
 * attempts to find memory address of an object specified by its name.
 * if object does not exist yet, allocates a new object with that name.
 *
 * returns the pointer of the object under the name.
 */
Object* findObject (std::unordered_map<std::string, Object*> &objects,
                  std::string &objectName);

/*
 * recursively totals the number of direct and indirect orbits around a single
 * object
 */
int countOrbits (Object *base);

/*
 * recursive backtracking solution to determine the shortest distance between
 * any two objects, determined by number of intermediate objects between
 */
int search (Object *start, Object *target, int currDistance);

int main () {
    // mapping of an object's name to the location of its corresponding struct
    std::unordered_map<std::string, Object*> objects;

    std::ifstream inFile ("input.txt");
    std::string line;
    // part 2: need to identify starting position and target position
    Object *start = nullptr;
    Object *target = nullptr;
    while (std::getline (inFile, line)) {
        // object on the right directly orbits object on the left
        std::string base = line.substr (0, 3);
        std::string orbiter = line.substr (4, 3);
        Object *objectBase = findObject (objects, base);
        Object *objectOrbiter = findObject (objects, orbiter);
        // link the two objects by their direct orbit
        objectBase->orbits.push_back (objectOrbiter);
        objectOrbiter->orbiting = objectBase;
        // check for start/target names; only consider base object in this case
        if (orbiter == "SAN") {
            target = objectBase;
        }
        if (orbiter == "YOU") {
            start = objectBase;
        }
    }

    /* Part 1: -------------------------------------------------------------- */

    int totalOrbits = 0;
    // must call counting function starting from every object in the map
    for (std::pair<std::string, Object*> entry : objects) {
        totalOrbits += countOrbits (entry.second);
    }
    // counted all direct/indirect orbits around every object
    printf ("Part 1 Solution: %d\n", totalOrbits);

    /* Part 2: -------------------------------------------------------------- */

    // search for shortest path between start and target objects
    printf ("Part 2 Solution: %d\n", search (start, target, 0));
}

int countOrbits (Object *base) {
    int numDirect = base->orbits.size ();
    // count indirect orbits by calling recursively on all direct orbits
    int numIndirect = 0;
    for (Object *direct : base->orbits) {
        numIndirect += countOrbits (direct);
    }
    return numDirect + numIndirect;
}

int search (Object *current, Object *target, int currDistance) {
    // check if reached target
    if (current == target) {
        return currDistance;
    }
    // mark current object as visited
    current->searched = true;
    // check orbiting object, if not null
    int currMin = std::numeric_limits<int>::max ();
    if (current->orbiting != nullptr && !current->orbiting->searched) {
        currMin = search (current->orbiting, target, currDistance + 1);
    }
    // check all orbiting objects
    for (Object *orbiter : current->orbits) {
        if (!orbiter->searched) {
            int result = search (orbiter, target, currDistance + 1);

            if (result < currMin) {
                currMin = result;
            }
        }
    }
    return currMin;
}

Object* findObject (std::unordered_map<std::string, Object*> &objects,
                    std::string &objectName) {
    // iterator for unordered map lookup
    std::unordered_map<std::string, Object*>::const_iterator search;
    search = objects.find (objectName);

    Object *pObject;
    if (search == objects.end ()) {
        pObject = new Object;
        objects.insert (std::make_pair(objectName, pObject));
    }
    else {
        pObject = search->second;
    }
    return pObject;
}
