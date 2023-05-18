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

xy generateRandomPoint(int max_x, int max_y){
    xy point;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 100);
    point.x = dis(gen) % max_x;
    point.y = dis(gen) % max_y;
    return point;
}

#endif //__OMASTAR_UTILS_H__
