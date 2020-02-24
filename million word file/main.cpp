//main.cpp
//Blake Caldwell
//Febuary 22, 2020
//parses a million word file and tallies 50 specific words

#include <iostream>
using std::cout;
using std::endl;

#include <vector>
using std::vector;

#include <fstream>
using std::ifstream;

#include <string>
using std::string;

#include <chrono>

#include <future>

#include <algorithm>
using std::sort;
using std::lower_bound;

#include <map>
using std::map;

#include <iomanip>

vector<vector<int>> allThreadTotal;

void reset() {
	for (auto a: allThreadTotal)
		allThreadTotal.pop_back();
}

void checkWord(const string& word, const vector<string>& words, vector<int>& count) {
	auto iter = lower_bound(words.begin(), words.end(), word);
	if (iter != words.end() && (*iter).size() == word.size()) {
		int index = iter - words.begin();
		count[index] += 1;
	}
}

template <typename Iter> void check(Iter first, Iter last, const vector<string>& words) {
	vector<int> count(50, 0);
	for (Iter i = first; i != last; ++i) {
		checkWord(*i, words, count);
	}
	allThreadTotal.push_back(count);
}

void dostuff(string fileName){
	ifstream file(fileName);
	if (!file) {
		cout << "Error" << endl;
	}
	map<string, int> actual;
	vector<string> fileVector;

	string input;
	while (true) {
		file >> input;
		if (!file) {
			if (file.eof())
				break;
			else
				cout << "Unknown error" << endl;
		}
		else {
			fileVector.push_back(input);
			if (actual.count(input) != 1)
				actual[input] = 1;
			else
				++actual[input];
		}
	}

	file.close();
	//have one million words in a vector now

	vector<string> words = {"the", "and", "mama", "was", "zebra", "ice", "word", "pie", "teeth", "love",
							"so", "kill", "life", "murder", "pizza", "snake", "all", "his", "her", "happy",
							"color", "have", "stopped", "in", "family", "could", "of", "law", "most", "man",
							"not", "young", "day", "still", "full", "but", "phone", "soda", "rock", "coin",
							"computer", "Alaska", "knife", "tape", "dagger", "our", "i", "question", "name", "those" };

	sort(words.begin(), words.end());

	auto startTime = std::chrono::steady_clock::now();

	vector<std::future<void>> results;
	vector<int> total(50, 0);

	int numThreads = 1;

	for (int i = 0; i < numThreads; ++i) {
		auto numWords = fileVector.size();
		auto start = i / double(numThreads) * numWords + fileVector.begin();
		auto end = (i + 1) / double(numThreads) * numWords + fileVector.begin();

		results.push_back(std::async(std::launch::async, check<vector<string>::const_iterator>, start, end, words));
	}

	for (auto& f : results) {
		f.get();
	}

	for (int i = 0; i < numThreads; ++i) {
		for (int j = 0; j < words.size(); ++j){
			total[j] += allThreadTotal[i][j];
		}
	}

	auto endTime = std::chrono::steady_clock::now();
	auto diff = endTime - startTime;

	for (int i = 0; i < words.size(); ++i) {
		cout << std::setw(8) << words[i] << " = " << total[i] << endl;
	}

	cout << "It took: " << std::chrono::duration<double>(diff).count() << " seconds for " << numThreads << " thread(s) to complete." << endl;
}

int main() {
	dostuff("fiction.txt");
	cout << endl;
	reset();
	dostuff("academic.txt");
}