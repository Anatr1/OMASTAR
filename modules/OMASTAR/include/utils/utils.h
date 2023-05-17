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

xy generateRandomPoint(){
    xy point;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 100);
    point.x = dis(gen);
    point.y = dis(gen);
    return point;
}

#endif //__OMASTAR_UTILS_H__
