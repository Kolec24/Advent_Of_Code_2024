#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>
#include<set>
#include<map>

std::vector<std::string> designs;
std::vector<std::string> possible_designs;
std::vector<std::string> towels;

std::set<std::string> possible_strings;
std::set<std::string> not_possible_strings;

std::map<std::string, long long> string_counts;

bool isDesignPossible(std::string design)
{
    if (design.empty())
    {
        return true;
    }

    if (possible_strings.find(design) != possible_strings.end())
    {
        return true;
    }

    if (not_possible_strings.find(design) != not_possible_strings.end())
    {
        return false;
    }

    bool possible = false;
    for (auto towel : towels)
    {
        size_t length = towel.length();
        std::string substring = design.substr(0, length);
        if (substring != towel)
        {
            continue;
        }

        std::string new_design = design.substr(length);
        if (isDesignPossible(new_design))
        {
            possible = true;
            break;
        }
    }

    if (possible)
    {
        possible_strings.insert(design);
    }
    else
    {
        not_possible_strings.insert(design);
    }

    return possible;
}

long long countPossiblePatterns(std::string design)
{
    if (design.empty())
    {
        return 1;
    }

    if (string_counts.count(design) != 0)
    {
        return string_counts[design];
    }

    long long counter = 0;
    for (auto towel : towels)
    {
        size_t length = towel.length();
        std::string substring = design.substr(0, length);
        if (substring != towel)
        {
            continue;
        }

        std::string new_design = design.substr(length);
        counter += countPossiblePatterns(new_design);
    }

    string_counts.insert({design, counter});
    return counter;
}

int main()
{
    std::ifstream infile;
    infile.open("puzzle_input.txt");
    if (!infile.is_open())
    {
        std::cout << "File did not open!" << std::endl;
        return -1;
    }

    std::string current_line;
    bool parse_designs = false;
    while (getline(infile, current_line))
    {
        if (current_line.empty())
        {
            parse_designs = true;
            continue;
        }

        if (parse_designs)
        {
            designs.push_back(current_line);
            continue;
        }

        std::istringstream line_stream(current_line);
        for (std::string i; line_stream >> i;)
        {
            if (i.back() == ',')
            {
                i.pop_back();
            }

            towels.push_back(i);
        }
    }

    // Part 1.
    int possible_counter = 0;
    for (auto design : designs)
    {
        if (isDesignPossible(design))
        {
            possible_counter++;
            possible_designs.push_back(design);
        }
    }

    std::cout << "The number of possible designs is: " << possible_counter << std::endl;

    // Part 2.
    long long arrangement_counter = 0;
    for (auto design : possible_designs)
    {
        long long design_counter = countPossiblePatterns(design);
        arrangement_counter += design_counter;
    }

    std::cout << "The sum of all possible arrangements of all possible designs is: " << arrangement_counter << std::endl;
    return 0;
}
