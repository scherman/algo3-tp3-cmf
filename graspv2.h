//
// Created by jscherman on 08/07/17.
//

#ifndef TEST_DEBUGGER_GRASPV2_H
#define TEST_DEBUGGER_GRASPV2_H


#include "Clique.h"
#include <vector>

Clique* randomizedGreedy(std::vector<std::vector<bool>> &matrizAdyacencias,
                         std::vector<std::list<int>> &listaAdyacencias,
                         int RCL);

Clique grasp2(std::vector<std::vector<bool>> &matriz,
              std::vector<std::list<int>> &listaAdyacencias,
              int RCL,
              int iterations);

void variarRCL(int cantInstanciasPorRCL, int minRCL, int maxRCL, int iterations, int n, int m, int saltarDeA);
void variarIterations(int cantInstanciasPorIterations, int minIterations, int maxIterations, int RCL, int n, int m, int saltarDeA);
void escribirTiemposVariandoNGrasp(int cantInstanciasPorN, int minN, int maxN, int RCL, int iterations, int saltarDeA);
void escribirTiemposVariandoNM0Grasp(int cantInstanciasPorN, int minN, int maxN, int RCL, int iterations, int saltarDeA);
void escribirTiemposVariandoNMCompletoGrasp(int cantInstanciasPorN, int minN, int maxN, int RCL, int iterations, int saltarDeA);
void escribirTiemposVariandoMGrasp(int cantInstanciasPorM, int constanteN, int RCL, int iterations, int saltarDeA);
void porcentajeErrorVariandoMGrasp(int cantInstanciasPorM, int constanteN, int RCL, int iterations, int saltarDeA);

#endif //TEST_DEBUGGER_GRASPV2_H
