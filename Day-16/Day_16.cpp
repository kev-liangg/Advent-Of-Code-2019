/*
 * Day 16: perform repeated operations on a list of numbers using a given
 * pattern: at every step, generate a new list of numbers using an accumulate
 * algorithm
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <deque>

/*
 * Apply the transformation pattern on the list of numbers
 */
void transformList (std::vector<int> &list);

/*
 * Generate the pattern for the applied transformation for each position
 */
void generatePattern (std::vector<int> &pattern, size_t pos);

int main () {
	// generate initial list by parsing complete string
	std::ifstream inFile ("input.txt");
	std::string input;
	std::getline (inFile, input);
	std::vector<int> list;
	list.reserve (input.size());
	for (char s : input) {
		list.push_back(s - 48);
	}
}

void transformList (std::vector<int> &list) {
	// generate all n values of the new list
	std::vector<int> temp;
	temp.reserve(list.size());

	int newVal;
	std::vector<int> pattern;
	pattern.reserve(list.size());
	for (size_t i = 0; i < list.size(); ++i) {
		generatePattern(pattern, i);
		// todo: generate the new val using accumulate+pattern
		temp.push_back(newVal);
	}
}

void generatePattern (std::vector<int> &pattern, size_t pos) {
	// base pattern: 0, 1, 0, -1
	int base[] = {0, 1, 0, -1};
	std::deque<int> temp;
	size_t i = 0;
	int baseIndex = 0;
	while (i < pattern.size()) {
		// expand base pattern from position
		for (size_t j = 0; j <= pos; ++j) {
			temp.push_back(base[baseIndex % 4]);
			++i;
		}
		++baseIndex;
	}
	temp.pop_front();
	std::copy (temp.begin(), temp.end(), pattern.begin());
}
