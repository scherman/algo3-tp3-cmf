//
// Created by jscherman on 20/06/17.
//

#include "grasp.h"
#include <fstream>
#include "stringTokenizer.hpp"
#include "graspv2.h"

int main(int argc, char** argv) {
//    {
//        int cantInstanciasPorRCL = 10, minRCL = 1, iterations = 1, n = 50, maxRCL = n, m = 800, saltarDeA = 1;
//        variarRCL(cantInstanciasPorRCL, minRCL, maxRCL, iterations, n, m, saltarDeA);
//    }
//    {
//        int cantInstanciasPorIterations = 2, minIterations = 1, maxIterations = 1000, RCL = 3, n = 50, m = 800, saltarDeA = 1;
//        variarIterations(cantInstanciasPorIterations, minIterations, maxIterations, RCL, n, m, saltarDeA);
//    }
//    {
//        int cantInstanciasPorN = 2, minN = 1, maxN = 10000, RCL = 3, iterations = 20, saltarDeA = 30;
//        escribirTiemposVariandoNGrasp(cantInstanciasPorN, minN, maxN, RCL, iterations, saltarDeA);
//    }
//    {
//        int cantInstanciasPorM = 3, constanteN = 150, RCL = 3, iterations = 20, saltarDeA = 30;
//        escribirTiemposVariandoMGrasp(cantInstanciasPorM, constanteN, RCL, iterations, saltarDeA);
//    }
    {
        int cantInstanciasPorM = 10, constanteN = 100, RCL = 3, iterations = 20, saltarDeA = 5;
        porcentajeErrorVariandoMGrasp(cantInstanciasPorM, constanteN, RCL, iterations, saltarDeA);
    }

//    int RCL = 3;
//    int iterations = 20;
//
//    unsigned n, m;
//    stringTokenizer strTok;
//    string linea;
//    fstream input(argv[1], fstream::in);
//    while(getline(input, linea)){
//        if(linea == "-1 -1") continue;
//        strTok.tokenize(linea, ' ');
//        n = stoi(strTok[0]);
//        m = stoi(strTok[1]);
//        std::vector<std::list<int>> listaAdyacencias(n);
//        std::vector<std::vector<bool>> matrizAdyacencias(n, std::vector<bool>(n));
//        for (int j = 0; j < n; ++j) {
//            for (int i = 0; i < n; ++i) {
//                matrizAdyacencias[j][i] = false;
//            }
//        }
//
//        int c = 0;
//        for(int i = 0; i < m; i++){
//            int v1, v2;
//            getline(input, linea);
//            strTok.tokenize(linea, ' ');
//            v1 = stoi(strTok[0]);
//            v2 = stoi(strTok[1]);
//            listaAdyacencias[v1].push_back(v2);
//            listaAdyacencias[v2].push_back(v1);
//            matrizAdyacencias[v1][v2] = true;
//            matrizAdyacencias[v2][v1] = true;
//        }
//        Clique cliqueMax = grasp2(matrizAdyacencias, listaAdyacencias, RCL, iterations);
//
//         Output
//        std::cout << cliqueMax.frontera << " " << cliqueMax.vertices.size();
//        for (std::list<int>::const_iterator it = cliqueMax.vertices.begin(); it != cliqueMax.vertices.end(); ++it) {
//            std::cout << " " << *it ;
//        }
//        std::cout << std::endl;
//    }
//    input.close();
    return 0;
}