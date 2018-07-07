#include <iostream>
#include <array>
#include <algorithm>
#include <string>
#include <cstdlib>
#include<time.h>
#include<stdio.h>
#include <fstream>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

class Algorithm {
public:
	static int algorithm1(int X[]);
	static int algorithm2(int X[]);
	static int algorithm3(int X[], int L, int U);
	static int algorithm4(int X[]);
};

// reads from phw_intput.txt
class Input {
public:
	static int inputArray[];
	static int size;
};
int Input::size = 0;

class RandomGenerator {
public:
	static int* generateNewArray(int size);
	static long findAverageTime(int* array, int size, int algorithm);
	static int findTheoreticalTime(int size, int algorithm);
};
//int* randomArrays[19] = { 0 };

class FileReaderWriter {
public:
	static void createHeader();
	static int* readFile(string fileName);
	static void writeToFile(long output);
};

//TODO: Find Theoretical complexities, write algorithm that generates theoretical answer
int main() {

	srand(time(0));

	// read input file, run list against the algorithms
	int* array = FileReaderWriter::readFile("phw_input.txt");
	cout << "algorithm-1: " << Algorithm::algorithm1(array) << endl;
	cout << "algorithm-2: " << Algorithm::algorithm2(array) << endl;
	cout << "algorithm-3: " << Algorithm::algorithm3(array, 0, Input::size - 1) << endl;
	cout << "algorithm-4: " << Algorithm::algorithm4(array) << endl;

	FileReaderWriter::createHeader();

	// Get 19 arrays of random ints with different sizes
	int* randomArrays[19];
	for (int i = 2; i < 21; i++) {
		randomArrays[i - 2] = RandomGenerator::generateNewArray(i * 5);
	}

	// Finds average time of each algorithm for each size
	long timeAverages[19][4];
	// theoreticalAverages[19][4];
	for (int i = 2; i < 21; i++) {
		timeAverages[i - 2][0] = RandomGenerator::findAverageTime(randomArrays[i - 2], i * 5, 1);     
		timeAverages[i - 2][1] = RandomGenerator::findAverageTime(randomArrays[i - 2], i * 5, 2);
		timeAverages[i - 2][2] = RandomGenerator::findAverageTime(randomArrays[i - 2], i * 5, 3);
		timeAverages[i - 2][3] = RandomGenerator::findAverageTime(randomArrays[i - 2], i * 5, 4);

		/*theoreticalAverages[i - 2][0] = RandomGenerator::findTheoreticalTime(i * 5, 1);
		theoreticalAverages[i - 2][1] = RandomGenerator::findTheoreticalTime(i * 5, 2);
		theoreticalAverages[i - 2][2] = RandomGenerator::findTheoreticalTime(i * 5, 3);
		theoreticalAverages[i - 2][3] = RandomGenerator::findTheoreticalTime(i * 5, 4);*/
	}
	
	// Print answers accordingly in 19x8 matrix
	for (int i = 0; i < 19; i++) {
		FileReaderWriter::writeToFile(timeAverages[i][0]);
		FileReaderWriter::writeToFile(timeAverages[i][1]);
		FileReaderWriter::writeToFile(timeAverages[i][2]);
		FileReaderWriter::writeToFile(timeAverages[i][3]);

		// Should move this
		ofstream outputFile;
		outputFile.open("granthaislip_phw_output.txt", fstream::app);
		outputFile << endl;
		outputFile.close();

		/*FileReaderWriter::writeToFile(timeAverages[i][0]);
		FileReaderWriter::writeToFile(timeAverages[i][1]);
		FileReaderWriter::writeToFile(timeAverages[i][2]);
		FileReaderWriter::writeToFile(timeAverages[i][3]);
		add endl */
	}

	system("pause");
	return 0;
}

int Algorithm::algorithm1(int X[])
{
	int maxSoFar;
	int sum;

	maxSoFar = 0;
	for (int L = 0; L < Input::size; L++) {
		for (int U = L; U < Input::size; U++) {
			sum = 0;
			for (int I = L; I <= U; I++) {
				sum = sum + X[I];
			}
			maxSoFar = max(maxSoFar, sum);
		}
	}

	return maxSoFar;
}

int Algorithm::algorithm2(int X[])
{
	int maxSoFar = 0;
	for (int L = 0; L < Input::size; L++) {
		int sum = 0;
		for (int U = L; U < Input::size; U++) {
			sum = sum + X[U];
			maxSoFar = max(maxSoFar, sum);
		}
	}

	return maxSoFar;
}

