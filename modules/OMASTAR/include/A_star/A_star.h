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
    return point.x >= 0 && point.x < rows && point.y >= 0 && point.y < cols;
}

// Helper function to get the neighboring points of a given point
vector<xy> getNeighbors(xy point) {
    vector<xy> neighbors;
    neighbors.push_back({point.x - 1, point.y}); // Up
    neighbors.push_back({point.x + 1, point.y}); // Down
    neighbors.push_back({point.x, point.y - 1}); // Left
    neighbors.push_back({point.x, point.y + 1}); // Right
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
 * @param map vector of vectors of integers representing the map
 * @param start xy structure representing the start point
 * @param goal xy structure representing the goal point
 * @return
 */
vector<xy> a_star(vector<vector<int>>& map, xy start, xy goal) {
    cout << "---- A* algorithm ----" << endl;
    cout << "   Map size: " << map.size() << " " << map[0].size() << endl;
    cout << "   Start: " << start.x << " " << start.y << endl;
    cout << "   Goal: " << goal.x << " " << goal.y << endl;
    vector<xy> path;
    int rows = map.size();
    int cols = map[0].size();

    vector<vector<bool>> visited(rows, vector<bool>(cols, false));
    vector<vector<int>> gScore(rows, vector<int>(cols, INT_MAX));

    auto compareFScore = [](Node* a, Node* b) { return a->fScore > b->fScore; };
    priority_queue<Node*, vector<Node*>, decltype(compareFScore)> openSet(compareFScore);

    gScore[start.x][start.y] = 0;
    int hScore = heuristicDistance(start, goal);
    openSet.push(new Node(start, 0, hScore, nullptr));

    while (!openSet.empty()) {
        Node* current = openSet.top();
        openSet.pop();

        xy currentPoint = current->point;
        visited[currentPoint.x][currentPoint.y] = true;

        if (currentPoint.x == goal.x && currentPoint.y == goal.y) {
            // Goal reached, reconstruct the path and return
            vector<xy> path = reconstructPath(current);
            return path;
        }

        vector<xy> neighbors = getNeighbors(currentPoint);
        for (const xy& neighbor : neighbors) {
            if (isValidPoint(neighbor, map) && !visited[neighbor.x][neighbor.y] && map[neighbor.x][neighbor.y] != 0) {
                int newGScore = current->gScore + 1; // Assuming all movements have a cost of 1

                if (newGScore < gScore[neighbor.x][neighbor.y]) {
                    // Update the gScore for the neighbor and add it to the open set
                    gScore[neighbor.x][neighbor.y] = newGScore;
                    int hScore = heuristicDistance(neighbor, goal);
                    int fScore = newGScore + hScore;
                    openSet.push(new Node(neighbor, newGScore, fScore, current));
                }
            }
        }
    }

    // If the open set becomes empty and the goal is not reached, return an empty path
    cout << "   GOAL NOT REACHABLE" << endl;
    return {};
}

#endif //__OMASTAR_A_STAR_H__
