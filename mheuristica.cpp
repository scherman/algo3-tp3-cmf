//
// Created by jscherman on 20/06/17.
//

#include "grasp.h"
#include <fstream>
#include "stringTokenizer.hpp"
#include "grasp.h"

char* getCmdOption(char ** begin, char ** end, const string & option)
{
    char ** itr = find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return 0;
}

bool cmdOptionExists(char** begin, char** end, const string& option)
{
    return find(begin, end, option) != end;
}

int main(int argc, char** argv) {
    int RCL = 3;
    int iterations = 20;

    if(cmdOptionExists(argv, argv+argc, "--rcl")) {
        char* rclOption = getCmdOption(argv, argv + argc, "--rcl");
        RCL = stoi(rclOption);
    }

    if(cmdOptionExists(argv, argv+argc, "--iterations")) {
        char* iterationsOption = getCmdOption(argv, argv + argc, "--iterations");
        iterations = stoi(iterationsOption);
    }

    unsigned n, m;
    stringTokenizer strTok;
    string linea;

    while(getline(std::cin, linea)){
        if(linea == "-1 -1") continue;
        strTok.tokenize(linea, ' ');
        n = stoi(strTok[0]);
        m = stoi(strTok[1]);
        std::vector<std::list<int>> listaAdyacencias(n);
        std::vector<std::vector<bool>> matrizAdyacencias(n, std::vector<bool>(n));
        for (int j = 0; j < n; ++j) {
            for (int i = 0; i < n; ++i) {
                matrizAdyacencias[j][i] = false;
            }
        }

        int c = 0;
        for(int i = 0; i < m; i++){
            int v1, v2;
            getline(std::cin, linea);
            strTok.tokenize(linea, ' ');
            v1 = stoi(strTok[0]);
            v2 = stoi(strTok[1]);
            listaAdyacencias[v1].push_back(v2);
            listaAdyacencias[v2].push_back(v1);
            matrizAdyacencias[v1][v2] = true;
            matrizAdyacencias[v2][v1] = true;
        }

        Clique cliqueMax = grasp2(matrizAdyacencias, listaAdyacencias, RCL, iterations);

//         Output
        std::cout << cliqueMax.frontera << " " << cliqueMax.vertices.size();
        for (std::list<int>::const_iterator it = cliqueMax.vertices.begin(); it != cliqueMax.vertices.end(); ++it) {
            std::cout << " " << *it ;
        }
        std::cout << std::endl;
    }
    return 0;
}