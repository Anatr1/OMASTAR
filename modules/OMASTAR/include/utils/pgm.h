//
// Created by h4r on 18/05/23.
//

#ifndef __OMASTAR_PGM_H__
#define __OMASTAR_PGM_H__

#include <iostream>
#include <fstream>
#include "OMASTAR_cmakevariables.h"
#include <iostream>
#include <toml.hpp>
#include <Shmem.hpp>
#include "./utils.h"

//**
// * @brief Read a PGM map file and return a vector of vectors of integers
// * Example of a PGM map file:
// * P2
// * 3 3
// * 255
// * 0 0 0
// * 0 255 0
// * 0 0 0
// * @param filename
// * @return vector<vector<int>>
vector<vector<int>> readPGMmap(string filename) {
    ifstream file(filename);
    string line;
    vector<vector<int>> map;
    int rows, cols, maxVal;

    if (file.is_open()) {
        getline(file, line);
        if (line != "P2") {
            cout << "Error: file is not a PGM map" << endl;
            return map;
        }
        getline(file, line);
        //cout << line << endl;
        stringstream ss(line);
        ss >> rows >> cols;
        getline(file, line);
        stringstream ss2(line);
        ss2 >> maxVal;
        map.resize(rows, vector<int>(cols));
        for (int i = 0; i < rows; i++) {
            getline(file, line);
            stringstream ss3(line);
            for (int j = 0; j < cols; j++) {
                ss3 >> map[i][j];
            }
        }
    } else {
        cout << "Error: file not found" << endl;
        cout << "File path is " << filename << endl;
    }
    return map;
}

void printMap(vector<vector<int>> map) {
    cout << "Printing map" << endl;
    for (auto row : map) {
        for (auto cell : row) {
            cout << cell << " ";
        }
        cout << endl;
    }
}

vector<vector<int>> colorPath(vector<vector<int>> map, vector<xy> path) {
    for (auto point : path) {
        map[point.x][point.y] = 255;
    }
    return map;
}


#endif //__OMASTAR_PGM_H__
