#include <iostream>
#include <string>
#include <chrono>
#include "benchmark.h"
#include "utils.h"


BenchmarkTest runBenchmark(std::string command, int id, int iterationsCount) {
	std::cout << "\e[1mBenchmark #" << id << ":\e[0m " << command << std::endl;
	
	std::future<BenchmarkTest> test = Benchmark::testAsync(command, iterationsCount);
	utils::LoadingIndicator* loadingIndicator = new utils::LoadingIndicator();

	std::chrono::steady_clock::time_point testBegin = std::chrono::steady_clock::now();

	std::cout << loadingIndicator->render();

	while (test.wait_for(std::chrono::milliseconds(100)) != std::future_status::ready) {
		std::cout << loadingIndicator->next();
	}

	std::cout << loadingIndicator->purge();
	test.wait();

	std::chrono::nanoseconds timeElapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(
		std::chrono::steady_clock::now() - testBegin
	);

	BenchmarkTest results = test.get();
	
	std::cout << "Time elapsed:\t\t± " << "\e[1m\x1B[92m" << utils::formatTime(timeElapsed);
	std::cout << "\e[0m\x1B[90m " << iterationsCount << " runs\033[0m" << std::endl;

	if (iterationsCount > 1) {
		std::cout << "Average:\t\t" << "\e[1m\x1B[92m" << utils::formatTime(results.average);
		std::cout << "\e[0m\033[0m" << std::endl;

		std::cout << "Median:\t\t\t" << "\e[1m\x1B[92m" << utils::formatTime(results.median);
		std::cout << "\e[0m\033[0m" << std::endl;

		std::cout << "Range (\e[1m\x1B[96mmin \e[0m\033[0m… \e[1m\x1B[93mmax\e[0m\033[0m):\t";
		std::cout << "\e[1m\x1B[96m" << utils::formatTime(results.fastest) << "\e[0m\033[0m";
		std::cout << " … ";
		std::cout << "\e[1m\x1B[93m" << utils::formatTime(results.slowest) << "\e[0m\033[0m";
		std::cout << std::endl;
	}

	std::cout << std::endl;

	return results;
}

bool isNumber(const std::string& s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

int main(int argc, char* argv[]) {
	int id = 1;
	int iterationsCount = 1;

	for (int i = 1; i < argc; i++) {
		std::string arg = std::string(argv[i]);
		
		if (arg[0] == '-' && (i + 1) != argc) {
			std::string numberPart = arg.substr(1, arg.length());

			if (isNumber(numberPart)) {
				iterationsCount = std::stoi(numberPart);
			}

			continue;
		} else {
			runBenchmark(arg, id, iterationsCount);
			iterationsCount = 1;
			id++;
		}
	}

	return 0;
}

