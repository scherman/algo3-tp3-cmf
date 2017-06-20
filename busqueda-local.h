//
// Created by jscherman on 20/06/17.
//

#ifndef TEST_DEBUGGER_BUSQUEDA_LOCAL_H_H
#define TEST_DEBUGGER_BUSQUEDA_LOCAL_H_H

#include "cmf-heuristica-constructiva-golosa.h"

Clique & busquedaLocal(int n, Clique &clique, std::list<int> *grafoOriginal);

Clique & busquedaLocal2(int n, Clique &clique, std::list<Eje>& listaIncidencias);


#endif //TEST_DEBUGGER_BUSQUEDA_LOCAL_H_H