#include<iostream>
#include<fstream>
#include<utility>
#include<vector>

struct board_data
{
    std::pair<int, int> position;
    std::pair<int, int> direction;
};

bool data_already_achieved(const std::vector<board_data>& hypothetical_data, board_data current_data)
{
    for (const auto& data : hypothetical_data)
    {
        if (data.position == current_data.position && data.direction == current_data.direction)
        {
            return true;
        }
    }

    return false;
}

bool is_board_looping(char (&board)[130][130], board_data starting_data, const std::pair<int, int>& additional_obstacle)
{
    // TODO: I should cache my previous steps, instead of starting over.
    if (board[additional_obstacle.second][additional_obstacle.first] != '.')
    {
        std::cout << "Trying to place an obstacle on an invalid spot!" << additional_obstacle.second << " " << additional_obstacle.first << std::endl;
        return false;
    }

    board[additional_obstacle.second][additional_obstacle.first] = '#';

    std::vector<board_data> hypothetical_data;
    hypothetical_data.push_back(starting_data);

    std::pair<int, int> current_position = starting_data.position;
    std::pair<int, int> current_direction = starting_data.direction;
    board_data current_data;
    current_data.position = current_position;
    current_data.direction = current_direction;
    int safety_counter = 0;
    while (current_position.first != 0 && 
        current_position.first != 129 && 
        current_position.second != 0 && 
        current_position.second != 129)
    {
        if (safety_counter > 1000000)
        {
            std::cout << "Loop function invalid for obstacle: " << additional_obstacle.first << " " << additional_obstacle.second << std::endl;
            break;
        }

        safety_counter++;
        std::pair<int, int> next_position = {current_position.first + current_direction.first, 
                                                current_position.second + current_direction.second};

        if (board[next_position.second][next_position.first] == '#')
        {
            std::pair<int, int> next_direction = {current_direction.second * (-1), current_direction.first};
            current_direction = next_direction;
            current_data.direction = current_direction;
            if (data_already_achieved(hypothetical_data, current_data))
            {
                board[additional_obstacle.second][additional_obstacle.first] = '.';
                return true;
            }

            hypothetical_data.push_back(current_data);
            continue;
        }

        current_position = next_position;
        current_data.position = current_position;
        if (data_already_achieved(hypothetical_data, current_data))
        {
            board[additional_obstacle.second][additional_obstacle.first] = '.';
            return true;
        }

        hypothetical_data.push_back(current_data);
    }

    board[additional_obstacle.second][additional_obstacle.first] = '.';
    return false;
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
    char board[130][130];

    std::ifstream infile;
    infile.open("puzzle_input.txt");
    if (!infile.is_open())
    {
        std::cout << "File did not open!" << std::endl;
        return -1;
    }

    int directions[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    std::pair<int, int> starting_direction;
    std::pair<int, int> starting_position;
    for (int i = 0; i < 130; ++i)
    {
        for (int j = 0; j < 130; ++j)
        {
            infile >> board[i][j];
            switch (board[i][j])
            {
                case '<':
                    starting_direction = {-1, 0}; 
                    starting_position = {j, i};
                    break;
                case '>':
                    starting_direction = {1, 0}; 
                    starting_position = {j, i};
                    break;
                case '^':
                    starting_direction = {0, -1}; 
                    starting_position = {j, i};
                    break;
                case 'v':
                    starting_direction = {0, 1}; 
                    starting_position = {j, i};
                    break;
                default:
                    break;
            }
        }
    }

    if (starting_position.first == 0 || 
        starting_position.first == 129 || 
        starting_position.second == 0 || 
        starting_position.second == 129)
    {
        std::cout << "Starting position is on the edge - watch out!" << std::endl;
        return -1;
    }

    // Part 1.
    std::vector<std::pair<int, int>> visited_locations;
    visited_locations.push_back(starting_position);

    std::pair<int, int> current_position = starting_position;
    std::pair<int, int> current_direction = starting_direction;
    while (current_position.first != 0 && 
        current_position.first != 129 && 
        current_position.second != 0 && 
        current_position.second != 129)
    {
        std::pair<int, int> next_position = {current_position.first + current_direction.first, 
                                                current_position.second + current_direction.second};

        if (board[next_position.second][next_position.first] == '#')
        {
            std::pair<int, int> next_direction = {current_direction.second * (-1), current_direction.first};
            current_direction = next_direction;
            continue;
        }

        current_position = next_position;
        if (!contains_pair(visited_locations, current_position))
        {
            visited_locations.push_back(current_position);
        }
    }

    std::cout << "The number of distinct positions the guard will visit is: " << visited_locations.size() << std::endl;

    // Part 2.
    board_data starting_data;
    starting_data.position = starting_position;
    starting_data.direction = starting_direction;

    current_position = starting_position;
    current_direction = starting_direction;
    std::vector<std::pair<int, int>> already_checked_obstacles;
    int additional_obstacles_counter = 0;
    int safety_counter = 0;
    while (current_position.first != 0 && 
        current_position.first != 129 && 
        current_position.second != 0 && 
        current_position.second != 129)
    {
        if (safety_counter > 1000000)
        {
            std::cout << "Main loop invalid!" << std::endl;
            break;
        }

        safety_counter++;

        std::pair<int, int> next_position = {current_position.first + current_direction.first, 
                                                current_position.second + current_direction.second};
        if (board[next_position.second][next_position.first] == '#')
        {
            std::pair<int, int> next_direction = {current_direction.second * (-1), current_direction.first};
            current_direction = next_direction;
            continue;
        }

        if (next_position == starting_position)
        {
            current_position = next_position;
            continue;
        }

        if (contains_pair(already_checked_obstacles, next_position))
        {
            current_position = next_position;
            continue;
        }

        already_checked_obstacles.push_back(next_position);
        if (is_board_looping(board, starting_data, next_position))
        {
            additional_obstacles_counter++;
        }

        current_position = next_position;
    }

    std::cout << "The number of different positions for an additional obstacle is: " << additional_obstacles_counter << std::endl;
}
