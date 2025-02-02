#include<iostream>
#include<fstream>
#include<vector>
#include<sstream>

int main()
{
    std::vector<long long> results;
    std::vector<std::vector<int>> operators;
    std::vector<std::vector<long long>> possible_outcomes;

    std::ifstream infile;
    infile.open("puzzle_input.txt");
    if (!infile.is_open())
    {
        std::cout << "File did not open!" << std::endl;
        return -1;
    }

    std::string current_line;
    while (getline(infile, current_line))
    {
        if (current_line.empty())
        {
            std::cout << "Empty line!" << std::endl;
            continue;
        }

        std::string result_string;
        while (current_line[0] != ':')
        {
            if (!isdigit(current_line[0]))
            {
                std::cout << "There is a non-digit character before the colon!" << std::endl;
                return -1;
            }

            result_string += current_line[0];
            current_line.erase(current_line.begin());
        }

        current_line.erase(current_line.begin());
        current_line.erase(current_line.begin());

        long long result = std::stoll(result_string); 
        results.push_back(result);

        std::istringstream line_stream(current_line);
        std::vector<int> current_vector{std::istream_iterator<int>{line_stream}, std::istream_iterator<int>{}};
        operators.push_back(current_vector);
    }

    if (results.size() != operators.size())
    {
        std::cout << "Results and operators are of different size!" << std::endl;
        return -1;
    }

    // Part 1. & 2.
    long long total_sum = 0;
    for (int i = 0; i < results.size(); ++i)
    {
        const std::vector<int>& current_operators = operators[i];

        std::vector<long long> possible_results = { current_operators[0] };
        int current_operator_index = 1;
        while (current_operator_index < current_operators.size())
        {
            std::vector<long long> next_step;
            for (int j = 0; j < possible_results.size(); ++j)
            {
                next_step.push_back(possible_results[j] + current_operators[current_operator_index]);
                next_step.push_back(possible_results[j] * current_operators[current_operator_index]);

                // Part 2.
                std::string first = std::to_string(possible_results[j]);
                std::string second = std::to_string(current_operators[current_operator_index]);

                std::string concatenated_string = first + second;
                long long concatenated = std::stoll(concatenated_string);
                next_step.push_back(concatenated);
            }

            possible_results = next_step;
            ++current_operator_index;
        }

    
        for (const long long& possible_result : possible_results)
        {
            if (possible_result == results[i])
            {
                total_sum += results[i];
                break;
            }
        }
    }

    std::cout << "The sum of all possible results is: " << total_sum << std::endl;
}
