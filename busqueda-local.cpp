//
// Created by jscherman on 20/06/17.
//

#include "busqueda-local.h"

Clique & busquedaLocal(int n, Clique &clique, std::list<int> *grafoOriginal) {
    bool nodosEnClique[n] = {false};
    for (std::list<int>::iterator it = clique.vertices.begin(); it != clique.vertices.end(); ++it) {
        nodosEnClique[*it] = true;
    }

    int mejoraSolucion = true;
    while (mejoraSolucion) {
        mejoraSolucion = false;
        for (int i = 0; i < n; ++i) {
            if (!nodosEnClique[i] && mejoraFrontera(&clique, i, grafoOriginal) && extiendeClique(&clique, i, grafoOriginal)) {
                // Extiendo clique
                clique.frontera = clique.frontera - 2*clique.vertices.size() + grafoOriginal[i].size();
                clique.vertices.push_back(i);
                nodosEnClique[i] = true;
                mejoraSolucion = true;
                break;
            }
        }
    }
    return clique;
}


Clique &busquedaLocal2(int n, Clique &clique, std::list<Eje>& listaIncidencias){
    std::list<int> listaAdyacencias[n];
    for (std::list<Eje>::iterator it = listaIncidencias.begin(); it != listaIncidencias.end(); ++it) {
        Eje &eje = *it;
        listaAdyacencias[eje.origen].push_back(eje.destino);
        listaAdyacencias[eje.destino].push_back(eje.origen);
    }

    return busquedaLocal(n, clique, listaAdyacencias);
}