#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>
#include<map>
#include<unordered_map>

struct Code
{
    std::string code;

    std::string last_robot_input;
    std::string first_good_robot_inputs;

    long long final_length = 0;
    std::vector<std::string> robot_inputs;
};

constexpr int length_iterations = 11;
constexpr int number_of_robots = 26;
std::vector<Code> codes;
std::map<char, std::pair<int, int>> directions = {{'>', {0, 1}}, {'v', {1, 0}}, {'<', {0, -1}}, {'^', {-1, 0}}};
std::map<char, std::pair<int, int>> directional_keypad = {{'^', {0, 1}}, {'A', {0, 2}}, {'<', {1, 0}}, {'v', {1, 1}}, {'>', {1, 2}}};
std::map<char, std::pair<int, int>> numeric_keypad = {{'7', {0, 0}}, {'8', {0, 1}}, {'9', {0, 2}},
    {'4', {1, 0}}, {'5', {1, 1}}, {'6', {1, 2}}, 
    {'1', {2, 0}}, {'2', {2, 1}}, {'3', {2, 2}},
    {'0', {3, 1}}, {'A', {3, 2}}};

std::unordered_map<std::string, std::vector<char>> input_expansion;
std::unordered_map<std::string, size_t> input_expansion_after_1;

bool table[4] = {true, true, false, false};
bool getVerticalFirst(int horizontal, int vertical)
{
    if (horizontal == 0)
    {
        return true;
    }

    if (vertical == 0)
    {
        return false;
    }

    if (horizontal > 0 && vertical > 0)
    {
        return table[0];
    }
    if (horizontal > 0 && vertical < 0)
    {
        return table[1];
    }
    if (horizontal < 0 && vertical > 0)
    {
        return table[2];
    }
    if (horizontal < 0 && vertical < 0)
    {
        return table[3];
    }

    return false;
}

bool areInputsGood(std::vector<char> inputs, std::pair<int, int> starting_position, bool numerical)
{
    std::pair<int, int> wrong_position = {0, 0};
    if (numerical)
    {
        wrong_position = {3, 0};
    }

    std::pair<int, int> current_position = starting_position;
    for (const char& entry : inputs)
    {
        auto direction = directions[entry];
        std::pair<int, int> next_position = {current_position.first + direction.first, current_position.second + direction.second};
        if (next_position == wrong_position)
        {
            return false;
        }

        current_position = next_position;
    }

    return true;
}

