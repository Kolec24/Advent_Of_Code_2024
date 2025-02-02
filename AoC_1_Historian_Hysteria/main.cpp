#include<iostream>
#include<fstream>
#include<cmath>
#include<vector>
#include<algorithm>

int main()
{
    std::vector<int> left;
    std::vector<int> right;

    std::ifstream in;
    in.open("puzzle_input.txt");
    if (!in.is_open())
    {
        std::cout << "File did not open!" << std::endl;
        return -1;
    }

    int current_number = 0;
    int counter = 1;
    while (in >> current_number)
    {
        // This should be populated in a better way!
        if (counter > 0)
        {
            left.push_back(current_number);
        }
        else
        {
            right.push_back(current_number);
        }

        counter *= -1;
    }

    if (left.size() != right.size())
    {
        std::cout << "Arrays are of different size! Returning." << std::endl;
        return -1;
    }

    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());

    // Part 1 of the puzzle.
    int total_distance = 0;
    for (int i = 0; i < left.size(); ++i)
    {
       total_distance += abs(left[i] - right[i]);
    }

    std::cout << "The total distance between given arrays is " << total_distance << std::endl;

    // Part 2 of the puzzle.
    int similarity_score = 0;
    int index_to_start = 0;
    for (const int& left_number : left)
    {
        int instance_counter = 0;
        for (int i = index_to_start; i < right.size(); ++i)
        {
            if (right[i] < left_number)
            {
                continue;
            }

            if (right[i] == left_number)
            {
                instance_counter++;
            }
            else if (right[i] > left_number)
            {
                index_to_start = i;
                similarity_score += left_number * instance_counter;
                break;
            }
        }
    }

    std::cout << "The similarity score between given arrays is " << similarity_score << std::endl;

    return 0;
}

