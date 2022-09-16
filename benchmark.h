#include <string>
#include <vector>


class BenchmarkTest {
	public:
		std::string alias;
		std::vector<float> iterationsTime;

		BenchmarkTest(std::string alias, std::vector<int> iterationsTime);

		static std::vector<BenchmarkTest> compareByMedian(std::vector<BenchmarkTest> tests);
		static std::vector<BenchmarkTest> compareByAverage(std::vector<BenchmarkTest> tests);
		static std::vector<BenchmarkTest> compareByFastest(std::vector<BenchmarkTest> tests);
		static std::vector<BenchmarkTest> compareBySlowest(std::vector<BenchmarkTest> tests);

		static BenchmarkTestComparison compare(std::vector<BenchmarkTest> tests);

		float median();
		float average();
		float fastest();
		float slowest();
};

class Benchmark {
	public:
		std::string command;

		Benchmark(std::string command);

		BenchmarkTest test(int iterations);
	private:
		float execute();
};

