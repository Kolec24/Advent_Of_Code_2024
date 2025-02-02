#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>
#include<map>

struct Gate
{
    std::pair<std::string, std::string> inputs;
    std::string operand;
    std::string output;
};

std::map<std::string, int> z_to_number;
std::map<std::string, int> wires;
std::vector<Gate> and_gates;
std::vector<Gate> or_gates;
std::vector<Gate> xor_gates;
std::vector<Gate> all_gates;

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
    bool finished_wires = false;
    while (getline(infile, current_line))
    {
        if (current_line.empty())
        {
            finished_wires = true;
            continue;
        }

        std::istringstream line_stream(current_line);
        std::vector<std::string> line_string{std::istream_iterator<std::string>(line_stream), std::istream_iterator<std::string>()};
        if (finished_wires)
        {
            if (line_string.size() != 5)
            {
                std::cout << "Gate string has incorrect length!" << std::endl;
                return -1;
            }

            std::pair<std::string, std::string> inputs{line_string[0], line_string[2]};
            Gate new_gate;
            new_gate.inputs = inputs;
            new_gate.output = line_string[4];
            new_gate.operand = line_string[1];

            all_gates.push_back(new_gate);

            if (line_string[1] == "AND")
            {
                and_gates.push_back(new_gate);
                continue;
            }
            if (line_string[1] == "OR")
            {
                or_gates.push_back(new_gate);
                continue;
            }
            if (line_string[1] == "XOR")
            {
                xor_gates.push_back(new_gate);
                continue;
            }

            std::cout << line_string[0] << " -> " << line_string[2] << " has an incorrect gate!" << std::endl;
        }

        if (line_string.size() != 2)
        {
            std::cout << "Wire string has incorrect length!" << std::endl;
            return -1;
        }

        std::string current_wire = line_string[0];
        current_wire.pop_back();
        int current_value = std::stoi(line_string[1]);
        wires.insert({current_wire, current_value});
    }

    size_t number_size = wires.size() / 2;

    while (and_gates.size() + or_gates.size() + xor_gates.size() != 0)
    {
        for (size_t i = 0; i < and_gates.size(); ++i)
        {
            auto gate = and_gates[i];
            if (wires.count(gate.inputs.first) == 0)
            {
                continue;
            }
            if (wires.count(gate.inputs.second) == 0)
            {
                continue;
            }

            int first_value = wires[gate.inputs.first];
            int second_value = wires[gate.inputs.second];

            int result = 0;
            if (first_value == 1 && second_value == 1)
            {
                result = 1;
            }

            wires.insert({gate.output, result});
            and_gates.erase(and_gates.begin() + i);
            --i;
        }

        for (size_t i = 0; i < or_gates.size(); ++i)
        {
            auto gate = or_gates[i];
            if (wires.count(gate.inputs.first) == 0)
            {
               continue;
            }
            if (wires.count(gate.inputs.second) == 0)
            {
                continue;
            }

            int first_value = wires[gate.inputs.first];
            int second_value = wires[gate.inputs.second];

            int result = 0;
            if (first_value == 1 || second_value == 1)
            {
                result = 1;
            }

            wires.insert({gate.output, result});
            or_gates.erase(or_gates.begin() + i);
            --i;
        }

        for (size_t i = 0; i < xor_gates.size(); ++i)
        {
            auto gate = xor_gates[i];
            if (wires.count(gate.inputs.first) == 0)
            {
                continue;
            }
            if (wires.count(gate.inputs.second) == 0)
            {
                continue;
            }

            int first_value = wires[gate.inputs.first];
            int second_value = wires[gate.inputs.second];

            int result = 0;
            if (first_value != second_value)
            {
                result = 1;
            }

            wires.insert({gate.output, result});
            xor_gates.erase(xor_gates.begin() + i);
            --i;
        }
    }

    std::vector<int> z_reverse_binary;
    for (auto wire : wires)
    {
        if (wire.first[0] == 'z')
        {
            z_reverse_binary.push_back(wire.second);
            continue;
        }
    }

    long long decimal_number = 0;
    for (int i = 0; i < z_reverse_binary.size(); ++i)
    {
        if (z_reverse_binary[i] == 1)
        {
            decimal_number += pow(2, i);
        }
    }

    std::cout << "The end result is: " << decimal_number << std::endl;

    // Part 2.
    bool valid = true;
    int current_bit = 1;
    std::string current_and;
    std::string current_xor;
    std::string current_carry;
    std::string current_intermediate_carry;
    std::string current_value;
    std::string last_carry = "bdk";
    while (valid)
    {
        for (const auto& gate : all_gates)
        {
            if (current_xor.empty() || current_and.empty())
            {
                if (gate.inputs.first[0] == 'x' || gate.inputs.second[0] == 'x')
                {
                    std::string starting_input = gate.inputs.first;
                    starting_input.erase(starting_input.begin());
                    int actual_bit = std::stoi(starting_input);

                    if (actual_bit == current_bit)
                    {
                        if (gate.operand == "AND")
                        {
                            std::cout << "Found " << current_bit << " AND! New: " << gate.output << std::endl;
                            current_and = gate.output;
                        }

                        if (gate.operand == "XOR")
                        {
                            std::cout << "Found " << current_bit << " XOR! New: " << gate.output << std::endl;
                            current_xor = gate.output;
                        }
                    }
                }

                continue;
            }

            if (current_intermediate_carry.empty() && gate.operand == "AND" && ((gate.inputs.first == current_xor && gate.inputs.second == last_carry) ||
                (gate.inputs.second == current_xor && gate.inputs.first == last_carry)))
            {
                std::cout << "Found " << current_bit << " INTERMEDIATE! New: " << gate.output << std::endl;
                current_intermediate_carry = gate.output; 
                continue;
            }

            if (current_carry.empty() && gate.operand == "OR" && ((gate.inputs.first == current_intermediate_carry && gate.inputs.second == current_and) ||
                (gate.inputs.second == current_intermediate_carry && gate.inputs.first == current_and)))
            {
                std::cout << "Found " << current_bit << " CARRY! New: " << gate.output << std::endl;
                current_carry = gate.output;
                continue;
            }

            if (!current_carry.empty() && current_value.empty() && gate.operand == "XOR" &&
                ((gate.inputs.first == current_xor && gate.inputs.second == last_carry) ||
                (gate.inputs.second == current_xor && gate.inputs.first == last_carry)))
            {
                std::cout << "Found " << current_bit << " VALUE!" << std::endl;
                current_value = gate.output;
                if (current_value[0] != 'z')
                {
                    valid = false;
                    std::cout << "Bit nr: " << current_bit << " has a wrong value, should start with z, but says: " << current_value;
                    return 0;
                }

                current_value.erase(current_value.begin());
                int current_value_bit = std::stoi(current_value);
                if (current_value_bit != current_bit)
                {
                    valid = false;
                    std::cout << "Bit nr: " << current_bit << " starts with z, but has a wrong bit: " << current_value;
                    return 0;
                }

                // This means, that current bit is all right and we need to look at the next one.
                current_bit++;
                last_carry = current_carry;

                current_xor.clear();
                current_and.clear();
                current_carry.clear();
                current_intermediate_carry.clear();
                current_value.clear();
                break;
            }
        }
    }
    // z_n = x_n XOR y_n XOR carry_{n-1}
    // carry_n = ((x_n XOR y_n) and carry_{n-1}) OR (x_n AND y_n)
}
