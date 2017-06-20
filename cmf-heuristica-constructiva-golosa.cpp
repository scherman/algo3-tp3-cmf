//
// Created by jscherman on 15/06/17.
//

#include "cmf-heuristica-constructiva-golosa.h"

bool mejoraFrontera(Clique* clique, int vecino, std::list<int> listaAdyacencias[]) {
    int gradoVecino = listaAdyacencias[vecino].size();
    return (clique->frontera - 2*clique->vertices.size() + gradoVecino) > clique->frontera;
}

bool extiendeClique(Clique* clique, int vecino, std::list<int> listaAdyacencias[]) {
    std::list<int> &adyacentesVecino = listaAdyacencias[vecino];
    std::list<int> &verticesClique = clique->vertices;

    for (std::list<int>::const_iterator itVClique = verticesClique.begin(); itVClique != verticesClique.end(); ++itVClique) {
        int verticeClique = *itVClique;
        bool verticeCliqueIncideEnVecino = false;
        for (std::list<int>::const_iterator itAdyacentes = adyacentesVecino.begin(); itAdyacentes != adyacentesVecino.end(); ++itAdyacentes) {
            int adyacenteVecino = *itAdyacentes;

            if (verticeClique == adyacenteVecino) verticeCliqueIncideEnVecino = true;
        }
        if (!verticeCliqueIncideEnVecino) return false;
    }
    return true;
}

void extenderClique (Clique* clique, int vecino, bool **matrizAdyacencias, std::list<int> listaAdyacencias[]) {
    for (std::list<int>::const_iterator itVClique = clique->vertices.begin(); itVClique != clique->vertices.end(); ++itVClique) {
        int verticeClique = *itVClique;
        matrizAdyacencias[verticeClique][vecino] = true;
        matrizAdyacencias[vecino][verticeClique] = true;
    }
    clique->frontera = clique->frontera - 2*clique->vertices.size() + listaAdyacencias[vecino].size();
    clique->vertices.push_back(vecino);
}

/*
 * 1. Empezar del nodo con mayor grado
 * 2. Moverse al adyacente con mayor grado.
 *      a. Si extiende la clique del anterior y aumenta la frontera, agregarlo
 *      b. Sino, asignarle una clique nueva y agregarla a la lista de cliques.
 * 3. Repetir 2 hasta que no haya adyacentes
 * 4. Devolver la clique con mayor frontera de la lista de cliques
 */
Clique hconstructiva(int n, std::list<int> *listaAdyacencias){

    // Busco nodo de mayor grado
    int actual = 0;
    for (int i = 0; i < n; ++i) {
        actual = listaAdyacencias[i].size() > listaAdyacencias[actual].size() ? i : actual;
    }

    Clique *cliques[n] = {nullptr};
    std::list<int> V;
    V.push_back(actual);
    cliques[actual] = new Clique(V, listaAdyacencias[actual].size());

    bool **matrizAdyacencias= new bool*[n];
    for(int i = 0; i < n; ++i) {
        matrizAdyacencias[i] = new bool[n];
        for(int r = 0; r < n; ++r) {
            matrizAdyacencias[i][r] = (i == r);
        }
    }

    while(actual != -1) {
        std::list<int> &adyacentes = listaAdyacencias[actual];

        // Obtener vecinoDisponible de mayor grado
        int vecinoDisponible = -1;
        for (std::list<int>::const_iterator it = adyacentes.begin(); it != adyacentes.end(); ++it) {
            if (matrizAdyacencias[*it][actual]) continue;
            if (cliques[*it] != nullptr) continue;
            if(vecinoDisponible == -1 || listaAdyacencias[vecinoDisponible].size() < listaAdyacencias[*it].size()) {
                vecinoDisponible = *it;
            }
        }

        if (vecinoDisponible != -1) {
            // Puedo agregar otro nodo
            matrizAdyacencias[actual][vecinoDisponible] = true;
            matrizAdyacencias[vecinoDisponible][actual] = true;

            Clique *cliqueActual = cliques[actual];

            if (mejoraFrontera(cliqueActual, vecinoDisponible, listaAdyacencias)
                    && extiendeClique(cliqueActual, vecinoDisponible, listaAdyacencias)) {
                // vecinoActual esta en la clique de actual y aumenta la frontera. Lo agrego a la clique
                extenderClique(cliqueActual, vecinoDisponible, matrizAdyacencias, listaAdyacencias);
                cliques[vecinoDisponible] = cliqueActual;
            } else {
                // Le asigno una clique nueva
                std::list<int> V;
                V.push_back(vecinoDisponible);
                Clique *cliqueNueva = new Clique(V, listaAdyacencias[vecinoDisponible].size());
                cliques[vecinoDisponible] = cliqueNueva;
            }
        }
        actual = vecinoDisponible;
    }

    // Busco entre las cliques encontradas la que tenga mayor frontera
    Clique* maxClique = nullptr;
    for (int j = 0; j < n; ++j) {
        if (cliques[j] == nullptr) continue;
//        if (cliques[j] == nullptr){
//            continue;
//        }  else {
//            std::cout << "v=" << j << ":" << *cliques[j] << std::endl;
//        }
        if (maxClique == nullptr || maxClique->frontera < cliques[j]->frontera) {
            maxClique = cliques[j];
        }
    }

    for(int i = 0; i < n; ++i) {
        delete matrizAdyacencias[i];
    }
    delete[] matrizAdyacencias;

    return *maxClique;
}

Clique heuristicaConstructiva(int n, std::list<Eje> &listaIncidencias) {
    std::list<int> listaAdyacencias[n];
    for (std::list<Eje>::iterator it = listaIncidencias.begin(); it != listaIncidencias.end(); ++it) {
        Eje &eje = *it;
        listaAdyacencias[eje.origen].push_back(eje.destino);
        listaAdyacencias[eje.destino].push_back(eje.origen);
    }

    return hconstructiva(n, listaAdyacencias);
}