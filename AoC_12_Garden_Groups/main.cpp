#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>

constexpr int board_size = 140;

struct Region
{
    char plant;
    int area = 0;
    int perimeter = 0;
    int sides = 0;
};

struct Tile
{
    char plant;
    bool checked = false;
};

int calculateRegionHorizontalSides(const bool (&region_board)[board_size][board_size])
{
    int horizontal_count = 0;
    bool wall_started = false;

    // Top walls.
    for (int j = 0; j < board_size; ++j)
    {
        if (region_board[0][j])
        {
            wall_started = true;
            continue;
        }

        if (wall_started)
        {
            wall_started = false;
            horizontal_count++;
        }
    }
    if (wall_started)
    {
        wall_started = false;
        horizontal_count++;
    }

    // Bottom walls.
    for (int j = 0; j < board_size; ++j)
    {
        if (region_board[board_size - 1][j])
        {
            wall_started = true;
            continue;
        }

        if (wall_started)
        {
            wall_started = false;
            horizontal_count++;
        }
    }
    if (wall_started)
    {
        wall_started = false;
        horizontal_count++;
    }

    // Middle walls.
    for (int i = 0; i < board_size - 1; ++i)
    {
        for (int j = 0; j < board_size; ++j)
        {
            if (region_board[i][j] && !region_board[i + 1][j])
            {
                wall_started = true;
                continue;
            }

            if (wall_started)
            {
                wall_started = false;
                horizontal_count++;
            }
        }
        if (wall_started)
        {
            wall_started = false;
            horizontal_count++;
        }
    }

    for (int i = 0; i < board_size - 1; ++i)
    {
        for (int j = 0; j < board_size; ++j)
        {
            if (!region_board[i][j] && region_board[i + 1][j])
            {
                wall_started = true;
                continue;
            }

            if (wall_started)
            {
                wall_started = false;
                horizontal_count++;
            }
        }
        if (wall_started)
        {
            wall_started = false;
            horizontal_count++;
        }
    }

    return horizontal_count;
}

int calculateRegionVerticalSides(const bool (&region_board)[board_size][board_size])
{
    int vertical_count = 0;
    bool wall_started = false;

    // Left walls;
    for (int j = 0; j < board_size; ++j)
    {
        if (region_board[j][0])
        {
            wall_started = true;
            continue;
        }

        if (wall_started)
        {
            wall_started = false;
            vertical_count++;
        }
    }
    if (wall_started)
    {
        wall_started = false;
        vertical_count++;
    }

    // Right walls;
    for (int j = 0; j < board_size; ++j)
    {
        if (region_board[j][board_size - 1])
        {
            wall_started = true;
            continue;
        }

        if (wall_started)
        {
            wall_started = false;
            vertical_count++;
        }
    }
    if (wall_started)
    {
        wall_started = false;
        vertical_count++;
    }

    // Middle walls.
    for (int j = 0; j < board_size - 1; ++j)
    {
        for (int i = 0; i < board_size; ++i)
        {
            if (region_board[i][j] && !region_board[i][j + 1])
            {
                wall_started = true;
                continue;
            }

            if (wall_started)
            {
                wall_started = false;
                vertical_count++;
            }
        }
        if (wall_started)
        {
            wall_started = false;
            vertical_count++;
        }
    }

    for (int j = 0; j < board_size - 1; ++j)
    {
        for (int i = 0; i < board_size; ++i)
        {
            if (!region_board[i][j] && region_board[i][j + 1])
            {
                wall_started = true;
                continue;
            }

            if (wall_started)
            {
                wall_started = false;
                vertical_count++;
            }
        }
        if (wall_started)
        {
            wall_started = false;
            vertical_count++;
        }
    }

    return vertical_count;
}


void calculateRegion(const std::pair<int, int> initial_position, std::pair<int, int> position, Region& region, Tile (&board)[board_size][board_size], 
                     const std::vector<std::pair<int, int>>& directions, bool (&region_board)[board_size][board_size])
{
    region_board[position.first][position.second] = true;

    region.area++;
    board[position.first][position.second].checked = true;
    char current_plant = board[position.first][position.second].plant;
    for (const auto& direction : directions)
    {
        std::pair<int, int> next_position = {position.first + direction.first, position.second + direction.second};
        if (next_position.first < 0 || next_position.first > board_size - 1 ||
            next_position.second < 0 || next_position.second > board_size - 1)
        {
            region.perimeter++;
            continue;
        }

        Tile next_tile = board[next_position.first][next_position.second];
        if (next_tile.plant != current_plant)
        {
            region.perimeter++;
            continue;
        }
        
        if (next_tile.checked)
        {
            continue;
        }

        calculateRegion(initial_position, next_position, region, board, directions, region_board);
    }
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

    Tile board[board_size][board_size];
    std::string current_line;
    int row_index = 0;
    while (getline(infile, current_line))
    {
        std::istringstream line_stream(current_line);
        std::vector<char> current_vector{std::istream_iterator<char>{line_stream}, std::istream_iterator<char>{}};
        for (int i = 0; i < current_vector.size(); ++i)
        {
            board[row_index][i].plant = current_vector[i];
        }

        ++row_index;
    }

    std::vector<std::pair<int, int>> directions = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

    int total_price = 0;
    int new_total_price = 0;
    for (int i = 0; i < board_size; ++i)
    {
        for (int j = 0; j < board_size; ++j)
        {
            if (board[i][j].checked)
            {
                continue;
            }

            bool region_board[board_size][board_size] = {};
            Region new_region;
            new_region.plant = board[i][j].plant;
            calculateRegion({i, j}, {i, j}, new_region, board, directions, region_board);

            int horizontal_sides = calculateRegionHorizontalSides(region_board);
            int vertical_sides = calculateRegionVerticalSides(region_board);
            new_region.sides = horizontal_sides + vertical_sides;

            total_price += new_region.area * new_region.perimeter;
            new_total_price += new_region.area * new_region.sides;
        }
    }

    std::cout << "The total price of the garden is: " << total_price << std::endl;
    std::cout << "The new total price of the garden is: " << new_total_price << std::endl;
}
