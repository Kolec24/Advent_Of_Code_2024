#include<iostream>
#include<fstream>
#include<vector>
#include<map>
#include<string>

void calculate_smallest_step(std::pair<int, int> pair, std::pair<int, int>& smallest_step)
{
    if (pair.first == 0 && pair.second == 0)
    {
        std::cout << "The pair is (0,0)!" << std::endl; 
        return;
    }

    if (pair.first == 0)
    {
        int sign = pair.second > 0 ? 1 : -1;
        smallest_step = {0, sign};
        return;
    }

    if (pair.second == 0)
    {
        int sign = pair.first > 0 ? 1 : -1;
        smallest_step = {sign, 0};
        return;
    }

    int max_divisor = std::min(abs(pair.first), abs(pair.second));
    if (max_divisor < 2)
    {
        smallest_step = pair;
        return;
    }

    // We should go here only with primes.
    for (int a = 2; a <= max_divisor; ++a)
    {
        if (pair.first % a == 0 && pair.second % a == 0)
        {
            std::pair<int, int> new_pair = { pair.first / a, pair.second / a };
            calculate_smallest_step(new_pair, smallest_step);
            return;
        }
    }

    smallest_step = pair;
}

bool contains_pair(const std::vector<std::pair<int, int>>& vector, const std::pair<int, int>& pair)
{
    for (const auto& current_pair : vector)
    {
        if (current_pair == pair)
        {
            return true;
        }
    }

    return false;
}

int main()
{
    std::map<char, std::vector<std::pair<int, int>>> antennas;

    std::ifstream infile;
    infile.open("puzzle_input.txt");
    if (!infile.is_open())
    {
        std::cout << "File did not open!" << std::endl;
        return -1;
    }

    std::string current_line;
    int row_counter = 0;
    int max_column = 0;
    while (getline(infile, current_line))
    {
        if (max_column == 0)
        {
            max_column = current_line.length() - 1;
        }

        for (int i = 0; i < current_line.length(); ++i)
        {
            if (current_line[i] == '.')
            {
                continue;
            }

            if (antennas.count(current_line[i]) == 0)
            {
                antennas.insert(std::pair<char, std::vector<std::pair<int ,int>>>(current_line[i], std::vector<std::pair<int, int>>()));
            }

            antennas[current_line[i]].push_back(std::pair<int, int>(row_counter, i));
        }

        row_counter++;
    }

    int max_row = row_counter - 1;

    // Part 1.
    std::vector<std::pair<int, int>> antinodes;
    for (auto antenna: antennas)
    {
        for (int i = 0; i < antenna.second.size(); ++i)
        {
            int first_row = antenna.second[i].first;
            int first_column = antenna.second[i].second;
            for (int j = 0; j < antenna.second.size(); ++j)
            {
                if (j == i)
                {
                    continue;
                }

                int second_row = antenna.second[j].first; 
                int second_column = antenna.second[j].second; 

                int row_difference = second_row - first_row;
                int column_difference = second_column - first_column;

                std::pair<int, int> antinode = {second_row + row_difference, second_column + column_difference};
                if (antinode.first < 0 || antinode.first > max_row || antinode.second < 0 || antinode.second > max_column)
                {
                    continue;
                }

                if (!contains_pair(antinodes, antinode))
                {
                    antinodes.push_back(antinode);
                }
            }
        }
    }

    std::cout << "The number of distinct antinodes is: " << antinodes.size() << std::endl;

    // Part 2.
    std::vector<std::pair<int, int>> new_antinodes;
    for (auto antenna: antennas)
    {
        for (int i = 0; i < antenna.second.size(); ++i)
        {
            int first_row = antenna.second[i].first;
            int first_column = antenna.second[i].second;
            for (int j = 0; j < antenna.second.size(); ++j)
            {
                if (j == i)
                {
                    continue;
                }

                int second_row = antenna.second[j].first; 
                int second_column = antenna.second[j].second; 

                int row_difference = second_row - first_row;
                int column_difference = second_column - first_column;

                std::pair<int, int> step;
                std::pair<int, int> difference = { row_difference, column_difference };
                calculate_smallest_step(difference, step);

                std::pair<int, int> current_pair = { first_row, first_column };
                while (current_pair.first >= 0 &&
                    current_pair.first <= max_row && 
                    current_pair.second >= 0 && 
                    current_pair.second <= max_column)
                {
                    std::pair<int, int> antinode = {current_pair.first + step.first, current_pair.second + step.second};
                    if (antinode.first < 0 || antinode.first > max_row || antinode.second < 0 || antinode.second > max_column)
                    {
                        break;
                    }

                    if (!contains_pair(new_antinodes, antinode))
                    {
                        new_antinodes.push_back(antinode);
                    }

                    current_pair = antinode;
                }
            }
        }
    }

    std::cout << "The number of new distinct antinodes is: " << new_antinodes.size() << std::endl;
}
