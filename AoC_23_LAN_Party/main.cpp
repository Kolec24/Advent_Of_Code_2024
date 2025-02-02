#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<map>
#include<set>

std::vector<std::pair<std::string, std::string>> all_pairs;
std::map<std::string, std::vector<std::string>> connections;
std::map<std::string, std::vector<std::pair<std::string, std::string>>> all_groups;
std::set<std::string> all_computers;


std::vector<std::vector<std::string>> part_2_groups;

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
        std::string first;
        std::string second;
        bool is_first = true;
        for (const char& c : current_line)
        {
            if (c == '-')
            {
                is_first = false;
                continue;
            }

            if (is_first)
            {
                first.push_back(c);
            }
            else
            {
                second.push_back(c);
            }
        }

        all_computers.insert(first);
        all_computers.insert(second);
        std::pair<std::string, std::string> current_pair = {first, second};
        all_pairs.push_back(current_pair);
    }

    for (const auto& pair : all_pairs)
    {
        if (connections.count(pair.first) == 0)
        {
            std::vector<std::string> empty_vector;
            connections.insert({pair.first, empty_vector});
        }

        connections[pair.first].push_back(pair.second);

        if (connections.count(pair.second) == 0)
        {
            std::vector<std::string> empty_vector;
            connections.insert({pair.second, empty_vector});
        }

        connections[pair.second].push_back(pair.first);
    }

    for (const auto& connection : connections)
    {
        std::vector<std::pair<std::string, std::string>> groups;
        const auto& connectors = connection.second;
        for (int i = 0; i < connectors.size(); ++i)
        {
            for (int j = i + 1; j < connectors.size(); ++j)
            {
                auto first_connector = connectors[i];
                auto second_connector = connectors[j];
                if (std::find(
                    connections[first_connector].begin(), connections[first_connector].end(), second_connector) != connections[first_connector].end())
                {
                    groups.push_back({first_connector, second_connector});
                }
            }
        }
        
        all_groups.insert({connection.first, groups});
    }

    int appropriate_counter = 0;
    std::map<std::string, std::vector<std::pair<std::string, std::string>>> already_counted_multiple_t_groups;
    for (const auto& element_group : all_groups)
    {
        if (element_group.first[0] != 't')
        {
            continue;
        }

        bool should_check_already_counted = already_counted_multiple_t_groups.count(element_group.first) != 0;
        for (const auto& group : element_group.second)
        {
            bool skip = false;
            if (should_check_already_counted)
            {
                for (const auto& counted_group : already_counted_multiple_t_groups[element_group.first])
                {
                    bool first_contained = counted_group.first == group.first || counted_group.second == group.first;
                    bool second_contained = counted_group.first == group.second || counted_group.second == group.second;
                    if (first_contained && second_contained)
                    {
                        skip = true;
                        break;
                    }
                }
            }

            if (skip)
            {
                continue;
            }

            appropriate_counter++;
            if (group.first[0] == 't')
            {
                if (already_counted_multiple_t_groups.count(group.first) == 0)
                {
                    already_counted_multiple_t_groups.insert({group.first, {}});
                }

                already_counted_multiple_t_groups[group.first].push_back({element_group.first, group.second});
            }
            if (group.second[0] == 't')
            {
                if (already_counted_multiple_t_groups.count(group.second) == 0)
                {
                    already_counted_multiple_t_groups.insert({group.second, {}});
                }

                already_counted_multiple_t_groups[group.second].push_back({element_group.first, group.first});
            }
        }
    }

    std::cout << "The number of the appropriate groups is: " << appropriate_counter << std::endl;

    // Part 2.
    for (const auto& computer : all_computers)
    {
        std::vector<std::string> connection = connections[computer];
        for (auto& group : part_2_groups)
        {
            bool viable_group = true;
            for (const auto& element : group)
            {
                if (std::find(connection.begin(), connection.end(), element) == connection.end())
                {
                    viable_group = false;
                    break;
                }
            }

            if (viable_group)
            {
                group.push_back(computer);
            }
        }

        part_2_groups.push_back({computer});
    }

    size_t max_length = 0;
    std::vector<std::string> best_group;
    for (const auto& group : part_2_groups)
    {
        if (max_length < group.size())
        {
            max_length = group.size();
            best_group = group;
        }
    }

    for (auto element : best_group)
    {
         std::cout << element << ",";
    }
    std::cout << std::endl;

    std::cout << "The largest group has " << max_length << " elements." << std::endl;

    return 0;
}