void computeInputExpansion()
{
    std::vector<std::string> strings = {"<A", ">A", "vA", "^A", "A", "<vA", "v<A",  ">^A", "^>A", ">vA", "v>A", "<^A", "^<A", "<<A", ">>A", "v<<A", ">>^A"};
    for (const std::string& string : strings)
    {
        char current_character = 'A';
        std::vector<char> full_string_input;
        for (const char& character : string)
        {
            std::vector<char> next_input;
            
            std::pair<int, int> current_position = directional_keypad[current_character];
            std::pair<int, int> next_position = directional_keypad[character];
            int vertical = next_position.first - current_position.first;
            int horizontal = next_position.second - current_position.second;
            int steps = std::abs(vertical) + std::abs(horizontal);

            char vertical_char = vertical > 0 ? 'v' : '^';
            char horizontal_char = horizontal > 0 ? '>' : '<';

            bool vertical_first = getVerticalFirst(horizontal, vertical);
            bool completed = false;
            if (vertical_first)
            {
                std::vector<char> possible_next_input;
                for (int i = 0; i < steps; ++i)
                {
                    if (i < std::abs(vertical))
                    {
                        possible_next_input.push_back(vertical_char);
                    }
                    else
                    {
                        possible_next_input.push_back(horizontal_char);
                    }
                }

                if (areInputsGood(possible_next_input, current_position, false))
                {
                    completed = true;
                    next_input = possible_next_input;
                }
            }

            if (vertical_first && !completed)
            {
                std::vector<char> possible_next_input;
                for (int i = 0; i < steps; ++i)
                {
                    if (i < std::abs(horizontal))
                    {
                        possible_next_input.push_back(horizontal_char);
                    }
                    else
                    {
                        possible_next_input.push_back(vertical_char);
                    }
                }

                if (areInputsGood(possible_next_input, current_position, false))
                {
                    completed = true;
                    next_input = possible_next_input;
                }
            }

            if (!vertical_first)
            {
                std::vector<char> possible_next_input;
                for (int i = 0; i < steps; ++i)
                {
                    if (i < std::abs(horizontal))
                    {
                        possible_next_input.push_back(horizontal_char);
                    }
                    else
                    {
                        possible_next_input.push_back(vertical_char);
                    }
                }

                if (areInputsGood(possible_next_input, current_position, false))
                {
                    completed = true;
                    next_input = possible_next_input;
                }
            }

            if (!vertical_first && !completed)
            {
                std::vector<char> possible_next_input;
                for (int i = 0; i < steps; ++i)
                {
                    if (i < std::abs(vertical))
                    {
                        possible_next_input.push_back(vertical_char);
                    }
                    else
                    {
                        possible_next_input.push_back(horizontal_char);
                    }
                }

                if (areInputsGood(possible_next_input, current_position, false))
                {
                    completed = true;
                    next_input = possible_next_input;
                }
            }

            if (!completed)
            {
                std::cout << "Neither horizontal nor vertical worked!" << std::endl;
                return;
            }

            current_character = character;
            next_input.push_back('A');
            full_string_input.insert(full_string_input.end(), next_input.begin(), next_input.end());
        }

        input_expansion[string] = full_string_input;
    }
}

std::vector<char> getStepInputs(const std::string& key)
{
    return input_expansion[key];
}

void computeInputExpansionLength()
{
    std::vector<std::string> strings = {"<A", ">A", "vA", "^A", "A", "<vA", "v<A",  ">^A", "^>A", ">vA", "v>A", "<^A", "^<A", "<<A", ">>A", "v<<A", ">>^A"};
    for (const std::string& string : strings)
    {
        int counter = 0;
        std::string current_string = string;
        while (counter < length_iterations)
        {
            std::string final_result;
            std::istringstream input_stream(current_string);
            std::string subkey;
            std::vector<std::string> subkeys;
            while (std::getline(input_stream, subkey, 'A'))
            {
                subkey.push_back('A');
                subkeys.push_back(subkey);
            }

            for (const auto& substring : subkeys)
            {
                std::vector<char> next_inputs = getStepInputs(substring);
                final_result.insert(final_result.end(), next_inputs.begin(), next_inputs.end());
            }

            current_string = final_result;
            counter++;
        }

        input_expansion_after_1[string] = current_string.size();
    }
}

size_t getInputLength(std::string input)
{
    size_t counter = 0;
    std::string final_result;
    std::istringstream input_stream(input);
    std::string subkey;
    std::vector<std::string> subkeys;
    while (std::getline(input_stream, subkey, 'A'))
    {
        subkey.push_back('A');
        subkeys.push_back(subkey);
    }

    for (const auto& substring : subkeys)
    {
        counter += input_expansion_after_1[substring];
    }

    return counter;
}

