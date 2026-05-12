#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <regex>
#include "Simulation.h"

void printErrorLine(const std::string& line) {
    if (!line.empty()) {
        std::cerr << line << std::endl;
    } else {
        std::cerr << "Error or unexpected EOF\n" << std::endl;
    }
}

std::vector<long long> parseList(const std::string& str) {
    std::vector<long long> res;
    std::stringstream ss(str);
    std::string item;
    while (ss >> item) {
        res.push_back(stoll(item));
    }
    return res;
}

int main(int argc, char* argv[]) {

    // Этап парсинга

    if (argc < 2) {
        std::cerr << "Error! Enter input file name as an argument." << std::endl;
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    if (!inputFile.is_open()) {
        perror("Error! Cannot open the file.");
        return 1;
    }

    std::string line;
    std::smatch matches;

    std::regex head_regex(R"(^\s*(-?\d+)\s+(-?\d+)\s*$)");
    std::regex list_regex(R"(^\s*-?\d+(?:\s+-?\d+)*\s*$)");

    if (!getline(inputFile, line) || !regex_match(line, matches, head_regex)) {
        printErrorLine(line);
        return 1;
    }

    long long m, n;
    try {
        m = stoll(matches[1].str());
        n = stoll(matches[2].str());
        if (m < 1 || m > 100 || n < 1 || n > 100) {
            printErrorLine(line);
            return 1;
        }
    } catch (...) {
        printErrorLine(line);
        return 1;
    }

    std::vector t_table(m - 1, std::vector<int>(n));
    for (int i = 0; i < m - 1; ++i) {
        if (!getline(inputFile, line) || !regex_match(line, list_regex)) {
            printErrorLine(line);
            return 1;
        }
        try {
            std::vector<long long> row = parseList(line);
            if (row.size() != n) {
                printErrorLine(line);
                return 1;
            }
            for (int j = 0; j < n; ++j) {
                if (row[j] < 0 || row[j] > 10000) {
                    printErrorLine(line);
                    return 1;
                }
                t_table[i][j] = static_cast<int>(row[j]);
            }
        } catch (...) {
            printErrorLine(line);
            return 1;
        }
    }

    std::vector<std::queue<Item>> machine_queues(n);
    std::vector<long long> current_load(n, 0);
    int global_k = 0;

    for (int j = 0; j < n; ++j) {
        if (!getline(inputFile, line) || !regex_match(line, list_regex)) {
            printErrorLine(line);
            return 1;
        }
        try {
            std::vector<long long> q_data = parseList(line);
            if (q_data.empty()) {
                printErrorLine(line);
                return 1;
            }
            long long q_j_count = q_data[0];
            if (q_j_count < 0 || q_data.size() != q_j_count + 1) {
                printErrorLine(line);
                return 1;
            }
            for (int k = 1; k <= q_j_count; ++k) {
                long long type = q_data[k];
                if (type < 0 || type > m - 2) {
                    printErrorLine(line);
                    return 1;
                }
                machine_queues[j].push({global_k++, static_cast<int>(type)});
                current_load[j] += t_table[static_cast<int>(type)][j];
            }
        } catch (...) {
            printErrorLine(line);
            return 1;
        }
    }

    inputFile.close();

    // Этап симуляции

    Simulation sim(m, n, t_table, machine_queues, current_load);
    sim.run();

    return 0;
}