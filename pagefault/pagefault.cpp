
#include <vector>
using std::vector;

#include <iostream>
using std::cout;
using std::endl;

#include <chrono>




void pagefault() {
	
	vector<int> largeVector(3'500'000'000,0);

	cout << "First iteration: " << endl;

	for (int i = 0; i < largeVector.size(); i += 500'000'000) {
		auto startTime = std::chrono::steady_clock::now();
		largeVector[i] = i;
		auto endTime = std::chrono::steady_clock::now();
		auto diff = endTime - startTime;
		cout << "Access number: " << i / 500'000'000 + 1 << " Time taken: " << std::chrono::duration<double>(diff).count() << endl;
	}

	cout << endl << "Second iteration: " << endl;

	for (int i = 0; i < largeVector.size(); i += 500'000'000) {
		auto startTime = std::chrono::steady_clock::now();
		largeVector[i] += i;
		auto endTime = std::chrono::steady_clock::now();
		auto diff = endTime - startTime;
		cout << "Access number: " << i / 500'000'000 + 1 << " Time taken: " << std::chrono::duration<double>(diff).count() << endl;
	}

	cout << largeVector[3000000000];
}

int main() {
	pagefault();

	return 0;
}