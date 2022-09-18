#pragma once

#ifndef FORMAT_H
#define FORMAT_H

#include <string>
#include <vector>
#include <chrono>


typedef std::pair<std::string, std::string> tableElement;
typedef std::vector<tableElement> tableRow;
typedef std::vector<tableRow> tableRows;


namespace format {
    namespace symbols {
        std::vector<std::string> braileCycled = {
            "⠟",
            "⠯",
            "⠷",
            "⠾",
            "⠽",
            "⠻"
        };
    }

    namespace style {
        namespace colors {
            namespace text {
                namespace intense {
                    std::string black = "\x1B[90m";
                    std::string red = "\x1B[91m";
                    std::string green = "\x1B[92m";
                    std::string yellow = "\x1B[93m";
                    std::string blue = "\x1B[94m";
                    std::string purple = "\x1B[95m";
                    std::string cyan = "\x1B[96m";
                    std::string white = "\x1B[97m";
                }

                std::string black = "\x1B[30m";
                std::string red = "\x1B[31m";
                std::string green = "\x1B[32m";
                std::string yellow = "\x1B[33m";
                std::string blue = "\x1B[34m";
                std::string purple = "\x1B[35m";
                std::string cyan = "\x1B[36m";
                std::string white = "\x1B[37m";
            }

            namespace background {
                namespace intense {
                    std::string black = "\x1B[100m";
                    std::string red = "\x1B[101m";
                    std::string green = "\x1B[102m";
                    std::string yellow = "\x1B[103m";
                    std::string blue = "\x1B[104m";
                    std::string purple = "\x1B[105m";
                    std::string cyan = "\x1B[106m";
                    std::string white = "\x1B[107m";
                }

                std::string black = "\x1B[40m";
                std::string red = "\x1B[41m";
                std::string green = "\x1B[42m";
                std::string yellow = "\x1B[43m";
                std::string blue = "\x1B[44m";
                std::string purple = "\x1B[45m";
                std::string cyan = "\x1B[46m";
                std::string white = "\x1B[47m";
            }
        }

        namespace decoration {
            std::string bold = "\e[1m";
            std::string underline = "\e[4m";
        }

        std::string reset = "\e[0m";
    
        std::string wrap(std::string text, std::vector<std::string> styles);
    }

    class ProgressBar {
        public:
			std::vector<std::string> indicators;
			std::string alias;
            int indicatorIndex;

			ProgressBar(std::vector<std::string> indicators, std::string alias);

            void setAlias(std::string alias);

			std::string render();
			std::string update();
			std::string purge();
    };

    class Table {
        public:
            tableRows rows;

            Table();
            Table(tableRows rows);

            void addRow(tableRow row);
            void addRows(tableRows rows);
            void extendRow(int rowIndex, tableRow elements);

            std::string render();
    };

    std::string join(std::string separator, std::vector<std::string> strings);
    std::string time(std::chrono::nanoseconds t);

    std::string prevLine = "\033[F";
    std::string clearLine = "\033[2K\r";
}

#include "format.cpp"

#endif
