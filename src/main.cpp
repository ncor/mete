#include <iostream>
#include <string>
#include <chrono>
#include "benchmark.h"
#include "utils.h"
#include "format.h"


enum Comparison {
    Total,
    Average,
    Median,
    Fastest,
    Slowest
};

BenchmarkTest runBenchmark(std::string command, int id, int iterationsCount) {
    std::cout << format::style::wrap(
            "\nBenchmark #" + std::to_string(id) + ":",
            std::vector<std::string> {
                format::style::decoration::bold,
                format::style::colors::text::intense::yellow
            }
        )
        << " "
        << command
        << std::endl;
	
	std::future<BenchmarkTest> test = Benchmark::testAsync(id, command, iterationsCount);
	format::ProgressBar* progressBar = new format::ProgressBar(format::symbols::braileCycled, "Working");

    std::cout << progressBar->render();

	while (test.wait_for(std::chrono::milliseconds(100)) != std::future_status::ready) {
		std::cout << progressBar->update();
	}

	std::cout << progressBar->purge();
	test.wait();

	BenchmarkTest results = test.get();
	
    std::cout
        << "Total time:\t\t± "
        << format::style::wrap(
            format::time(results.total),
            std::vector<std::string>{ format::style::decoration::bold, format::style::colors::text::intense::blue }
        )
        << format::style::wrap(
            " " + std::to_string(iterationsCount) + " run(s)",
            std::vector<std::string>{ format::style::colors::text::intense::black }
        )
        << std::endl;

	if (iterationsCount > 1) {
        std::cout
            << "Average:\t\t"
            << format::style::wrap(
                format::time(results.average),
                std::vector<std::string>{ format::style::decoration::bold, format::style::colors::text::intense::blue }
            )
            << std::endl;

		std::cout
            << "Median:\t\t\t"
            << format::style::wrap(
                format::time(results.median),
                std::vector<std::string>{ format::style::decoration::bold, format::style::colors::text::intense::blue }
            )
            << std::endl;

        std::cout
            << "Range ("
            << format::style::wrap(
                "min",
                std::vector<std::string>{ format::style::decoration::bold, format::style::colors::text::intense::green }
            )
            << " … "
            << format::style::wrap(
                "max",
                std::vector<std::string>{ format::style::decoration::bold, format::style::colors::text::intense::red }
            )
            << "):\t"
            << format::style::wrap(
                format::time(results.fastest),
                std::vector<std::string>{ format::style::decoration::bold, format::style::colors::text::intense::green }
            )
            << " … "
            << format::style::wrap(
                format::time(results.slowest),
                std::vector<std::string>{ format::style::decoration::bold, format::style::colors::text::intense::red }
            );
	}

	std::cout << "\n";

	return results;
}

std::string compareTable(int compareType, std::vector<BenchmarkTest> results) {
    format::Table* table = new format::Table();

    for (int i = 0; i < results.size(); i++) {
        BenchmarkTest result = results[i];
        std::string flag = "";
        std::string timeColor = format::style::colors::text::intense::yellow;
        std::string displayTime;

        if (compareType == Comparison::Total) {
            displayTime = format::time(result.total);
        } else if (compareType == Comparison::Average) {
            displayTime = format::time(result.average);
        } else if (compareType == Comparison::Median) {
            displayTime = format::time(result.median);
        } else if (compareType == Comparison::Fastest) {
            displayTime = format::time(result.fastest);
        } else if (compareType == Comparison::Slowest) {
            displayTime = format::time(result.slowest);
        }

        if (!i) {
            flag = " (best)";
        } else if ((i + 1) == results.size()) {
            flag = " (worst)";
        }

        if ((i + 1) == results.size()) {
            timeColor = format::style::colors::text::intense::red;
        }

        if (!i) {
            timeColor = format::style::colors::text::intense::green;
        }

        table->addRow(
            tableRow {
                tableElement {
                    result.alias,
                    result.alias
                },
                tableElement {
                    displayTime + flag,
                    format::style::wrap(
                        displayTime,
                        std::vector<std::string> {
                            format::style::decoration::bold,
                            timeColor
                        }
                    ) + format::style::wrap(
                        flag,
                        std::vector<std::string> {
                            format::style::decoration::bold,
                            format::style::colors::text::intense::black
                        }
                    )
                }
            }
        );
    }

    return table->render() + "\n";
}

bool isNumber(const std::string& s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}


int main(int argc, char* argv[]) {
	int id = 1;
	int iterationsCount = 1;
    std::vector<BenchmarkTest> results;

	for (int i = 1; i < argc; i++) {
		std::string arg = std::string(argv[i]);
		
		if (arg[0] == '-' && (i + 1) != argc) {
			std::string numberPart = arg.substr(1, arg.length());

			if (isNumber(numberPart)) {
				iterationsCount = std::stoi(numberPart);
			}

			continue;
		} else {
			results.push_back(runBenchmark(arg, id, iterationsCount));
			iterationsCount = 1;
			id++;
		}
	}

    if (results.size() > 1) {
        std::cout
            << format::style::wrap(
                "\nComparison:",
                std::vector<std::string> {
                    format::style::decoration::bold,
                    format::style::colors::text::intense::yellow
                }
            )
            << "\n";

        std::cout << compareTable(Comparison::Average, BenchmarkTest::compareByAverage(results)) << std::endl;
    }

	return 0;
}
