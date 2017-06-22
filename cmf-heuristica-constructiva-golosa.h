//
// Created by jscherman on 20/06/17.
//

#ifndef TEST_DEBUGGER_CMF_HEURISTICA_CONSTRUCTIVA_GOLOSA_H
#define TEST_DEBUGGER_CMF_HEURISTICA_CONSTRUCTIVA_GOLOSA_H

#include <iostream>
#include "Clique.h"
#include "Eje.h"
#include "DisjointSet.h"
#include <list>
#include "Utils.h"
#include <fstream>
#include "stringTokenizer.hpp"

bool mejoraFrontera(Clique& clique, int vecino, std::vector<std::list<int>> &listaAdyacencias);

bool extiendeClique(Clique& clique, int vecino, std::vector<std::vector<bool>> &matrizAdyancias, std::vector<std::list<int>> &listaAdyacencias);

void extenderClique (Clique& clique, int vecino, std::vector<std::vector<bool>> &matrizAdyancias, std::vector<std::list<int>> &listaAdyacencias);

Clique* hconstructiva(std::vector<std::list<int>> &listaAdyacencias, int nodoInicial);

#endif //TEST_DEBUGGER_CMF_HEURISTICA_CONSTRUCTIVA_GOLOSA_H
