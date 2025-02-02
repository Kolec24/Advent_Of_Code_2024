#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>
#include<map>

constexpr int max_press = 100;
constexpr int a_cost = 3;
constexpr int b_cost = 1;

struct State
{
    long long current_cost = 0;

    long long current_x = 0;
    long long current_y = 0;

    bool blocked = false;

    State* next_a_state;
    State* next_b_state;
};

struct Button
{
    int x = 0;
    int y = 0;
};

struct Machine
{
    long long prize_x = 0;
    long long prize_y = 0;

    Button a;
    Button b;
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

    std::string current_line;
    std::string current_string;
    Button current_button_a;
    Button current_button_b;
    std::vector<Machine> machines;
    while (getline(infile, current_line))
    {
        std::istringstream line_stream(current_line);
        std::vector<std::string> string_vector{std::istream_iterator<std::string>(line_stream), std::istream_iterator<std::string>()};
        if (string_vector.empty())
        {
            continue;
        }
        
        if (string_vector[1] == "A:")
        {
            string_vector[2].pop_back();
            std::string x_string = string_vector[2].substr(2);
            int x = std::stoi(x_string);
            std::string y_string = string_vector[3].substr(2);
            int y = std::stoi(y_string);

            current_button_a.x = x;
            current_button_a.y = y;
        }
        if (string_vector[1] == "B:")
        {
            string_vector[2].pop_back();
            std::string x_string = string_vector[2].substr(2);
            int x = std::stoi(x_string);
            std::string y_string = string_vector[3].substr(2);
            int y = std::stoi(y_string);

            current_button_b.x = x;
            current_button_b.y = y;
        }
        if (string_vector[0] == "Prize:")
        {
            string_vector[1].pop_back();
            std::string x_string = string_vector[1].substr(2);
            long long x = std::stoll(x_string);
            std::string y_string = string_vector[2].substr(2);
            long long y = std::stoll(y_string);

            Machine new_machine;
            new_machine.a = current_button_a;
            new_machine.b = current_button_b;
            new_machine.prize_x = x;
            new_machine.prize_y = y;

            machines.push_back(new_machine);
        }
    }

    // Part 1.
    int total_token_count = 0;
    for (const auto& machine : machines)
    {
        bool possible = false;
        int min_token_count = max_press * 10;
        for (int a = 1; a <= max_press; ++a)
        {
            for (int b = 1; b <= max_press; ++b)
            {
                int x = a * machine.a.x + b * machine.b.x;
                int y = a * machine.a.y + b * machine.b.y;
                if (machine.prize_x == x && machine.prize_y == y)
                {
                    possible = true;
                    int cost = a * a_cost + b * b_cost;
                    if (cost < min_token_count)
                    {
                        min_token_count = cost;
                    }
                }
            }
        }

        if (!possible)
        {
            continue;
        }

        total_token_count += min_token_count;
    }

    std::cout << "The fewest tokens to win all possible prizes is: " << total_token_count << std::endl;

    // Part 2.
    for (auto& machine : machines)
    {
        machine.prize_x += 10000000000000;
        machine.prize_y += 10000000000000;
    }


    long long new_token_count = 0;
    for (const auto& machine : machines)
    {
        int a_x = machine.a.x;
        int a_y = machine.a.y;
        int b_x = machine.b.x;
        int b_y = machine.b.y;
        long long p_x = machine.prize_x;
        long long p_y = machine.prize_y;

        long long determinant = a_x * b_y - b_x * a_y;

        if (determinant == 0)
        {
            continue;
        }

        long long a_determinant = p_x * b_y - b_x * p_y;
        long long b_determinant = a_x * p_y - p_x * a_y;

        long long a_remainder = a_determinant % determinant;
        long long b_remainder = b_determinant % determinant;
        if (a_remainder != 0 || b_remainder != 0)
        {
            continue;
        }

        long long a_solution = a_determinant / determinant;
        long long b_solution = b_determinant / determinant;

        new_token_count += a_solution * a_cost + b_solution * b_cost; 
    }

    std::cout << "The fewest tokens to win all updated possible prizes is: " << new_token_count << std::endl;
}
