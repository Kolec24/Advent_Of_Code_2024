#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>

long long counter = 0;
long long A = 0;
long long B = 0;
long long C = 0;
long long instruction_pointer = 0;
std::vector<long long> program;

bool should_reset = false;
std::vector<long long> output;
std::string output_string;

void reset()
{
    A = 0;
    B = 0;
    C = 0;
    instruction_pointer = 0;
    output.clear();
    output_string.clear();
    should_reset = false;
}

long long getComboOperand(long long literal_operand)
{
    switch (literal_operand)
    {
    case 0:
    case 1:
    case 2:
    case 3:
        return literal_operand;
    case 4:
        return A;
    case 5:
        return B;
    case 6:
        return C;
    case 7:
        std::cout << "Combo operand 7 - invalid program!" << std::endl;
        break;
    default:
        std::cout << "Wrong operand!" << std::endl;
        break;
    }

    return -1;
}

void adv(long long operand)
{
    long long numerator = A;
    long long combo_operand = getComboOperand(operand);
    long long denominator = pow(2, combo_operand);
    long long result = numerator / denominator;
    A = result;
}

void bxl(long long operand)
{
    long long bitwise_xor = B^operand;
    B = bitwise_xor;
}

void bst(long long operand)
{
    long long combo_operand = getComboOperand(operand);
    long long modulo = combo_operand % 8;
    B = modulo;
}

void jnz(long long operand)
{
    if (A == 0)
    {
        instruction_pointer += 2;
        return;
    }

    instruction_pointer = operand;
}

void bxc(long long operand)
{
    long long bitwise_xor = B^C;
    B = bitwise_xor;
}

void out(long long operand)
{
    long long combo_operand = getComboOperand(operand);
    long long modulo = combo_operand % 8;
    output.push_back(modulo);

    std::string next_string = std::to_string(modulo) + ",";
    output_string += next_string;
}

void bdv(long long operand)
{
    long long numerator = A;
    long long combo_operand = getComboOperand(operand);
    long long denominator = pow(2, combo_operand);
    long long result = numerator / denominator;
    B = result;
}

void cdv(long long operand)
{
    long long numerator = A;
    long long combo_operand = getComboOperand(operand);
    long long denominator = pow(2, combo_operand);
    long long result = numerator / denominator;
    C = result;
}

void callFunction(long long opcode, long long operand)
{
    switch (opcode)
    {
    case 0:
        adv(operand);
        instruction_pointer += 2;
        break;
    case 1:
        bxl(operand);
        instruction_pointer += 2;
        break;
    case 2:
        bst(operand);
        instruction_pointer += 2;
        break;
    case 3:
        jnz(operand);
        break;
    case 4:
        bxc(operand);
        instruction_pointer += 2;
        break;
    case 5:
        out(operand);
        instruction_pointer += 2;
        break;
    case 6:
        bdv(operand);
        instruction_pointer += 2;
        break;
    case 7:
        cdv(operand);
        instruction_pointer += 2;
        break;
    default:
        std::cout << "Wrong opcode!" << std::endl;
        break;
    }
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
    std::string current_string;
    bool registers = true;

    while (getline(infile, current_line))
    {
        if (current_line.empty())
        {
            registers = false;
            continue;
        }

        std::istringstream line_stream(current_line);
        std::vector<std::string> line_vector{std::istream_iterator<std::string>(line_stream), std::istream_iterator<std::string>()};

        if (registers)
        {
            if (line_vector[1][0] == 'A')
            {
                A = std::stoll(line_vector[2]);
                continue;
            }
            if (line_vector[1][0] == 'B')
            {
                B = std::stoll(line_vector[2]);
                continue;
            }
            if (line_vector[1][0] == 'C')
            {
                C = std::stoll(line_vector[2]);
                continue;
            }
            
            continue;
        }

        std::istringstream program_stream(line_vector[1]);
        for (long long i; program_stream >> i;)
        {
            program.push_back(i);
            if (program_stream.peek() == ',')
            {
                program_stream.ignore();
            }
        }
    }

    while (instruction_pointer < program.size())
    {
        callFunction(program[instruction_pointer], program[instruction_pointer + 1]);
    }

    std::cout << "The program output is: " << output_string << std::endl;

    // Part 2.
    reset();
    std::vector<long long> final_a;
    std::vector<long long> a_to_check = {0, 1, 2, 3, 4, 5, 6, 7};
    long long program_size = program.size();
    long long iteration_counter = 0;
    while(iteration_counter < program_size)
    {
        std::vector<long long> subprogram(program.begin() + program_size - 1 - iteration_counter, program.end());
        std::vector<long long> good_a;
        for (const auto& a : a_to_check)
        {
            A = a;
            while (instruction_pointer < program.size())
            {
                callFunction(program[instruction_pointer], program[instruction_pointer + 1]);
            }

            if (output == subprogram)
            {
                good_a.push_back(a);
            }

            reset();
        }

        a_to_check.clear();
        if (subprogram == program)
        {
            for (const auto& good : good_a)
            {
                final_a.push_back(good);
            }
        }

        for (const auto& good : good_a)
        {
            long long multiplied_a = good * 8;
            for (long long i = 0; i < 8; ++i)
            {
                a_to_check.push_back(multiplied_a + i);
            }
        }

        iteration_counter++;
    }

    long long min_result = LLONG_MAX;
    for (const auto& a : final_a)
    {
        if (a < min_result)
        {
            min_result = a;
        }
    }

    std::cout << "The smallest value that makes our program output itself is: " << min_result << std::endl;
}
