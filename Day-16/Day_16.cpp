/*
 * Day 16: perform repeated operations on a list of numbers using a given
 * pattern: at every step, generate a new list of numbers using an accumulate
 * algorithm
 */

#include <iostream>
#include <fstream>
#include <vector>

/*
 * Apply the transformation pattern on the list of numbers
 */
void transformList (std::vector<short> &list);

int main () {
	// generate initial list by parsing complete string
	std::ifstream inFile ("input.txt");
	std::string input;
	std::getline (inFile, input);
	std::vector<short> list;
	list.reserve (input.size());
	for (char s : input) {
		list.push_back(s - 48);
	}
}

void transformList (std::vector<short> &list) {
	// generate all n values of the new list
	std::vector<short> temp;
	temp.reserve(list.size());
	short newVal;
	for (int i = 0; i < list.size(); ++i) {
		// todo: generate the new val using accumulate+pattern
		temp.push_back(newVal);
	}
}
