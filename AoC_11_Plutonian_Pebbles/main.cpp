#include<iostream>
#include<fstream>
#include<vector>
#include<map>

void addPebble(const long long& pebble, const long long& count, std::map<long long, long long>& pebbles)
{
    if (pebbles.count(pebble) == 0)
    {
        pebbles.insert({pebble, count});
    }
    else
    {
        pebbles[pebble] += count;
    }
}

int numDigits(long long number)
{
    int digits = 0;
    while (number)
    {
        number /= 10;
        digits++;
    }

    return digits;
}

long long getStoneCount(std::map<long long, long long>& pebbles, int number_of_blinks)
{
    if (number_of_blinks == 0)
    {
        long long stone_counter = 0;
        for (const auto& pebble : pebbles)
        {
            std::cout << "Pebble: " << pebble.first << " appeared " << pebble.second << " times!" << std::endl;
            stone_counter += pebble.second;
        }

        std::cout << "Number of pebbles: " << pebbles.size() << std::endl;

        return stone_counter;
    }

    std::map<long long, long long> new_pebbles;
    for (const auto& pebble : pebbles)
    {
        if (pebble.first == 0)
        {
            addPebble(1, pebble.second, new_pebbles);
            continue;
        }

        int pebble_digits = numDigits(pebble.first);
        if (pebble_digits % 2 != 0)
        {
            addPebble(pebble.first * 2024, pebble.second, new_pebbles);
            continue;
        }

        int half_length = pebble_digits / 2;
        long long first_number = pebble.first / static_cast<long long>(pow(10.f, half_length));
        long long second_number = pebble.first % static_cast<long long>(pow(10.f, half_length));

        addPebble(first_number, pebble.second, new_pebbles);
        addPebble(second_number, pebble.second, new_pebbles);
    }


    pebbles.clear();

    std::cout << "Blinks left: " << number_of_blinks - 1 << std::endl;
    return getStoneCount(new_pebbles, number_of_blinks - 1);
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

    std::vector<long long> pebbles_vector{std::istream_iterator<long long>{infile}, std::istream_iterator<long long>{}};
    std::map<long long, long long> pebbles;
    for (const auto& pebble : pebbles_vector)
    {
        addPebble(pebble, 1, pebbles);
    }

    // Part 1 & 2.
    long long total_stone_count = 0;
    int number_of_blinks = 75;
    total_stone_count = getStoneCount(pebbles, number_of_blinks);

    std::cout << "The number of stones after " << number_of_blinks << " blinks is: " << total_stone_count << std::endl;
}
