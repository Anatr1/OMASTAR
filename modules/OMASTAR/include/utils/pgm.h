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
#include <noise/noise.h>
#include <noise/module/perlin.h>
#include <cmath>
#include <vector>

/**
 * @brief Read a PGM map file and return a vector of vectors of integers
 * Example of a PGM map file:
 * P2
 * 3 3
 * 255
 * 0 0 0
 * 0 255 0
 * 0 0 0
 * @param filename
 * @return vector<vector<int>>
 */
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
        ss >> cols >> rows;
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


/**
 * @brief Print a map to the console -> unpractical for large maps
 * @param map
 */
void printMap(vector<vector<int>> map) {
    cout << "\nPrinting map" << endl;
    for (auto row : map) {
        for (auto cell : row) {
            cout << cell << " ";
        }
        cout << endl;
    }
    cout << endl;
}

/**
 * @brief Highlight a path in a map by coloring the cells in the path in white
 * @param map vector of vectors of integers
 * @param path vector of xy structs with the coordinates of the path
 * @return vector<vector<int>> map
 */
vector<vector<int>> colorPath(vector<vector<int>> map, vector<xy> path) {
    for (auto point : path) {
        map[point.x][point.y] = 255;
    }
    return map;
}

/**
 * @brief Export a map to a PGM file
 * @param map vector of vectors of integers
 * @param filename name of the file to be created
 */
void exportToPGM(vector<vector<int>> map, std::string filename) {
    FILE *imageFile;

    imageFile=fopen(filename.c_str(),"wb");
    if(imageFile==NULL){
        perror("ERROR: Cannot open output file");
        exit(EXIT_FAILURE);
    }
    cout << "Map size: " << map[0].size() << "x" << map.size() << endl;

    fprintf(imageFile,"P2\n"); //Magic Number
    fprintf(imageFile, "%d %d\n", map[0].size(), map.size()); //Dimensions
    fprintf(imageFile, "%d\n", 255); //Dimensions
    for(int i = 0; i < map.size(); i++){
        for(int j = 0; j < map[0].size(); j++){
            fprintf(imageFile, "%d ", map[i][j]);
        }
        fprintf(imageFile, "\n");
    }

    cout << "Map exported to " << filename << "\n\n" << endl;
    fclose(imageFile);
}

/**
 * @brief Generate a map with Perlin noise
 * @param rows number of rows of the map
 * @param cols number of columns of the map
 * @param maxVal maximum value of the map
 * @param frequency frequency of the Perlin noise
 * @return vector<vector<int>> map
 */
vector<vector<int>> generatePerlinNoiseMap(int rows, int cols, int maxVal, double frequency) {
    vector<vector<int>> map;
    noise::module::Perlin perlinModule;
    srand(time(0));
    perlinModule.SetSeed(time(0));
    perlinModule.SetFrequency(std::fmod(frequency,16.0));
    map.resize(rows, vector<int>(cols));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; ++j) {
            double x = (double)j/((double)cols);
            double y = (double)i/((double)rows);
            double val = perlinModule.GetValue(x, y, 0.0);
            map[i][j] = (int)((val + 1.0) * 0.5 * (double)maxVal);
        }
    }
    return map;
}

/**
 * @brief Filter a map by a threshold to obtain a binary map
 * @param map  vector of vectors of integers
 * @param threshold threshold value
 * @return vector<vector<int>> filtered map
 */
vector<vector<int>> filterMap(vector<vector<int>> map, int threshold) {
    vector<vector<int>> filteredMap;
    filteredMap.resize(map.size(), vector<int>(map[0].size()));
    for (int i = 0; i < map.size(); ++i) {
        for (int j = 0; j < map[0].size(); ++j) {
            if (map[i][j] > threshold) {
                filteredMap[i][j] = 128;
            } else {
                filteredMap[i][j] = 0;
            }
        }
    }
    return filteredMap;
}


/**
 * @brief Translate a path so that it always starts at (0,0) by subtracting to each point the coordinates of the first point
 * @param path vector of xy structs with the coordinates of the path
 * @return vector<vector<xy>> translated path
 */
vector<xy> translatePath(vector<xy> path) {
    vector<xy> translatedPath;
    xy firstPoint = path[0];
    for (auto point : path) {
        translatedPath.push_back({point.x - firstPoint.x, point.y - firstPoint.y});
    }
    return translatedPath;
}
#endif //__OMASTAR_PGM_H__
