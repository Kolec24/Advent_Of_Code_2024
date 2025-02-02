#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>

constexpr int board_size = 71;
constexpr int number_of_falling_bytes = 1024;

std::vector<std::pair<int, int>> falling_bytes;
char board[board_size][board_size];
std::vector<std::pair<size_t, size_t>> all_positions;
std::pair<int, int> starting_position = {0, 0};
std::pair<int, int> end_position = {board_size - 1, board_size - 1};
std::vector<std::pair<int, int>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

size_t getPositionIndex(const std::pair<size_t, size_t>& position)
{
    for (size_t i = 0; i < all_positions.size(); ++i)
    {
        if (all_positions[i] == position)
        {
            return i;
        }
    }

    return -1;
}

size_t minDistance(const std::vector<int>& dist, const std::vector<bool>& visited)
{
    size_t index;
    int min = INT_MAX;

    for (size_t i = 0; i < all_positions.size(); ++i)
    {
        if (visited[i] == false && dist[i] <= min)
        {
            min = dist[i];
            index = i;
        }
    }

    return index;
}

int dijkstra()
{
    std::vector<int> dist;
    std::vector<bool> visited;

    for (size_t i = 0; i < all_positions.size(); ++i)
    {
        dist.push_back(INT_MAX);
        visited.push_back(false);
    }

    int starting_index = 0;


    dist[starting_index] = 0;

    for (size_t i = 0; i < all_positions.size(); ++i)
    {
        size_t m = minDistance(dist, visited);
        auto current_position = all_positions[m];

        visited[m] = true;

        for (const auto& direction : directions)
        {
            if (current_position.first + direction.first < 0 || current_position.first + direction.first >= board_size ||
                current_position.second + direction.second < 0 || current_position.second + direction.second >= board_size)
            {
                continue;
            }

            std::pair<size_t, size_t> next_position = {current_position.first + direction.first, current_position.second + direction.second};
            if (board[next_position.first][next_position.second] == '#')
            {
                continue;
            }

            size_t next_index = getPositionIndex(next_position);

            int traversal_cost = 1;

            if (visited[next_index] == false && dist[m] != INT_MAX && dist[m] + traversal_cost < dist[next_index])
            {
                dist[next_index] = dist[m] + traversal_cost;
            }
        }
    }

    size_t end_index = getPositionIndex(end_position);
    int min_end_cost = dist[end_index];


    return min_end_cost;
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

    while (getline(infile, current_line))
    {
        std::istringstream line_stream(current_line);
        std::vector<int> coordinates;
        for (int i; line_stream >> i;)
        {
            coordinates.push_back(i);
            if (line_stream.peek() == ',')
            {
                line_stream.ignore();
            }
        }

        if (coordinates.size() < 2)
        {
            std::cout << "Coordinates have less than two members!" << std::endl;
            continue;
        }
        std::pair<int, int> byte = {coordinates[0], coordinates[1]};
        falling_bytes.push_back(byte);
    }

    // Part 1.
    for (size_t i = 0; i < number_of_falling_bytes; ++i)
    {
        if (i >= falling_bytes.size())
        {
            std::cout << "Less bytes than falling number!" << std::endl;
            return -1;
        }

        auto pair = falling_bytes[i];
        if (pair.first >= board_size || pair.second >= board_size)
        {
            std::cout << "Byte has a coordinate larger than board size!" << std::endl;
            return -1;
        }

        board[pair.second][pair.first] = '#';
    }

    for (size_t i = 0; i < board_size; ++i)
    {
        for (size_t j = 0; j < board_size; ++j)
        {
            if (board[i][j] == '#')
            {
                continue;
            }

            board[i][j] = '.';
            all_positions.push_back({i, j});
        }
    }

    int min_cost = 0;
    min_cost = dijkstra();
    std::cout << "The minimum number of steps is: " << min_cost << std::endl;

    // Part 2.
    size_t next_half_size = falling_bytes.size() / 2;
    size_t current_number_of_falling_bytes = next_half_size;
    size_t final_index = 0;
    while (next_half_size != 0)
    {
        std::cout << "Next falling number is: " << current_number_of_falling_bytes << std::endl;

        // Reset the board.
        for (size_t i = 0; i < board_size; ++i)
        {
            for (size_t j = 0; j < board_size; ++j)
            {
                board[i][j] = '.';
            }
        }

        // Fill new walls.
        for (size_t i = 0; i < current_number_of_falling_bytes; ++i)
        {
            if (i >= falling_bytes.size())
            {
                std::cout << "Less bytes than falling number!" << std::endl;
                return -1;
            }

            auto pair = falling_bytes[i];
            if (pair.first >= board_size || pair.second >= board_size)
            {
                std::cout << "Byte has a coordinate larger than board size!" << std::endl;
                return -1;
            }

            board[pair.second][pair.first] = '#';
        }

        // Fill all remaining positions.
        all_positions.clear();
        for (size_t i = 0; i < board_size; ++i)
        {
            for (size_t j = 0; j < board_size; ++j)
            {
                if (board[i][j] == '#')
                {
                    continue;
                }

                board[i][j] = '.';
                all_positions.push_back({i, j});
            }
        }

        next_half_size /= 2;
        int cost = dijkstra();
        if (cost == INT_MAX)
        {
            current_number_of_falling_bytes -= next_half_size;
            if (next_half_size == 0)
            {
                final_index = current_number_of_falling_bytes - 1;
            }
        }
        else
        {
            current_number_of_falling_bytes += next_half_size;
            if (next_half_size == 0)
            {
                final_index = current_number_of_falling_bytes;
            }
        }
    }

    std::cout << "The final index is: " << final_index << std::endl;
    auto final_byte = falling_bytes[final_index];
    std::cout << "The first byte that will make the escape impossible is: " << final_byte.first << "," << final_byte.second << std::endl;
}
