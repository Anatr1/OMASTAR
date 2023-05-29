#include "project_cmakevariables.h" //Project cmake variables automatically generated by the CMake root project.
//cmake variables automatically generated by this CMake module project.
//Notice: the first part of the name is the same as declared in PROJECT_NAME in the CMake project.
#include "OMASTAR_cmakevariables.h"
#include <iostream>
#include <toml.hpp>
#include <MqttManager.hpp>
#include <Property.hpp>
#include <Moquette2/MoquetteClient.hpp>
#include "./include/utils/pgm.h"
#include "./include/utils/utils.h"
#include "./include/A_star/A_star.h"

using namespace std;

int main(void) {
    srand(time(0));
    //prints some project variables, defined in <buildfolder>/project_cmakevariables.h and <buildfolder>/<moduleName>_cmakevariables.h
    cout << DIANA_OMASTAR_PROJECT_NAME << " from project "<<DIANA_PROJECT_NAME<<" version " << DIANA_PROJECT_VERSION << endl
            << "Example file path is "<< DIANA_OMASTAR_FILE_EXAMPLEFILE_TXT << endl
            << "Config file path is " << DIANA_OMASTAR_CFG_FILE_CONFIG_TOML << endl
            << "Installation folder is " << DIANA_OMASTAR_MODULE_ROOTPATH << endl<< endl;

    for (int i = 0; i < 3; i++) {
        srand(time(0) + i);
        cout << "Iteration " << i << endl;
        vector<vector<int>> map = generatePerlinNoiseMap(400, 500, 255, (double)i+1);
        exportToPGM(map, to_string(i) + "_0_map_raw.pgm");


        map = filterMap(map, 100);
        exportToPGM(map, to_string(i) + "_1_map_filtered.pgm");

        cout << "MAP SIZE: " << map[0].size() << "x" << map.size() << endl;

        xy start = generateRandomPoint(map[0].size(), map.size(), map);
        vector<xy> goals = generateRandomPoints(map[0].size(), map.size(), map, 4);

        cout << "Start: " << start.x << " " << start.y << endl;
        cout << "Goals: " << endl;
        for (int i = 0; i < goals.size(); i++) {
            cout << goals[i].x << " " << goals[i].y << endl;
        }
        vector<xy> path = reachMultipleWaypoints(map, start, goals);

        map = colorPoint(map, start, true);
        for (int i = 0; i < goals.size(); i++) {
            map = colorPoint(map, goals[i]);
        }
        exportToPGM(map, to_string(i) + "_2_map_goals.pgm");

        map = colorPath(map, path);

        exportToPGM(map, to_string(i) + "_3_map_path.pgm");

        sleep(1);
    }

    return 0;
}
