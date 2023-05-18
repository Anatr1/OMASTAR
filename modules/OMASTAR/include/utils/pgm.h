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

vector<vector<int>> colorPath(vector<vector<int>> map, vector<xy> path) {
    for (auto point : path) {
        map[point.x][point.y] = 255;
    }
    return map;
}

void exportToPGM(vector<vector<int>> map){
    FILE *imageFile;

    imageFile=fopen("map.pgm","wb");
    if(imageFile==NULL){
        perror("ERROR: Cannot open output file");
        exit(EXIT_FAILURE);
    }
    cout << "Exporting map to map.pgm" << endl;
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

    cout << "Map exported to map.pgm" << endl;
    fclose(imageFile);
}

#endif //__OMASTAR_PGM_H__
