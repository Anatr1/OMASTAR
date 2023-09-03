//
// Created by h4r on 18/05/23.
//

#ifndef __OMASTAR_A_STAR_H__
#define __OMASTAR_A_STAR_H__

#include "../utils/utils.h"
#include <vector>
#include <queue>
#include <cmath>
#include <limits.h>

using namespace std;

struct Node {
    xy point;
    int gScore; // cost to reach this node from the start
    int fScore; // gScore + heuristic estimate to the goal
    Node* parent;

    Node(xy p, int g, int f, Node* par) : point(p), gScore(g), fScore(f), parent(par) {}
};

// Helper function to calculate the heuristic distance between two points using Euclidean distance
int heuristicDistance(xy p1, xy p2) {
    int dx = abs(p1.x - p2.x);
    int dy = abs(p1.y - p2.y);
    return static_cast<int>(sqrt(dx * dx + dy * dy));
}

// Helper function to check if a point is within the map boundaries
bool isValidPoint(xy point, vector<vector<int>>& map) {
    int rows = map.size();
    int cols = map[0].size();
    return point.x >= 0 && point.x < cols && point.y >= 0 && point.y < rows;
}

// Helper function to get the neighboring points of a given point, including diagonal neighbors
vector<xy> getNeighbors(xy point) {
    vector<xy> neighbors;
    neighbors.push_back({point.x - 1, point.y}); // Up
    neighbors.push_back({point.x + 1, point.y}); // Down
    neighbors.push_back({point.x, point.y - 1}); // Left
    neighbors.push_back({point.x, point.y + 1}); // Right
    neighbors.push_back({point.x - 1, point.y - 1}); // Up Left
    neighbors.push_back({point.x - 1, point.y + 1}); // Up Right
    neighbors.push_back({point.x + 1, point.y - 1}); // Down Left
    neighbors.push_back({point.x + 1, point.y + 1}); // Down Right
    return neighbors;
}

// Helper function to reconstruct the path from the goal node to the start node
vector<xy> reconstructPath(Node* current) {
    vector<xy> path;
    while (current != nullptr) {
        path.push_back(current->point);
        current = current->parent;
    }
    reverse(path.begin(), path.end());
    return path;
}

/**
 * @brief A* algorithm
 * Find the shortest path between two points in a map
 * The map is a vector of vectors of integers obtained by reading a PGM map file. Values equal to 0 are occupied cells, values different from 0 are free cells.
 * The start and goal points are xy structures with x and y integer values.
 * Assume the cost of moving from a cell to another is 1 if the cells are adjacent, sqrt(2) if the cells are diagonal.
 * @param map vector of vectors of integers representing the map
 * @param start xy structure representing the start point
 * @param goal xy structure representing the goal point
 * @return
 */
vector<xy> a_star(vector<vector<int>>& map, xy start, xy goal) {
    cout << "---- A* algorithm ----" << endl;
    cout << "   Start: " << start.x << " " << start.y << endl;
    cout << "   Goal: " << goal.x << " " << goal.y << endl;
    vector<xy> path = {};
    int rows = map.size();
    int cols = map[0].size();

    // Check if the start and goal points are valid
    if (!isValidPoint(start, map) || !isValidPoint(goal, map)) {
        cout << "   Invalid start or goal point" << endl;
        return path;
    }

    // Check if the start and goal points are free
    if (map[start.y][start.x] == 0 || map[goal.y][goal.x] == 0) {
        cout << "   Start or goal point is not free" << endl;
        return path;
    }

    // Check if the start and goal points are the same
    if (start.x == goal.x && start.y == goal.y) {
        cout << "   Start and goal points are the same" << endl;
        return path;
    }

    // Initialize the open and closed lists
    vector<vector<bool>> closedList(rows, vector<bool>(cols, false));
    vector<vector<bool>> openList(rows, vector<bool>(cols, false));
    vector<vector<Node*>> nodes(rows, vector<Node*>(cols, nullptr));
    priority_queue<pair<int, Node*>, vector<pair<int, Node*>>, greater<pair<int, Node*>>> openQueue;

    // Initialize the start node
    Node* startNode = new Node(start, 0, heuristicDistance(start, goal), nullptr);
    nodes[start.y][start.x] = startNode;
    openQueue.push({startNode->fScore, startNode});
    openList[start.y][start.x] = true;

    // A* algorithm
    while (!openQueue.empty()) {
        // Get the node with the lowest fScore from the open list
        Node* current = openQueue.top().second;
        openQueue.pop();
        openList[current->point.y][current->point.x] = false;
        closedList[current->point.y][current->point.x] = true;

        // Check if the goal has been reached
        if (current->point.x == goal.x && current->point.y == goal.y) {
            cout << "   Goal reached" << endl;
            path = reconstructPath(current);
            return path;
        }

        // Get the neighbors of the current node
        vector<xy> neighbors = getNeighbors(current->point);
        for (const xy& neighbor : neighbors) {
            // Check if the neighbor is valid
            if (!isValidPoint(neighbor, map)) {
                continue;
            }

            // Check if the neighbor is free
            if (map[neighbor.y][neighbor.x] == 0) {
                continue;
            }

            // Check if the neighbor is in the closed list
            if (closedList[neighbor.y][neighbor.x]) {
                continue;
            }

            // Calculate the gScore of the neighbor
            int gScore = current->gScore + (current->point.x != neighbor.x && current->point.y != neighbor.y ? 14 : 10);

            // Check if the neighbor is in the open list
            if (!openList[neighbor.y][neighbor.x]) {
                // Add the neighbor to the open list
                Node* neighborNode = new Node(neighbor, gScore, gScore + heuristicDistance(neighbor, goal), current);
                nodes[neighbor.y][neighbor.x] = neighborNode;
                openQueue.push({neighborNode->fScore, neighborNode});
                openList[neighbor.y][neighbor.x] = true;
            } else {
                // Check if the neighbor is reached with a lower gScore
                if (gScore < nodes[neighbor.y][neighbor.x]->gScore) {
                    // Update the neighbor node
                    nodes[neighbor.y][neighbor.x]->gScore = gScore;
                    nodes[neighbor.y][neighbor.x]->fScore = gScore + heuristicDistance(neighbor, goal);
                    nodes[neighbor.y][neighbor.x]->parent = current;
                }
            }
        }
    }

    cout << "   GOAL NOT REACHABLE" << endl;
    return path;
}

/**
 * @brief Reach multiple waypoints and return to the start point
 * @param map vector of vectors of integers representing the map
 * @param start xy structure representing the start point
 * @param waypoints vector of xy structures representing the waypoints
 * @return
 */
vector<xy> reachMultipleWaypoints(vector<vector<int>>& map, xy start, vector<xy> waypoints) {
    cout << "---- Reach multiple waypoints ----" << endl;
    cout << "   Start: " << start.x << " " << start.y << endl;
    cout << "   Waypoints: ";
    for (const xy& waypoint : waypoints) {
        cout << waypoint.x << " " << waypoint.y << ", ";
    }
    cout << endl;

    vector<xy> path;
    xy current_point = start;
    for (const xy& waypoint : waypoints) {
        vector<xy> partial_path = a_star(map, current_point, waypoint);
        if (!partial_path.empty()) {
            path.insert(path.end(), partial_path.begin(), partial_path.end());
            current_point = waypoint;
        }
    }
    vector<xy> return_path = a_star(map, current_point, start);
    path.insert(path.end(), return_path.begin(), return_path.end());
    current_point = start;

    return path;
}

#endif //__OMASTAR_A_STAR_H__
