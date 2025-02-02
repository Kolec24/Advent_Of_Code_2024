#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<map>
#include<set>

constexpr int iterations = 2000;
std::vector<long long> secret_numbers;

struct BuyerSequences
{
    std::map<std::vector<int>, int> sequences;
};

long long calculateNextSecretNumber(long long secret_number)
{
    long long multiplication = secret_number * 64;
    long long bitwise_xor = multiplication ^ secret_number;
    long long first_step = bitwise_xor % 16777216;

    long long division = first_step / 32;
    long long second_bitwise_xor = division ^ first_step;
    long long second_step = second_bitwise_xor % 16777216;

    long long final_multiplication = second_step * 2048;
    long long final_xor = final_multiplication ^ second_step;
    long long result = final_xor % 16777216;

    return result;
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
        long long current_number = std::stoll(current_line);
        secret_numbers.push_back(current_number);
    }

    std::vector<std::vector<int>> prices;
    std::vector<long long> result;
    for (auto number : secret_numbers)
    {
        std::vector<int> current_prices;
        current_prices.push_back(number % 10);

        int counter = 0;
        while (counter < iterations)
        {
            long long next_number = calculateNextSecretNumber(number);
            current_prices.push_back(next_number % 10);
            number = next_number;
            ++counter;
        }

        result.push_back(number);
        prices.push_back(current_prices);
    }

    long long total_sum = 0;
    for (auto number : result)
    {
        total_sum += number;
    }

    std::cout << "The total sum of 2000th secret numbers is: " << total_sum << std::endl;

    std::vector<std::vector<int>> price_differences;
    for (const auto& buyer : prices)
    {
        std::vector<int> differences;
        for (int i = 1; i < buyer.size(); ++i)
        {
            differences.push_back(buyer[i] - buyer[i - 1]);
        }

        price_differences.push_back(differences);
    }

    std::vector<BuyerSequences> all_buyer_sequences;
    std::set<std::vector<int>> all_sequences;
    for (int i = 0; i < price_differences.size(); ++i)
    {
        const auto& buyer_differences = price_differences[i];
        BuyerSequences new_sequences;
        for (int j = 0; j < buyer_differences.size() - 3; ++j) 
        {
            std::vector<int> sequence(buyer_differences.begin() + j, buyer_differences.begin() + j + 4); 
            if (prices[i].size() <= j + 4)
            {
                std::cout << "Price is too short!" << std::endl;
                return -1;
            }

            if (new_sequences.sequences.count(sequence) == 0)
            {
                new_sequences.sequences.insert({sequence, prices[i][j + 4]});
            }

            all_sequences.insert(sequence);
        }

        all_buyer_sequences.push_back(new_sequences);
    }

    int max_banana_sum = 0;
    std::vector<int> best_sequence;
    for (const std::vector<int>& sequence : all_sequences)
    {
        int total_banana_sum = 0;
        for (auto& buyer : all_buyer_sequences)
        {
            if (buyer.sequences.count(sequence) == 0)
            {
                continue;
            }

            total_banana_sum += buyer.sequences[sequence];
        }

        if (max_banana_sum < total_banana_sum)
        {
            max_banana_sum = total_banana_sum;
            best_sequence = sequence;
        }
    }

    std::cout << "The most banana we can get is: " << max_banana_sum << ". The sequence is: " << std::endl;
    for (auto number : best_sequence)
    {
         std::cout << number << " ";
    }
    std::cout << std::endl;

    return 0;
}
