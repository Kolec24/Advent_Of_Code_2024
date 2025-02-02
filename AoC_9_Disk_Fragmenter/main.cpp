#include<iostream>
#include<fstream>
#include<vector>
#include<map>

int main()
{
    std::vector<std::pair<size_t, int>> free_map;
    std::map<size_t, int> memory_map;
    std::vector<size_t> free_indices;
    std::vector<int> memory;

    std::ifstream infile;
    infile.open("puzzle_input.txt");
    if (!infile.is_open())
    {
        std::cout << "File did not open!" << std::endl;
        return -1;
    }

    char current_char;
    int oddity_counter = 1;
    int file_id = 0;
    while (infile >> current_char)
    {
        if (!isdigit(current_char))
        {
            std::cout << "A character is not a number!" << std::endl;
            return -1;
        }

        int current_int = current_char - '0';
        if (oddity_counter > 0)
        {
            // File number.
            memory_map.insert(std::pair<size_t, int>(memory.size(), current_int));

            for (int i = 0; i < current_int; ++i)
            {
                memory.push_back(file_id);
            }

            file_id++;
            oddity_counter *= -1;
        }
        else
        {
            // Free number.
            free_map.push_back(std::pair<size_t, int>(memory.size(), current_int));

            for (int i = 0; i < current_int; ++i)
            {
                free_indices.push_back(memory.size());
                memory.push_back(-1);
            }

            oddity_counter *= -1;
        }
    }

    // Part 1.
    std::vector<int> memory_copy = memory;
    std::vector<size_t> free_indices_copy = free_indices;
    for (size_t i = memory_copy.size() - 1; i >= 0; --i)
    {
        if (free_indices_copy.empty())
        {
            break;
        }

        int current_number = memory_copy[i];
        if (current_number < 0)
        {
            continue;
        }

        size_t next_free_index = free_indices_copy[0];
        if (memory_copy.size() < next_free_index - 1)
        {
            std::cout << "Memory is smaller than the free index!" << std::endl;
            return -1;
        }

        if (next_free_index >= i)
        {
            break;
        }

        memory_copy[next_free_index] = current_number;
        memory_copy[i] = -1;
        free_indices_copy.erase(free_indices_copy.begin());
    }

    long long total_sum = 0;
    for (int i = 0; i < memory_copy.size(); ++i)
    {
        if (memory_copy[i] < 0)
        {
            break;
        }

        total_sum += i * memory_copy[i];
    }

    std::cout << "The resulting filesystem checksum is: " << total_sum << std::endl;

    // Part 2.
    for (auto file = memory_map.rbegin(); file != memory_map.rend(); ++file)
    {
        for (auto& free_block : free_map)
        {
            if (free_block.first > file->first)
            {
                break;
            }

            if (free_block.second < file->second)
            {
                continue;
            }

            if (memory.size() < file->first + file->second)
            {
                std::cout << "Memory is smaller than file's end location!" << std::endl;
                return -1;
            }

            if (memory.size() < free_block.first + free_block.second)
            {
                std::cout << "Memory is smaller than free block's end location!" << std::endl;
                return -1;
            }

            int current_file_id = memory[file->first];
            for (int i = 0; i < file->second; ++i)
            {
                memory[free_block.first + i] = current_file_id;
                memory[file->first + i] = -1;
            }

            free_block.first += file->second;
            free_block.second -= file->second;
            if (free_block.first > memory.size())
            {
                std::cout << "The new free block is farther than memory's size!" << std::endl;
                return -1;
            }

            if (free_block.second < 0)
            {
                std::cout << "A new free block has a negative size!" << std::endl;
                return -1;
            }

            break;
        }
    }

    long long new_total_sum = 0;
    for (int i = 0; i < memory.size(); ++i)
    {
        if (memory[i] < 0)
        {
            continue;
        }

        new_total_sum += i * memory[i];
    }

    std::cout << "The new resulting filesystem checksum is: " << new_total_sum << std::endl;
}
