//
// Created by h4r on 18/05/23.
//

#ifndef __OMASTAR_UTILS_H__
#define __OMASTAR_UTILS_H__
#define _USE_MATH_DEFINES
#include <cmath>
#include <random>

struct xy {
    int x,y;
};

/**
 * @brief Choose a random point in the map that is not an obstacle (map[y][x] != 0)
 * @param max_x Maximum x coordinate
 * @param max_y Maximum y coordinate
 * @param map Map of the environment
 * @return xy struct with the coordinates of the point
 */
xy generateRandomPoint(int max_x, int max_y, vector<vector<int>> map){
    xy point;
    point.x = rand() % max_x;
    point.y = rand() % max_y;
    while(map[point.y][point.x] == 0){
        point.x = rand() % max_x;
        point.y = rand() % max_y;
    }
    return point;
}

/**
 * @brief Generate n random points in the map that are not obstacles
 * @param max_x Maximum x coordinate
 * @param max_y Maximum y coordinate
 * @param map Map of the environment
 * @param n Number of points to generate
 * @return vector of xy structs with the coordinates of the points
 */
vector<xy> generateRandomPoints(int max_x, int max_y, vector<vector<int>> map, int n){
    vector<xy> points;
    for(int i = 0; i < n; i++){
        points.push_back(generateRandomPoint(max_x, max_y, map));
    }
    return points;
}

#endif //__OMASTAR_UTILS_H__
