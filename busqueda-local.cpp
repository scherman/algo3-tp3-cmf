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