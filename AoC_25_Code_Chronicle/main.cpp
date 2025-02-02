#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

constexpr int size = 7;
std::vector<std::vector<std::vector<char>>> keys;
std::vector<std::vector<std::vector<char>>> locks;

size_t getColumnLength(const std::vector<char> &column, bool is_key)
{
    size_t counter = 0;
    char desired_char = is_key ? '.' : '#';
    for (const auto &character : column)
    {
        if (character == desired_char)
        {
            counter++;
            continue;
        }

        return is_key ? column.size() - counter : counter;
    }

    std::cout << "getColumnLength() did not find a character change!"
              << std::endl;
    return 0;
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
    bool starting_new = true;
    bool is_key = false;
    bool in_progress = false;
    std::vector<std::vector<char>> current_piece = {{}, {}, {}, {}, {}};
    while (getline(infile, current_line))
    {
        if (current_line.empty())
        {
            if (!current_piece.empty())
            {
                if (is_key)
                {
                    keys.push_back(current_piece);
                }
                else
                {
                    locks.push_back(current_piece);
                }

                for (auto &column : current_piece)
                {
                    column.clear();
                }
            }

            in_progress = false;
            starting_new = true;
            continue;
        }

        std::istringstream line_stream(current_line);
        std::vector<char> line_string{std::istream_iterator<char>(line_stream),
                                      std::istream_iterator<char>()};

        if (starting_new)
        {
            starting_new = false;
            is_key = line_string[0] == '.';
            in_progress = true;
        }

        if (in_progress)
        {
            if (line_string.size() != current_piece.size())
            {
                std::cout << "More characters (" << line_string.size()
                          << ") than columns (" << current_piece.size() << ")!"
                          << std::endl;
                return -1;
            }
            for (size_t i = 0; i < line_string.size(); ++i)
            {
                current_piece[i].push_back(line_string[i]);
            }
        }
    }

    if (is_key)
    {
        keys.push_back(current_piece);
    }
    else
    {
        locks.push_back(current_piece);
    }

    int counter = 0;
    for (const auto &key : keys)
    {
        for (const auto &lock : locks)
        {
            bool valid_pair = true;
            for (size_t i = 0; i < key.size(); ++i)
            {
                size_t key_column = getColumnLength(key[i], true);
                size_t lock_column = getColumnLength(lock[i], false);
                if (key_column + lock_column > size)
                {
                    valid_pair = false;
                    break;
                }
            }

            if (valid_pair)
            {
                counter++;
            }
        }
    }

    std::cout << "The number of pairs that fit together without overlapping in "
                 "any column is: "
              << counter << std::endl;
    return 0;
}
