/*
 * Day 6: Construct a tree-like structure to represent a map of direct and
 * indirect orbits of objects orbiting objects
 *
 * If A orbits B, and B orbits C, then A also orbits C. Part 1: Output the total
 * number of pairwise orbits within the total structure.
 */

#include <unordered_map>
#include <vector>
#include <iostream>
#include <fstream>

/*
 * each object contains a list of all the other objects that directly orbit it.
 */
struct Object {
    std::vector<Object*> orbits;
};

/*
 * attempts to find memory address of an object specified by its name.
 * if object does not exist yet, allocates a new object with that name.
 *
 * returns the pointer of the object under the name.
 */
Object* findObject (std::unordered_map<std::string, Object*> &objects,
                  std::string objectName, Object *pResult);

int main () {
    // mapping of an object's name to the location of its corresponding struct
    std::unordered_map<std::string, Object*> objects;

    std::ifstream inFile ("input.txt");
    std::string line;
    while (std::getline (inFile, line)) {
        // object on the right directly orbits object on the left
        std::string base = line.substr (0, 3);
        std::string orbiter = line.substr (4, 7);

        Object *toAddBase;
        Object *toAddOrbiter;

    }
}

Object* findObject (std::unordered_map<std::string, Object*> &objects,
                  std::string objectName) {
    // iterator for unordered map lookup
    std::unordered_map<std::string, Object*>::const_iterator search;
    objects.find (objectName);

    Object *pObject;
    if (search == objects.end ()) {
        pObject = new Object;
    }
    else {
        pObject = search->second;
    }
    return pObject;
}
