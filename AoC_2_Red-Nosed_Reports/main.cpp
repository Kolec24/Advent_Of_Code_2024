#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>
#include<cmath>

bool is_report_safe(const std::vector<int>& report)
{
    if (report.size() <= 1)
    {
        return true;
    }

    bool is_report_positive = report[1] - report[0] > 0;
    for (int i = 1; i < report.size(); ++i)
    {
        int difference = report[i] - report[i - 1];
        if (abs(difference) < 1 || abs(difference) > 3)
        {
            return false;
        }

        if (is_report_positive != difference > 0)
        {
            return false;
        }
    }

    return true;
}

int main()
{
    std::vector<std::vector<int>> reports;

    std::ifstream infile;
    infile.open("puzzle_input.txt");
    if (!infile.is_open())
    {
        std::cout << "File did not open!" << std::endl;
        return -1;
    }

    std::string line;
    while(getline(infile, line))
    {
        std::istringstream line_stream(line);
        std::vector<int> current_vector{std::istream_iterator<int>{line_stream}, std::istream_iterator<int>{}};
        reports.push_back(current_vector);
    }

    // Part 1.
    bool is_report_positive = false;
    int safe_counter = 0;
    for (const std::vector<int>& report : reports)
    {
        if (is_report_safe(report))
        {
            safe_counter++;
        }
    }

    std::cout << "The number of safe reports is: " << safe_counter << std::endl;

    // Part 2.
    // TODO: Find some clever optimisation, to remove this copying and brute-forcing.
    size_t updated_safe_counter = 0;
    for (const std::vector<int>& report : reports)
    {
        if (report.size() <= 2)
        {
            continue;
        }

        std::vector<int> copy_report;
        for (size_t i = 0; i < report.size(); ++i)
        {
            copy_report = report;
            copy_report.erase(copy_report.begin() + i);
            if (is_report_safe(copy_report))
            {
                updated_safe_counter++;
                break;
            }
        }
    }

    std::cout << "The updated number of safe reports is: " << updated_safe_counter << std::endl;
}
