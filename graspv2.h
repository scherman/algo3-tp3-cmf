//
// Created by jscherman on 08/07/17.
//

#ifndef TEST_DEBUGGER_GRASPV2_H
#define TEST_DEBUGGER_GRASPV2_H


#include "Clique.h"
#include <vector>

Clique* maxFrontera(int n, Clique **cliques);

Clique* randomizedGreedy(std::vector<std::vector<bool>> &matrizAdyacencias,
                         std::vector<std::list<int>> &listaAdyacencias,
                         int RCL);

Clique grasp2(std::vector<std::vector<bool>> &matriz,
              std::vector<std::list<int>> &listaAdyacencias,
              int RCL,
              int iterations);

#endif //TEST_DEBUGGER_GRASPV2_H
