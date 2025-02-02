#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<sstream>

bool contains_pair(const std::vector<std::pair<size_t, size_t>>& vector, const std::pair<size_t, size_t>& pair)
{
    for (auto current_pair : vector)
    {
        if (pair == current_pair)
        {
            return true;
        }
    }

    return false;
}

void calculate_all_trail_ends(const int (&board)[54][54], std::pair<size_t, size_t> current_location, const std::vector<std::pair<int, int>>& directions, 
                              int& trailhead_count, int& new_trailhead_count, std::vector<std::pair<size_t, size_t>>& visited_end_points)
{
    int current_number = board[current_location.first][current_location.second]; 
    if (current_number == 9)
    {
        if (!contains_pair(visited_end_points, current_location))
        {
            trailhead_count++;
            visited_end_points.push_back(current_location);
        }

        new_trailhead_count++;
        return;
    }

    for (auto direction : directions)
    {
         std::pair<size_t, size_t> next_location = 
            std::pair<size_t, size_t>(current_location.first + direction.first, current_location.second + direction.second);
        if (next_location.first < 0 || next_location.first > 54 || next_location.second < 0 || next_location.second > 53)
        {
            continue;
        }

        if (board[next_location.first][next_location.second] != current_number + 1)
        {
            continue;
        }

        calculate_all_trail_ends(board, next_location, directions, trailhead_count, new_trailhead_count, visited_end_points);
    }
}

int main()
{
    int board[54][54];

    std::ifstream infile;
    infile.open("puzzle_input.txt");
    if (!infile.is_open())
    {
        std::cout << "File did not open!" << std::endl;
        return -1;
    }

    char current_char;
    std::string current_line;
    int row_index = 0;
    int column_index = 0;
    while (getline(infile, current_line))
    {
        std::istringstream line_stream(current_line);
        while (line_stream >> current_char)
        {
            if (!isdigit(current_char))
            {
                std::cout << "The entry is not a digit!" << std::endl;
                return -1;
            }

            int current_number = current_char - '0';
            board[row_index][column_index] = current_number;
            column_index++;
        }
        row_index++;
        column_index = 0;
    }

    int total_trailhead_sum = 0;
    int new_total_trailhead_sum = 0;
    std::vector<std::pair<int, int>> directions = { {1, 0}, {0, 1}, {-1, 0}, {0, -1} };
    for (int i = 0; i < 54; ++i)
    {
        for (int j = 0; j < 54; ++j)
        {
            if (board[i][j] != 0)
            {
                continue;
            }

            int trailhead_count = 0;
            int new_trailhead_count = 0;
            std::vector<std::pair<size_t, size_t>> visited_end_points;
            std::pair<size_t, size_t> current_location(i, j);
            calculate_all_trail_ends(board, current_location, directions, trailhead_count, new_trailhead_count, visited_end_points);

            total_trailhead_sum += trailhead_count;
            new_total_trailhead_sum += new_trailhead_count;
        }
    }

    std::cout << "The sum of all trailhead scores is: " << total_trailhead_sum << std::endl;
    std::cout << "The new sum of all trailhead scores is: " << new_total_trailhead_sum << std::endl;
}