int Algorithm::algorithm3(int X[], int L, int U)
{
	if (L > U) {
		return 0;
	}

	if (L == U) {
		return max(0, X[L]);
	}

	int M = ((L + U) / 2);

	int sum = 0;
	int maxToLeft = 0;
	for (int I = M; I >= L; I--) {     // unsure of >=
		sum = sum + X[I];
		maxToLeft = max(maxToLeft, sum);
	}

	sum = 0;
	int maxToRight = 0;
	for (int I = M + 1; I <= U; I++) { // unsure of <=
		sum = sum + X[I];
		maxToRight = max(maxToRight, sum);
	}

	int maxCrossing = maxToLeft + maxToRight;

	int maxInA = algorithm3(X, L, M);
	int maxInB = algorithm3(X, M + 1, U);
	//return max(maxCrossing, maxInA, maxInB); // returns const int
	if (maxCrossing > maxInA) {
		if (maxCrossing > maxInB) {
			return maxCrossing;
		}
	}

	if (maxInB > maxInA) {
		return maxInB;
	}

	return maxInA;
}

int Algorithm::algorithm4(int X[])
{
	int maxSoFar = 0;
	int maxEndingHere = 0;
	for (int I = 0; I < Input::size; I++) {
		maxEndingHere = max(0, maxEndingHere + X[I]);
		maxSoFar = max(maxSoFar, maxEndingHere);
	}

	return maxSoFar;
}

int* RandomGenerator::generateNewArray(int size) {
	int* newArray = new int[size];
	
	for (int i = 0; i < size; i++) {
		int val = (rand() % 2001) - 1000; // random int from -1000 to 1000
		newArray[i] = val;
	}
	return newArray;
}

int* FileReaderWriter::readFile(string fileName) {
	ifstream file;
	vector<int> numberVector(0);
	string numberIn;

	file.open(fileName);
	if (file.is_open()) {
		while (getline(file, numberIn, ',')) {
			numberVector.push_back(stoi(numberIn));
		}
	}

	Input::size = numberVector.size();

	int* numberArray = new int[Input::size];
	for (int i = 0; i < Input::size; i++) {
		numberArray[i] = numberVector[i];
	}

	return numberArray;
}

long RandomGenerator::findAverageTime(int* array, int size, int algorithm) {
	long timeAverages[19];
	long times[500];
	Input::size = size;

	// Run the algorithm 500 times
	for (int j = 0; j < 500; j++) {
		high_resolution_clock::time_point start;
		high_resolution_clock::time_point end;

		if (algorithm == 1) {
			start = high_resolution_clock::now();
			Algorithm::algorithm1(array);
			end = high_resolution_clock::now();
		}
		else if (algorithm == 2) {
			start = high_resolution_clock::now();
			Algorithm::algorithm2(array);
			end = high_resolution_clock::now();
		}
		else if (algorithm == 3) {
			start = high_resolution_clock::now();
			Algorithm::algorithm3(array, 0, size);
			end = high_resolution_clock::now();
		}
		else if (algorithm == 4) {
			start = high_resolution_clock::now();
			Algorithm::algorithm4(array);
			end = high_resolution_clock::now();
		}

		auto duration = duration_cast<nanoseconds>(end - start).count();
		times[j] = duration;
	}

	//find the average
	long sumOfTimes = 0;
	for (int j = 0; j < 500; j++) {
		sumOfTimes = sumOfTimes + times[j];
	}
	cout << "average: " << (sumOfTimes / 500) << endl;
	return (sumOfTimes / 500);
}

void FileReaderWriter::writeToFile(long output) {
	ofstream outputFile;
	outputFile.open("granthaislip_phw_output.txt", fstream::app);
	outputFile << output << ",";
}

void FileReaderWriter::createHeader() {
	ofstream outputFile;
	outputFile.open("granthaislip_phw_output.txt", fstream::app);
	outputFile << "algorithm-1,algorithm-2,algorithm-3,algorithm-4,T1(n),T2(n),T3(n), T4(n)" << endl;    // change each function once I find theoretical equations
}

int RandomGenerator::findTheoreticalTime(int size, int algorithm) {
	if (algorithm == 1) {

	}
	else if (algorithm == 2) {

	}
	else if (algorithm == 3) {

	}
	else if (algorithm == 4) {

	}

	return 0;
}