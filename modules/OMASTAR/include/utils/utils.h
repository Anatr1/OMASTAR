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
    while(map[point.x][point.y] == 0){
        point.x = rand() % max_x;
        point.y = rand() % max_y;
    }
    return point;
}

#endif //__OMASTAR_UTILS_H__
