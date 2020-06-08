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

/*
 * each object contains a list of all the other objects that directly orbit it.
 * an object also stores the object that it directly orbits.
 */
struct Object {
    std::vector<Object*> orbits;
    Object *orbiting;
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
 * recursively determines the center of mass object, the one object that does
 * not orbit any other object. takes any object in the list of input.
 */
Object* findRoot (Object *child);

int main () {
    // mapping of an object's name to the location of its corresponding struct
    std::unordered_map<std::string, Object*> objects;

    std::ifstream inFile ("input.txt");
    std::string line;
    while (std::getline (inFile, line)) {
        // object on the right directly orbits object on the left
        std::string base = line.substr (0, 3);
        std::string orbiter = line.substr (4, 3);
        Object *objectBase = findObject (objects, base);
        Object *objectOrbiter = findObject (objects, orbiter);
        // link the two objects by their direct orbit
        objectBase->orbits.push_back (objectOrbiter);
        objectOrbiter->orbiting = objectBase;
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

Object* findRoot (Object *child) {
    // simple recursive call from child objects until highest parent is reached
    if (child->orbiting == nullptr) {
        return child;
    }
    else {
        return findRoot (child->orbiting);
    }
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
