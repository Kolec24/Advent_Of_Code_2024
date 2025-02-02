#include<iostream>
#include<fstream>

int find_all_xmas_from_indices(const char (&board)[140][140], int i, int j, const int (&directions)[8][2])
{
    if (board[i][j] != 'X')
    {
        return 0;
    }

    std::string xmas = "XMAS";
    int word_counter = 0;

    int vertical_index, horizontal_index;
    for (int k = 0; k < 8; ++k)
    {
        horizontal_index = directions[k][0];
        vertical_index = directions[k][1];
        if (i + 3 * vertical_index < 0 || i + 3 * vertical_index > 139)
        {
            continue;
        }

        if (j + 3 * horizontal_index < 0 || j + 3 * horizontal_index > 139)
        {
            continue;
        }


        bool fine_direction = true;
        for (int l = 1; l < 4; ++l)
        {
            if (board[i + l * vertical_index][j + l * horizontal_index] != xmas[l])
            {
                fine_direction = false;
                break;
            }
        }

        if (fine_direction)
        {
            word_counter++;
        }
    }

    return word_counter;
}

int find_all_mas_centred_at_indices(const char (&board)[140][140], int i, int j)
{
    if (board[i][j] != 'A')
    {
        return 0;
    }

    if (i == 0 || i == 139 || j == 0 || j == 139)
    {
        return 0;
    }

    int x_counter = 0;

    bool first_direction_forwards = board[i + 1][j + 1] == 'M' && board[i - 1][j - 1] == 'S';
    bool first_direction_backwards = board[i + 1][j + 1] == 'S' && board[i - 1][j - 1] == 'M';
    bool second_direction_forwards = board[i + 1][j - 1] == 'M' && board[i - 1][j + 1] == 'S';
    bool second_direction_backwards = board[i + 1][j - 1] == 'S' && board[i - 1][j + 1] == 'M';
    bool first_direction_correct = first_direction_backwards || first_direction_forwards;
    bool second_direction_correct = second_direction_forwards || second_direction_backwards;
    if (first_direction_correct && second_direction_correct)
    {
        x_counter++;
    }

    return x_counter;
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

    // We know the exact size of our board.
    char board[140][140];
    for (size_t i = 0; i < 140; ++i)
    {
        for (size_t j = 0; j < 140; ++j)
        {
            infile >> board[i][j];
        }
    }

    int all_directions[8][2]{{0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}};

    // Part 1.
    int xmas_counter = 0;
    for (int i = 0; i < 140; ++i)
    {
        for (int j = 0; j < 140; ++j)
        {
            xmas_counter += find_all_xmas_from_indices(board, i, j, all_directions);
        }
    }

    std::cout << "The number of \"XMAS\" appearances is: " << xmas_counter << std::endl;

    // Part 2.
    int x_mas_counter = 0;
    for (int i = 0; i < 140; ++i)
    {
        for (int j = 0; j < 140; ++j)
        {
            x_mas_counter += find_all_mas_centred_at_indices(board, i, j);
        }
    }

    std::cout << "The number of \"X-MAS\" appearances is: " << x_mas_counter << std::endl;
}
