//
// Created by jscherman on 20/06/17.
//

#include "busqueda-local.h"
//O(n^3)
Clique& busquedaLocalExtendiendoClique(Clique &clique, std::vector<std::list<int>> &listaIncidencias) {
    int n = listaIncidencias.size();

    bool nodosEnClique[n] = {false};
    for (std::list<int>::iterator it = clique.vertices.begin(); it != clique.vertices.end(); ++it) {
        nodosEnClique[*it] = true;
    }

    int mejoraSolucion = true;
    while (mejoraSolucion) { // O(n)
        mejoraSolucion = false;
        for (int i = 0; i < n; ++i) {
            if (!nodosEnClique[i] && mejoraFrontera(clique, i, listaIncidencias) && extiendeClique(clique, i, listaIncidencias)) { // O(n^2)
                // Extiendo clique
                clique.frontera = clique.frontera - 2*clique.vertices.size() + listaIncidencias[i].size();
                clique.vertices.push_back(i);
                nodosEnClique[i] = true;
                mejoraSolucion = true;
                break;
            }
        }
    }
    return clique;
}
