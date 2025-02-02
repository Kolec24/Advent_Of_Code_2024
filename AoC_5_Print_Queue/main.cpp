#include<iostream>
#include<fstream>
#include<map>
#include<vector>
#include<string>
#include<sstream>

int main()
{
    std::map<int, std::vector<int>> rules;
    std::vector<std::vector<int>> updates;

    std::ifstream infile;
    infile.open("puzzle_input.txt");
    if (!infile.is_open())
    {
        std::cout << "File did not open!" << std::endl;
        return -1;
    }

    bool reading_rules = true;
    std::string current_line;
    while (getline(infile, current_line))
    {
        if (current_line.empty())
        {
            reading_rules = false;
            continue;
        }

        if (reading_rules)
        {
            if (current_line.length() != 5)
            {
                std::cout << "The length of a rule is not 5!" << std::endl;
                return -1;
            }

            int before_int = std::stoi(current_line.substr(0, 2));
            int after_int = std::stoi(current_line.substr(3, 2));
            if (rules.count(before_int) == 0)
            {
                rules.insert(std::pair<int, std::vector<int>>(before_int, std::vector<int>()));
            }

            rules[before_int].push_back(after_int);
            continue;
        }

        std::istringstream line_stream(current_line);
        int temporary_int = 0;
        std::vector<int> current_vector;
        while (line_stream >> temporary_int)
        {
            // TODO: Investigate this.
            current_vector.push_back(temporary_int);
            if (line_stream.peek() == ',')
            {
                line_stream.ignore();
            }
        }

        updates.push_back(current_vector);
    }

    std::vector<std::vector<int>> wrong_updates;

    // Part 1.
    int total_middle_sum = 0;
    for (auto update : updates)
    {
        bool update_correct = true;
        for (size_t i = update.size() - 1; i > 0; --i)
        {
            int current_number = update[i];
            if (rules.count(current_number) == 0)
            {
                continue;
            }

            // TODO: This is terrible.
            for (size_t j = 0; j < i; ++j)
            {
                for (const int& entry : rules[current_number])
                {
                    if (entry == update[j])
                    {
                        update_correct = false;
                        break;
                    }
                }

                if (!update_correct)
                {
                    break;
                }
            }

            if (!update_correct)
            {
                break;
            }
        }

        if (update_correct)
        {
            int middle_index = (update.size() - 1) / 2;
            int middle_number = update[middle_index];
            total_middle_sum += middle_number; 
        }
        else
        {
            wrong_updates.push_back(update);
        }

    }

    std::cout << "The total sum of middle numbers from correct updates is: " << total_middle_sum << std::endl;

    // Part 2.
    int corrected_middle_sum = 0;
    for (auto update : wrong_updates)
    {
        for (size_t i = 0; i < update.size() - 1;)
        {
            size_t current_index = i;
            bool update_modified = false;
            int current_number = update[i];

            for (size_t j = i + 1; j < update.size(); ++j)
            {
                for (const int& entry : rules[update[j]])
                {
                    if (entry == current_number)
                    {
                        int moved_number = update[j];
                        update[current_index] = moved_number;
                        update[j] = current_number;
                        current_index = j;
                        update_modified = true;
                        break;
                    }
                }
            }

            if (!update_modified)
            {
                ++i;
            }
        }

        // We are sure that update is correct here.
        int middle_index = (update.size() - 1) / 2;
        int middle_number = update[middle_index];
        corrected_middle_sum += middle_number; 
    }

    std::cout << "The total sum of middle numbers from newly correct updates is: " << corrected_middle_sum << std::endl;
}
