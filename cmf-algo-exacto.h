//
// Created by jscherman on 20/06/17.
//

#ifndef TEST_DEBUGGER_CMF_ALGO_EXACTO_H_H
#define TEST_DEBUGGER_CMF_ALGO_EXACTO_H_H

#include <iostream>
#include "Clique.h"
#include "Eje.h"
#include "Utils.h"
#include <fstream>
#include "stringTokenizer.hpp"

Clique exactoBTVertices(Clique clique,
                        std::vector<std::vector<bool>> &matrizAdyacencias,
                        std::vector<std::list<int>> &listaAdyacencias,
                        int verticeActual);

Clique exactoBTVertices(std::vector<std::vector<bool>> &matrizAdyacencias,
                        std::vector<std::list<int>> &listaAdyacencias);

void escribirTiemposVariandoN(int cantInstanciasPorN, int minN, int maxN);
void escribirTiemposVariandoNSinEjes(int cantInstanciasPorN, int minN, int maxN);
void escribirTiemposVariandoM(int cantInstanciasPorM, int constanteN);
void escribirTiemposVariandoNMCompleto(int cantInstanciasPorN, int minN, int maxN);

#endif //TEST_DEBUGGER_CMF_ALGO_EXACTO_H_H
