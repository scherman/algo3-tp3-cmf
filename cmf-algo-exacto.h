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

Clique * cmfExacto(DisjointSet &uds, std::list<Eje> ejesNoAgregados, std::list<int> *listaAdyacencias);

Clique * cmfExacto(int n, int m, std::list<Eje> &listaIncidencias);

#endif //TEST_DEBUGGER_CMF_ALGO_EXACTO_H_H
