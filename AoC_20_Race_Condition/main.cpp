#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>
#include<map>

constexpr int cheat_threshold = 100;
constexpr int board_size = 141;
char board[board_size][board_size];
std::pair<int, int> starting_position; 
std::pair<int, int> ending_position; 
std::map<std::pair<int, int>, int> distances;

std::vector<std::pair<int, int>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

void calculateDistances()
{
    std::pair<int, int> current_position = starting_position;
    distances.insert({current_position, 0});
    while (current_position != ending_position)
    {
        for (const auto& direction : directions)
        {
            std::pair<int, int> next_position = {current_position.first + direction.first, current_position.second + direction.second};
            if (next_position.first < 0 || next_position.first >= board_size ||
                next_position.second < 0 || next_position.second >= board_size)
            {
                continue;
            }

            if (board[next_position.first][next_position.second] == '#')
            {
                continue;
            }

            if (distances.count(next_position) != 0)
            {
                continue;
            }

            if (distances.count(current_position) == 0)
            {
                std::cout << "Current distance is unknown!" << std::endl;
                return;
            }

            int next_distance = distances[current_position] + 1;
            distances.insert({next_position, next_distance});

            current_position = next_position;
        }
    }
}

int calculateCheats(int max_cheat_length, int cheat_threshold)
{
    int cheat_counter = 0;
    for (const auto& pair : distances)
    {
        std::pair<int, int> current_position = pair.first;
        int current_distance = pair.second;
        for (int i = 0; i < board_size; ++i)
        {
            for (int j = 0; j < board_size; ++j)
            {
                int current_cheat_length = std::abs(i - current_position.first) + std::abs(j - current_position.second);
                if (current_cheat_length > max_cheat_length)
                {
                    continue;
                }

                if (board[i][j] == '#')
                {
                    continue;
                }

                std::pair<int, int> after_position = {i, j};
                if (distances.count(after_position) == 0)
                {
                    std::cout << "After next position is valid but not in the distances map!" << std::endl;
                    return -1;
                }

                int after_distance = distances[after_position];

                if (after_distance - current_distance >= cheat_threshold + current_cheat_length)
                {
                    cheat_counter++;
                }
            }
        }
    }

    return cheat_counter;
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
    int row_index = 0;
    while (getline(infile, current_line))
    {
        std::istringstream line_stream(current_line);
        std::vector<char> board_vector{std::istream_iterator<char>(line_stream), std::istream_iterator<char>()};
        for (int i = 0; i < board_size; ++i)
        {
            if (board_vector.size() != board_size)
            {
                std::cout << "Board vector has different size than the board!" << std::endl;
                return -1;
            }

            if (board_vector[i] == 'S')
            {
                starting_position = {row_index, i};
            }

            if (board_vector[i] == 'E')
            {
                ending_position = {row_index, i};
            }
            
            board[row_index][i] = board_vector[i];
        }

        row_index++;
    }

    calculateDistances();

    // Part 1.
    int cheat_length = 2;
    int part_1 = calculateCheats(cheat_length, cheat_threshold);

    std::cout << "The number of cheats of max length " << cheat_length << " saving at least " << cheat_threshold << " seconds is: " << part_1 << std::endl;

    // Part 2.
    cheat_length = 20;
    int part_2 = calculateCheats(cheat_length, cheat_threshold);

    std::cout << "The number of cheats of max length " << cheat_length << " saving at least " << cheat_threshold << " seconds is: " << part_2 << std::endl;

    return 0;
}
