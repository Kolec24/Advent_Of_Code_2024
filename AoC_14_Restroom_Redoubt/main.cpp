#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>

constexpr int number_of_seconds = 100;
constexpr int board_x = 101;
constexpr int board_y = 103;

struct Robot
{
    int pos_x = 0;
    int pos_y = 0;

    int vel_x = 0;
    int vel_y = 0;
};

int main()
{
    std::ifstream infile;
    infile.open("puzzle_input.txt");
    if (!infile.is_open())
    {
        std::cout << "File did not open!" << std::endl;
        return -1;
    }

    std::vector<Robot> robots;
    std::string current_line;
    std::string current_string;
    while (getline(infile, current_line))
    {
        std::istringstream line_stream(current_line);
        std::vector<std::string> string_vector{std::istream_iterator<std::string>(line_stream), std::istream_iterator<std::string>()};

        if (string_vector.size() != 2)
        {
            std::cout << "Line has more than two strings!" << std::endl;
            continue;
        }
        
        Robot new_robot;
        for (int i = 0; i < string_vector.size(); ++i)
        {
            std::string current_string = string_vector[i];
            if (current_string[0] == 'p')
            {
                std::string x_string, y_string;
                std::string pos_string = current_string.substr(2);
                bool second_coordinate = false;
                for (auto character : pos_string)
                {
                    if (isdigit(character))
                    {
                        if (!second_coordinate)
                        {
                            x_string += character;
                        }
                        else
                        {
                            y_string+= character;
                        }
                    }
                    else
                    {
                        second_coordinate = true;
                    }
                }

                new_robot.pos_x = std::stoi(x_string);
                new_robot.pos_y = std::stoi(y_string);
            }
            if (current_string[0] == 'v')
            {
                std::string x_string, y_string;
                std::string vel_string = current_string.substr(2);
                int x_sign = 1;
                int y_sign = 1;
                bool second_coordinate = false;
                for (auto character : vel_string)
                {
                    if (isdigit(character))
                    {
                        if (!second_coordinate)
                        {
                            x_string += character;
                        }
                        else
                        {
                            y_string+= character;
                        }
                    }
                    else if (character == '-')
                    {
                        if (!second_coordinate)
                        {
                            x_sign = -1;
                        }
                        else
                        {
                            y_sign = -1;
                        }
                    }
                    else
                    {
                        second_coordinate = true;
                    }
                }

                new_robot.vel_x = x_sign * std::stoi(x_string);
                new_robot.vel_y = y_sign * std::stoi(y_string);
            }
        }

        robots.push_back(new_robot);
    }

    // Part 1.
    int quadrants[4] = {};
    for (auto robot : robots)
    {
        int new_x = robot.pos_x + robot.vel_x * number_of_seconds;
        int new_y = robot.pos_y + robot.vel_y * number_of_seconds;

        int remainder_x = (new_x % board_x + board_x) % board_x;
        int remainder_y = (new_y % board_y + board_y) % board_y;

        if (remainder_x == board_x / 2)
        {
            continue;
        }
        if (remainder_y == board_y / 2)
        {
            continue;
        }

        if (remainder_x < board_x / 2)
        {
            if (remainder_y < board_y / 2)
            {
                quadrants[0]++;
            }
            else
            {
                quadrants[1]++;
            }
        }
        else
        {
            if (remainder_y < board_y / 2)
            {
                quadrants[2]++;
            }
            else
            {
                quadrants[3]++;
            }
        }
    }

    int safety_factor = 1;
    for (int i = 0; i < 4; ++i)
    {
        safety_factor *= quadrants[i];
    }

    std::cout << "Total safety counter is: " << safety_factor << std::endl;

    // Part 2.
    char board[board_y][board_x];
    int counter = 0;
    while (counter < 6620)
    {
        counter++;
        for (int i = 0; i < board_y; ++i)
        {
            for (int j = 0; j < board_x; ++j)
            {
                board[i][j] = ' ';
            }
        }

        for (auto& robot : robots)
        { 
            int new_x = robot.pos_x + robot.vel_x;
            int new_y = robot.pos_y + robot.vel_y;

            int remainder_x = (new_x % board_x + board_x) % board_x;
            int remainder_y = (new_y % board_y + board_y) % board_y; 

            robot.pos_x = remainder_x;
            robot.pos_y = remainder_y;
            board[remainder_y][remainder_x] = 'X';
        }

        if (counter == 6620)
        {
            std::cout << "After " << counter << " seconds:" << std::endl;
            for (int i = 0; i < board_y; ++i)
            {
                for (int j = 0; j < board_x; ++j)
                {
                    std::cout << board[i][j];
                }
                std::cout << std::endl;
            }
        }
    }
}
