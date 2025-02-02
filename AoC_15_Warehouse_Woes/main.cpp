#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>
#include<map>

constexpr int number_of_seconds = 100;
constexpr int board_size = 50;

void moveRobotInDirection(char (&board)[board_size][board_size], const std::pair<size_t, size_t>& direction, std::pair<size_t, size_t>& current_position)
{
    std::pair<size_t, size_t> next_position = {current_position.first + direction.first, current_position.second + direction.second}; 
    char next_character = board[next_position.first][next_position.second];
    if (next_character == '#')
    {
        return;
    }

    if (next_character == '.')
    {
        current_position = next_position;
        return;
    }

    std::vector<std::pair<size_t, size_t>> new_walls_positions;
    std::pair<size_t, size_t> wall_position = next_position;
    char wall_character = next_character;
    while (wall_character == 'O')
    {
        wall_position = {wall_position.first + direction.first, wall_position.second + direction.second}; 
        wall_character = board[wall_position.first][wall_position.second];
        new_walls_positions.push_back(wall_position);
    }

    if (wall_character == '#')
    {
        return;
    }

    for (auto position : new_walls_positions)
    {
        board[position.first][position.second] = 'O';
    }

    board[next_position.first][next_position.second] = '.';
    current_position = next_position;
}

void moveNewRobotInDirection(char (&board)[board_size][board_size * 2], const std::pair<size_t, size_t>& direction, std::pair<size_t, size_t>& current_position)
{
    std::pair<size_t, size_t> next_position = {current_position.first + direction.first, current_position.second + direction.second}; 
    char next_character = board[next_position.first][next_position.second];
    if (next_character == '#')
    {
        return;
    }

    if (next_character == '.')
    {
        current_position = next_position;
        return;
    }

    bool horizontal = direction.first == 0;
    if (horizontal)
    {
        size_t direction_sign = direction.second;

        std::vector<std::pair<size_t, size_t>> new_walls_positions;
        std::pair<size_t, size_t> wall_position = next_position;
        char wall_character = next_character;
        while (wall_character == '[' || wall_character == ']')
        {
            wall_position = {wall_position.first + 2 * direction.first, wall_position.second + 2 * direction.second}; 
            wall_character = board[wall_position.first][wall_position.second];
            new_walls_positions.push_back(wall_position);
        }

        if (wall_character == '#')
        {
            return;
        }

        for (auto position : new_walls_positions)
        {
            char first_wall, second_wall;
            if (direction_sign == 1)
            {
                first_wall = ']';
                second_wall = '[';
            }
            else
            {
                first_wall = '[';
                second_wall = ']';
            }

            board[position.first][position.second] = first_wall;
            board[position.first][position.second - direction_sign] = second_wall;
        }

        board[next_position.first][next_position.second] = '.';

        current_position = next_position;
        return;
    }

    // Vertical:
    std::vector<size_t> next_wall_indices;
    size_t wall_row_index = next_position.first;
    if (next_character == '[')
    {
        next_wall_indices.push_back(current_position.second);
    }
    else
    {
        next_wall_indices.push_back(current_position.second - 1);
    }

    std::vector<std::vector<size_t>> wall_indices;
    wall_indices.push_back(next_wall_indices);

    size_t direction_sign = direction.first;
    while (!next_wall_indices.empty())
    {
        std::vector<size_t> next_wall_indices_copy = next_wall_indices;
        next_wall_indices.clear();

        for (auto index : next_wall_indices_copy)
        {
            char next_wall_character = board[wall_row_index + direction_sign][index];
            if (next_wall_character == '#')
            {
                return;
            }

            if (next_wall_character == '[')
            {
                next_wall_indices.push_back(index);
                continue;
            }

            if (next_wall_character == ']')
            {
                next_wall_indices.push_back(index - 1);
            }

            char potential_next_wall_character = board[wall_row_index + direction_sign][index + 1];
            if (potential_next_wall_character == '#')
            {
                return;
            }

            if (potential_next_wall_character == '.')
            {
                continue;
            }
            
            if (potential_next_wall_character == '[')
            {
                next_wall_indices.push_back(index + 1);
            }
        }

        if (!next_wall_indices.empty())
        {
            wall_indices.push_back(next_wall_indices);
            wall_row_index += direction_sign;
        }
    }

    size_t base_row_index = next_position.first;
    for (int i = wall_indices.size() - 1; i >= 0; --i)
    {
        for (auto index : wall_indices[i])
        {
            board[base_row_index + (i + 1) * direction_sign][index] = '[';
            board[base_row_index + (i + 1) * direction_sign][index + 1] = ']';

            board[base_row_index + i * direction_sign][index] = '.';
            board[base_row_index + i * direction_sign][index + 1] = '.';
        }
    }

    current_position = next_position;
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

    char board[board_size][board_size];
    char new_board[board_size][board_size * 2];
    std::vector<char> movements;
    std::string current_line;
    std::string current_string;
    bool board_end = false;
    size_t row_count = 0;
    std::pair<size_t, size_t> starting_position;
    std::pair<size_t, size_t> new_starting_position;
    while (getline(infile, current_line))
    {
        if (current_line.empty())
        {
            board_end = true;
            continue;
        }

        std::istringstream line_stream(current_line);
        std::vector<char> board_vector{std::istream_iterator<char>(line_stream), std::istream_iterator<char>()};
        if (board_end)
        {
            movements.insert(std::end(movements), std::begin(board_vector), std::end(board_vector));
            continue;
        }

        if (board_size != board_vector.size())
        {
            std::cout << "Board has a different size than a line!" << std::endl;
            continue;
        }

        for (size_t i = 0; i < board_size; ++i)
        {
            if (board_vector[i] == '@')
            {
                starting_position = {row_count, i};
                board[row_count][i] = '.';
                continue;
            }

            board[row_count][i] = board_vector[i];
        }

        for (size_t i = 0; i < board_size; ++i)
        {
            if (board_vector[i] == '@')
            {
                new_starting_position = {row_count, 2 * i};
                new_board[row_count][2 * i] = '.';
                new_board[row_count][2 * i + 1] = '.';
                continue;
            }
            if (board_vector[i] == 'O')
            {
                new_board[row_count][2 * i] = '[';
                new_board[row_count][2 * i + 1] = ']';
                continue;
            }

            new_board[row_count][2 * i] = board_vector[i];
            new_board[row_count][2 * i + 1] = board_vector[i];
        }

        row_count++;
    }

    std::map<char, std::pair<size_t, size_t>> directions{{'<', {0, -1}},{'^', {-1, 0}},{'>', {0, 1}},{'v', {1, 0}}};

    // Part 1.
    std::pair<size_t, size_t> current_position = starting_position;
    for (char movement : movements)
    {
        std::pair<size_t, size_t> direction = directions[movement];
        moveRobotInDirection(board, direction, current_position);
    }

    int sum_of_gps = 0;
    for (int i = 0; i < board_size; ++i)
    {
        for (int j = 0; j < board_size; ++j)
        {
            int box_coordinates = 0;
            if (board[i][j] == 'O')
            {
                box_coordinates = 100 * i + j;
            }

            sum_of_gps += box_coordinates;
        }
    }

    std::cout << "The sum of all GPS coordinates is: " << sum_of_gps << std::endl;

    // Part 2.
    std::pair<size_t, size_t> new_current_position = new_starting_position;
    for (char movement : movements)
    {
        std::pair<size_t, size_t> direction = directions[movement];
        moveNewRobotInDirection(new_board, direction, new_current_position);
    }

    int new_sum_of_gps = 0;
    for (int i = 0; i < board_size; ++i)
    {
        for (int j = 0; j < board_size * 2; ++j)
        {
            int box_coordinates = 0;
            if (new_board[i][j] == '[')
            {
                box_coordinates = 100 * i + j;
            }

            new_sum_of_gps += box_coordinates;
        }
    }

    std::cout << "The new sum of all GPS coordinates is: " << new_sum_of_gps << std::endl;
}
