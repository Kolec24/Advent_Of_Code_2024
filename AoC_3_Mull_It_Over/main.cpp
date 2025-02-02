#include<iostream>
#include<fstream>
#include<string>

int main()
{
    std::ifstream infile;
    infile.open("puzzle_input.txt");
    if (!infile.is_open())
    {
        std::cout << "File did not open!" << std::endl;
        return -1;
    }

    // Part 1.
    // char current_char;
    // int acceptable_characters;
    // int comma_index;
    // std::string first_string;
    // std::string second_string;
    // int first_number;
    // int second_number;
    // int total_sum = 0;
    // while(infile >> current_char)
    // {
        // TODO: This sounds stupid, find a better way.
    //     if (current_char == 'm')
    //     {
    //         acceptable_characters = 1;
    //         comma_index = 0;
    //         first_string.clear();
    //         second_string.clear();
    //         first_number = 0;
    //         second_number = 0;
    //     }
    //     else if (current_char == 'u' && acceptable_characters == 1)
    //     {
    //         acceptable_characters = 2;
    //     }
    //     else if (current_char == 'l' && acceptable_characters == 2)
    //     {
    //         acceptable_characters = 3;
    //     }
    //     else if (current_char == '(' && acceptable_characters == 3)
    //     {
    //         acceptable_characters = 4;
    //     }
    //     else if (isdigit(current_char) && acceptable_characters >= 4 && acceptable_characters <= 6 && comma_index == 0)
    //     {
    //         acceptable_characters++;
    //         first_string += current_char;
    //     }
    //     else if (current_char == ',' && acceptable_characters >= 5 && acceptable_characters <= 7 && comma_index == 0)
    //     {
    //         acceptable_characters++;
    //         comma_index = acceptable_characters;
    //         first_number = std::stoi(first_string);
    //     }
    //     else if (isdigit(current_char) && comma_index != 0 && acceptable_characters >= comma_index && acceptable_characters <= comma_index + 2)
    //     {
    //         acceptable_characters++;
    //         second_string += current_char;
    //     }
    //     else if (current_char == ')' && comma_index != 0 && acceptable_characters >= comma_index + 1 && acceptable_characters <= comma_index + 3)
    //     {
    //         second_number = std::stoi(second_string);
    //         total_sum += first_number * second_number;
    //
    //        acceptable_characters = 0;
    //        comma_index = 0;
    //        first_string.clear();
    //        second_string.clear();
    //        first_number = 0;
    //        second_number = 0;
    //    }
    //    else
    //    {
    //        acceptable_characters = 0;
    //        comma_index = 0;
    //        first_string.clear();
    //        second_string.clear();
    //        first_number = 0;
    //        second_number = 0;
    //    }
    // }

    // std::cout << "The total sum of proper multiplications is: " << total_sum << std::endl;

    // Part 2.
    char current_char;
    int acceptable_characters = 0;
    int comma_index = 0;
    std::string first_string;
    std::string second_string;
    int first_number = 0;
    int second_number = 0;
    int total_sum = 0;
    bool enabled = true;
    int enable_characters = 0;
    while(infile >> current_char)
    {
        if (!enabled)
        {
            // If we are disabled, then nothing matters except finding do().
            if (current_char == 'd')
            {
                enable_characters = 1;
            }
            else if (current_char == 'o' && enable_characters == 1)
            {
                enable_characters = 2;
            }
            else if (current_char == '(' && enable_characters == 2)
            {
                enable_characters = 3;
            }
            else if (current_char == ')' && enable_characters == 3)
            {
                enabled = true;
                enable_characters = 0;
            }
            else
            {
                enable_characters = 0;
            }


            continue;
        }

        // Check for disable.
        if (current_char == 'd')
        {
            acceptable_characters = 0;
            comma_index = 0;
            first_string.clear();
            second_string.clear();
            first_number = 0;
            second_number = 0;

            enable_characters = 1;
        }
        else if (current_char == 'o' && enable_characters == 1)
        {
            enable_characters = 2;
        }
        else if (current_char == 'n' && enable_characters == 2)
        {
            enable_characters = 3;
        }
        else if (current_char == '\'' && enable_characters == 3)
        {
            enable_characters = 4;
        }
        else if (current_char == 't' && enable_characters == 4)
        {
            enable_characters = 5;
        }
        else if (current_char == '(' && enable_characters == 5)
        {
            enable_characters = 6;
        }
        else if (current_char == ')' && enable_characters == 6)
        {
            enabled = false;
            enable_characters = 0;
        }
        else if (current_char == 'm')
        {
            acceptable_characters = 1;
            comma_index = 0;
            first_string.clear();
            second_string.clear();
            first_number = 0;
            second_number = 0;
            enable_characters = 0;
        }
        else if (current_char == 'u' && acceptable_characters == 1)
        {
            acceptable_characters = 2;
        }
        else if (current_char == 'l' && acceptable_characters == 2)
        {
            acceptable_characters = 3;
        }
        else if (current_char == '(' && acceptable_characters == 3)
        {
            acceptable_characters = 4;
        }
        else if (isdigit(current_char) && acceptable_characters >= 4 && acceptable_characters <= 6 && comma_index == 0)
        {
            acceptable_characters++;
            first_string += current_char;
        }
        else if (current_char == ',' && acceptable_characters >= 5 && acceptable_characters <= 7 && comma_index == 0)
        {
            acceptable_characters++;
            comma_index = acceptable_characters;
            first_number = std::stoi(first_string);
        }
        else if (isdigit(current_char) && comma_index != 0 && acceptable_characters >= comma_index && acceptable_characters <= comma_index + 2)
        {
            acceptable_characters++;
            second_string += current_char;
        }
        else if (current_char == ')' && comma_index != 0 && acceptable_characters >= comma_index + 1 && acceptable_characters <= comma_index + 3)
        {
            second_number = std::stoi(second_string);
            total_sum += first_number * second_number;
    
            acceptable_characters = 0;
            comma_index = 0;
            first_string.clear();
            second_string.clear();
            first_number = 0;
            second_number = 0;
        }
        else
        {
            acceptable_characters = 0;
            comma_index = 0;
            first_string.clear();
            second_string.clear();
            first_number = 0;
            second_number = 0;
        }
    }

    std::cout << "The total sum of proper multiplications is: " << total_sum << std::endl;
}
