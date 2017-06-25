//
// Created by jscherman on 20/06/17.
//

#ifndef TEST_DEBUGGER_BUSQUEDA_LOCAL_H_H
#define TEST_DEBUGGER_BUSQUEDA_LOCAL_H_H

#include "cmf-heuristica-constructiva-golosa.h"

Clique& busquedaLocalExtendiendoClique(Clique &clique,
                                       std::vector<std::vector<bool>> &matrizAdyacencias,
                                       std::vector<std::list<int>> &listaIncidencias);

void porcentajeErrorVariandoMBusquedaLocal(int cantInstanciasPorM, int constanteN, int saltarDeA);
void escribirTiemposVariandoNBLocal(int cantInstanciasPorN, int minN, int maxN, int saltarDeA);
void escribirTiemposVariandoMBLocal(int cantInstanciasPorM, int constanteN, int saltarDeA);

#endif //TEST_DEBUGGER_BUSQUEDA_LOCAL_H_H
