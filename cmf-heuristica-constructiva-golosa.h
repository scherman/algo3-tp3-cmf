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

bool mejoraFrontera(Clique* clique, int vecino, std::list<int> listaAdyacencias[]);

bool extiendeClique(Clique* clique, int vecino, std::list<int> listaAdyacencias[]);

void extenderClique (Clique* clique, int vecino, bool **matrizAdyacencias, std::list<int> listaAdyacencias[]);

Clique hconstructiva(int n, std::list<int> *listaAdyacencias);
Clique heuristicaConstructiva(int n, std::list<Eje> &listaIncidencias);

#endif //TEST_DEBUGGER_CMF_HEURISTICA_CONSTRUCTIVA_GOLOSA_H