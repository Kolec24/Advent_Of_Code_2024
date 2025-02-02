#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>

struct Node
{
    std::pair<int, int> position;
    std::pair<int, int> direction;
};

constexpr int board_size = 141;
char board[board_size][board_size];
int node_counter = 0;
int starting_index = 0;
std::vector<int> end_indices;
std::pair<int, int> starting_direction = {0, 1};
std::vector<std::pair<int, int>> directions = {{0, -1}, {-1, 0}, {0, 1}, {1, 0}};
std::vector<Node> all_nodes;

bool contains_pair(const std::vector<std::pair<int, int>>& vector, const std::pair<int, int>& pair)
{
    for (auto current_pair : vector)
    {
        if (current_pair == pair)
        {
            return true;
        }
    }

    return false;
}

int getNodeIndex(const Node& node)
{
    for (int i = 0; i < all_nodes.size(); ++i)
    {
        if (all_nodes[i].position == node.position && all_nodes[i].direction == node.direction)
        {
            return i;
        }

    }

    return -1;
}

int minDistance(const std::vector<int>& dist, const std::vector<bool>& visited)
{
    int index;
    int min = INT_MAX;

    for (int i = 0; i < all_nodes.size(); ++i)
    {
        if (visited[i] == false && dist[i] <= min)
        {
            min = dist[i];
            index = i;
        }
    }

    return index;
}

int dijkstra()
{
    std::vector<int> dist;
    std::vector<bool> visited;
    std::vector<std::vector<std::vector<Node>>> best_paths;

    for (int i = 0; i < all_nodes.size(); ++i)
    {
        dist.push_back(INT_MAX);
        visited.push_back(false);
        best_paths.push_back(std::vector<std::vector<Node>>());
    }

    Node starting_node = all_nodes[starting_index];
    std::vector<Node> starting_path;
    starting_path.push_back(starting_node);
    best_paths[starting_index].push_back(starting_path);

    dist[starting_index] = 0;

    for (int i = 0; i < all_nodes.size(); ++i)
    {
        int m = minDistance(dist, visited);
        Node current_node = all_nodes[m];

        visited[m] = true;

        std::pair<int, int> inverted_direction = {current_node.direction.first * -1, current_node.direction.second * -1};
        for (const auto& direction : directions)
        {
            if (direction == inverted_direction)
            {
                continue;
            }

            Node next_node;
            next_node.direction = direction;
            next_node.position = current_node.position;
            if (current_node.direction == direction)
            {
                std::pair<int, int> next_position = {current_node.position.first + direction.first, current_node.position.second + direction.second};
                if (board[next_position.first][next_position.second] == '#')
                {
                    continue;
                }

                next_node.position = next_position;
            }

            int next_index = getNodeIndex(next_node);
            if (next_index < 0)
            {
                std::cout << "Direction index is negative!" << std::endl;
                return -1;
            }

            int traversal_cost = direction == current_node.direction ? 1 : 1000;

            if (visited[next_index] == false && dist[m] != INT_MAX && dist[m] + traversal_cost == dist[next_index])
            {
                for (auto best_path : best_paths[m])
                {
                    best_path.push_back(next_node);
                    best_paths[next_index].push_back(best_path);
                }
            }

            if (visited[next_index] == false && dist[m] != INT_MAX && dist[m] + traversal_cost < dist[next_index])
            {
                dist[next_index] = dist[m] + traversal_cost;
                best_paths[next_index].clear();
                for (auto best_path : best_paths[m])
                {
                    best_path.push_back(next_node);
                    best_paths[next_index].push_back(best_path);
                }
            }
        }

        std::cout << "Finished node: " << i << std::endl;
    }

    int min_end_cost = INT_MAX;
    for (auto end_index : end_indices)
    {
        if (dist[end_index] < min_end_cost)
        {
            min_end_cost = dist[end_index];
        }
    }


    std::vector<std::pair<int, int>> best_positions;
    for (auto end_index : end_indices)
    {
        if (dist[end_index] != min_end_cost)
        {
            continue;
        }

        for (auto best_path : best_paths[end_index])
        {
            for (auto current_node : best_path)
            {
                if (!contains_pair(best_positions, current_node.position))
                {
                    best_positions.push_back(current_node.position);
                }
            }
        }
    }

    std::cout << "The lowest score is: " << min_end_cost << std::endl;
    std::cout << "The number of positions on at least one best path is: " << best_positions.size() << std::endl;

    return min_end_cost;
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
    int row_count = 0;
    while (getline(infile, current_line))
    {
        std::istringstream line_stream(current_line);
        std::vector<char> board_vector{std::istream_iterator<char>(line_stream), std::istream_iterator<char>()};

        if (board_size != board_vector.size())
        {
            std::cout << "Board has a different size than a line!" << std::endl;
            continue;
        }

        for (int i = 0; i < board_size; ++i)
        {
            board[row_count][i] = board_vector[i];
            if (board_vector[i] == '#')
            {
                continue;
            }

            for (auto direction : directions)
            {
                if (board_vector[i] == 'S' && direction == starting_direction)
                {
                    starting_index = all_nodes.size();
                }
                if (board_vector[i] == 'E')
                {
                    end_indices.push_back(all_nodes.size());
                }

                Node node;
                node.position = {row_count, i};
                node.direction = direction;
                all_nodes.push_back(node);
            }
        }

        row_count++;
    }

    dijkstra();
}
