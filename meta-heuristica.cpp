//
// Created by jscherman on 20/06/17.
//

#include "meta-heuristica.h"
#include "cmf-heuristica-constructiva-golosa.h"
#include "busqueda-local.h"

// O(k(n+hconstructiva+busquedaLocal))
Clique* grasp(std::vector<std::list<int>> &listaAdyacencias, int k) {
    if (k > listaAdyacencias.size()) throw std::invalid_argument("No puede haber mas de n candidatos!");

    int n = listaAdyacencias.size();

    Clique *maxClique = nullptr;
    bool usados[n] = {false};
    for (int l = 0; l < k; ++l) {

        int actual = 0;
        for (int i = 0; i < n; ++i) {
            if (usados[i]) continue;
            actual = listaAdyacencias[i].size() > listaAdyacencias[actual].size() ? i : actual;
        }
        usados[actual] = true;

        Clique *clique = hconstructiva(listaAdyacencias, actual);
        clique = &busquedaLocalExtendiendoClique(*clique, listaAdyacencias);
        if (maxClique == nullptr || maxClique->frontera < clique->frontera) {
            delete maxClique;
            maxClique = clique;
        } else {
            delete clique;
        }
    }
    return maxClique;
}
