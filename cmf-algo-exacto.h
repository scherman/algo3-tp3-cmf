//
// Created by jscherman on 20/06/17.
//

#ifndef TEST_DEBUGGER_CMF_ALGO_EXACTO_H_H
#define TEST_DEBUGGER_CMF_ALGO_EXACTO_H_H

#include <iostream>
#include "Clique.h"
#include "Eje.h"
#include "DisjointSet.h"
#include "Utils.h"
#include <fstream>
#include "stringTokenizer.hpp"

Clique * exactoBT(DisjointSet &uds, std::list<Eje> ejesNoAgregados, std::vector<std::list<int>> &listaAdyacencias);

Clique * exactoBT(int n, std::list<Eje> &listaIncidencias);
Clique * exactoBT(std::vector<std::list<int>> &listaAdyacencias);

#endif //TEST_DEBUGGER_CMF_ALGO_EXACTO_H_H