void getPreviousInputs(std::string& robot_input)
{
    std::string final_result;
    std::istringstream input_stream(robot_input);
    std::string subkey;
    std::vector<std::string> subkeys;
    while (std::getline(input_stream, subkey, 'A'))
    {
        subkey.push_back('A');
        subkeys.push_back(subkey);
    }

    for (const auto& substring : subkeys)
    {
        std::vector<char> next_inputs = getStepInputs(substring);
        final_result.insert(final_result.end(), next_inputs.begin(), next_inputs.end());
    }

    robot_input = final_result;
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
        Code code;
        code.code = current_line;
        codes.push_back(code);
    }

    // Fill the last robot movements.
    for (auto& code : codes)
    {
        std::pair<int, int> current_position = numeric_keypad['A'];
        for (const char& entry : code.code)
        {
            if (numeric_keypad.count(entry) == 0)
            {
                std::cout << "Code entry is not on a keypad!" << std::endl;
                return -1;
            }

            std::pair<int, int> next_position = numeric_keypad[entry];

            int vertical = next_position.first - current_position.first;
            int horizontal = next_position.second - current_position.second;
            int steps = std::abs(vertical) + std::abs(horizontal);

            char vertical_char = vertical > 0 ? 'v' : '^';
            char horizontal_char = horizontal > 0 ? '>' : '<';
        
            bool vertical_first = getVerticalFirst(horizontal, vertical);
            bool completed = false;
            if (vertical_first)
            {
                std::vector<char> next_inputs;
                for (int i = 0; i < steps; ++i)
                {
                    if (i < std::abs(vertical))
                    {
                        next_inputs.push_back(vertical_char);
                    }
                    else
                    {
                        next_inputs.push_back(horizontal_char);
                    }
                }

                if (areInputsGood(next_inputs, current_position, true))
                {
                    completed = true;
                    code.last_robot_input.insert(std::end(code.last_robot_input), std::begin(next_inputs), std::end(next_inputs));
                }
            }

            if (vertical_first && !completed)
            {
                std::vector<char> next_inputs;
                for (int i = 0; i < steps; ++i)
                {
                    if (i < std::abs(horizontal))
                    {
                        next_inputs.push_back(horizontal_char);
                    }
                    else
                    {
                        next_inputs.push_back(vertical_char);
                    }
                }

                if (areInputsGood(next_inputs, current_position, true))
                {
                    completed = true;
                    code.last_robot_input.insert(std::end(code.last_robot_input), std::begin(next_inputs), std::end(next_inputs));
                }
            }

            if (!vertical_first)
            {
                std::vector<char> next_inputs;
                for (int i = 0; i < steps; ++i)
                {
                    if (i < std::abs(horizontal))
                    {
                        next_inputs.push_back(horizontal_char);
                    }
                    else
                    {
                        next_inputs.push_back(vertical_char);
                    }
                }

                if (areInputsGood(next_inputs, current_position, true))
                {
                    completed = true;
                    code.last_robot_input.insert(std::end(code.last_robot_input), std::begin(next_inputs), std::end(next_inputs));
                }
            }

            if (!vertical_first && !completed)
            {
                std::vector<char> next_inputs;
                for (int i = 0; i < steps; ++i)
                {
                    if (i < std::abs(vertical))
                    {
                        next_inputs.push_back(vertical_char);
                    }
                    else
                    {
                        next_inputs.push_back(horizontal_char);
                    }
                }

                if (areInputsGood(next_inputs, current_position, true))
                {
                    completed = true;
                    code.last_robot_input.insert(std::end(code.last_robot_input), std::begin(next_inputs), std::end(next_inputs));
                }
            }

            if (!completed)
            {
                std::cout << "Neither horizontal nor vertical worked!" << std::endl;
                return -1;
            }

            code.last_robot_input.push_back('A');
            current_position = next_position;
        }
    }

    std::cout << "Finished last robot movements!" << std::endl;
    computeInputExpansion();
    computeInputExpansionLength();
    std::cout << "Computed input expansion!" << std::endl;

    // Fill the first "good" inputs.
    for (auto& code : codes)
    {
        std::pair<int, int> current_position = directional_keypad['A'];
        for (const char& entry : code.last_robot_input)
        {
            if (directional_keypad.count(entry) == 0)
            {
                std::cout << "Code entry is not on a keypad!" << std::endl;
                return -1;
            }

            std::pair<int, int> next_position = directional_keypad[entry];

            int vertical = next_position.first - current_position.first;
            int horizontal = next_position.second - current_position.second;
            int steps = std::abs(vertical) + std::abs(horizontal);

            char vertical_char = vertical > 0 ? 'v' : '^';
            char horizontal_char = horizontal > 0 ? '>' : '<';
        
            bool vertical_first = getVerticalFirst(horizontal, vertical);
            bool completed = false;
            if (vertical_first)
            {
                std::vector<char> next_inputs;
                for (int i = 0; i < steps; ++i)
                {
                    if (i < std::abs(vertical))
                    {
                        next_inputs.push_back(vertical_char);
                    }
                    else
                    {
                        next_inputs.push_back(horizontal_char);
                    }
                }

                if (areInputsGood(next_inputs, current_position, false))
                {
                    completed = true;
                    code.first_good_robot_inputs.insert(std::end(code.first_good_robot_inputs), std::begin(next_inputs), std::end(next_inputs));
                }
            }

            if (vertical_first && !completed)
            {
                std::vector<char> next_inputs;
                for (int i = 0; i < steps; ++i)
                {
                    if (i < std::abs(horizontal))
                    {
                        next_inputs.push_back(horizontal_char);
                    }
                    else
                    {
                        next_inputs.push_back(vertical_char);
                    }
                }

                if (areInputsGood(next_inputs, current_position, false))
                {
                    completed = true;
                    code.first_good_robot_inputs.insert(std::end(code.first_good_robot_inputs), std::begin(next_inputs), std::end(next_inputs));
                }
            }

            if (!vertical_first)
            {
                std::vector<char> next_inputs;
                for (int i = 0; i < steps; ++i)
                {
                    if (i < std::abs(horizontal))
                    {
                        next_inputs.push_back(horizontal_char);
                    }
                    else
                    {
                        next_inputs.push_back(vertical_char);
                    }
                }

                if (areInputsGood(next_inputs, current_position, false))
                {
                    completed = true;
                    code.first_good_robot_inputs.insert(std::end(code.first_good_robot_inputs), std::begin(next_inputs), std::end(next_inputs));
                }
            }

            if (!vertical_first && !completed)
            {
                std::vector<char> next_inputs;
                for (int i = 0; i < steps; ++i)
                {
                    if (i < std::abs(vertical))
                    {
                        next_inputs.push_back(vertical_char);
                    }
                    else
                    {
                        next_inputs.push_back(horizontal_char);
                    }
                }

                if (areInputsGood(next_inputs, current_position, false))
                {
                    completed = true;
                    code.first_good_robot_inputs.insert(std::end(code.first_good_robot_inputs), std::begin(next_inputs), std::end(next_inputs));
                }
            }

            if (!completed)
            {
                std::cout << "Neither horizontal nor vertical worked!" << std::endl;
                return -1;
            }

            code.first_good_robot_inputs.push_back('A');
            current_position = next_position;
        }
    }

    /*for (auto code : codes)
    {
        std::cout << "Last and first robot: ";
        for (auto character : code.last_robot_input)
        {
            std::cout << character;
        }

        std::cout << ": ";
        for (auto character : code.first_good_robot_inputs)
        {
            std::cout << character;
        }
        std::cout << std::endl;
    }*/

    for (auto& code : codes)
    {
        int counter = 0;
        std::string current_input = code.first_good_robot_inputs;
        while (counter < number_of_robots - 2 - length_iterations)
        {
            std::cout << "Code: " << code.code << ". Iteration: " << counter << std::endl;
            getPreviousInputs(current_input);
            counter++;
        }

        //code.final_length = current_input.size();
        code.final_length = getInputLength(current_input);
    }

    long long total_complexity = 0;
    for (size_t i = 0; i < codes.size(); ++i)
    {
        std::string code = codes[i].code;
        code.pop_back();
        int number_code = std::stoi(code);
        total_complexity += codes[i].final_length * number_code;
        std::cout << "Number code = " << number_code << ". Input length = " << codes[i].final_length << std::endl;
    }

    std::cout << "The total complexity of provided codes is: " << total_complexity << std::endl;

    return 0;
}
