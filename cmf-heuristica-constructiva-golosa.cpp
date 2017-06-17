//
// Created by jscherman on 15/06/17.
//

#include <iostream>
#include "Clique.h"
#include "Eje.h"
#include "DisjointSet.h"
#include <list>
#include "stringTokenizer.hpp"
#include <fstream>

bool mejoraFrontera(Clique* clique, int vecino, std::list<int> listaAdyacencias[]) {
    int gradoVecino = listaAdyacencias[vecino].size();
    return clique->frontera - clique->vertices.size() + gradoVecino >= clique->frontera;
}

bool extiendeClique(Clique* clique, int vecino, std::list<int> listaAdyacencias[]) {
    std::list<int> &adyacentesVecino = listaAdyacencias[vecino];
    std::list<int> &verticesClique = clique->vertices;
    for (std::list<int>::const_iterator itAdyacentes = adyacentesVecino.begin(); itAdyacentes != adyacentesVecino.end(); ++itAdyacentes) {
        int adyacenteVecino = *itAdyacentes;
        bool estaEnClique = false;
        for (std::list<int>::const_iterator itVClique = verticesClique.begin(); itVClique != verticesClique.end(); ++itVClique) {
            int verticeClique = *itVClique;
            if (verticeClique == adyacenteVecino) estaEnClique = true;
        }
        if (!estaEnClique) return false;
    }
    return true;
}

void extenderClique (Clique* clique, int vecino, bool **matrizAdyacencias, std::list<int> listaAdyacencias[]) {
    for (std::list<int>::const_iterator itVClique = clique->vertices.begin(); itVClique != clique->vertices.end(); ++itVClique) {
        int verticeClique = *itVClique;
        matrizAdyacencias[verticeClique][vecino] = true;
        matrizAdyacencias[vecino][verticeClique] = false;
    }
    clique->vertices.push_back(vecino);
    clique->frontera = clique->frontera - clique->vertices.size() + listaAdyacencias[vecino].size();
}

/*
 * 1. Empezar del nodo con mayor grado
 * 2. Moverse al adyacente con mayor grado.
 *      a. Si extiende la clique del anterior y aumenta la frontera, agregarlo
 *      b. Sino, asignarle una clique nueva y agregarla a la lista de cliques.
 * 3. Repetir 2 hasta que no haya adyacentes
 * 4. Devolver la clique con mayor frontera de la lista de cliques
 */
Clique * hconstructiva(int n, std::list<int> listaAdyacencias[]){

    // Busco nodo de mayor grado
    int actual = 0;
    for (int i = 0; i < n; ++i) {
        actual = listaAdyacencias[i].size() > listaAdyacencias[actual].size() ? i : actual;
    }


    bool **matrizAdyacencias= new bool*[n];
    for(int i = 0; i < n; ++i) {
        matrizAdyacencias[i] = new bool[n];
        for(int r = 0; r < n; ++r) {
            if (i == r) {
                matrizAdyacencias[i][r] = true;
            } else {
                matrizAdyacencias[i][r] = false;
            }
        }
    }

    Clique *cliques[n] = {nullptr};
    while(actual != -1) {
        std::list<int> &adyacentes = listaAdyacencias[actual];

        // Obtener vecino de mayor grado
        int vecino = -1;
        for (std::list<int>::const_iterator it = adyacentes.begin(); it != adyacentes.end(); ++it) {
            if(vecino == -1 || listaAdyacencias[vecino].size() < listaAdyacencias[*it].size()) {
                vecino = *it;
            }
        }
        if (vecino != -1) {
            matrizAdyacencias[actual][vecino] = true;
            matrizAdyacencias[vecino][actual] = true;

            Clique *cliqueActual = cliques[actual];

            if (mejoraFrontera(cliqueActual, vecino, listaAdyacencias)
                    && extiendeClique(cliqueActual, vecino, listaAdyacencias)) {
                extenderClique(cliqueActual, vecino, matrizAdyacencias, listaAdyacencias);
            } else {
                // Le asigno nueva clique
                std::list<int> V;
                V.push_back(vecino);
                Clique *cliqueNueva = new Clique(V, listaAdyacencias[vecino].size());
                cliques[vecino] = cliqueNueva;
            }
        }
        actual = vecino;
    }

    Clique* maxClique = nullptr;
    for (int j = 0; j < n; ++j) {
        if (maxClique == nullptr || maxClique->frontera < cliques[j]->frontera) {
            maxClique = cliques[j];
        }
    }
    return maxClique;
}

int main(int argc, char** argv) {
    std::cout << "hola" << std::endl;

    unsigned n, m;
    stringTokenizer strTok;
    string linea;
    fstream input(argv[1], fstream::in);
    while(getline(input, linea)){
        if(linea == "-1 -1") continue;
        strTok.tokenize(linea, ' ');
        n = stoi(strTok[0]);
        m = stoi(strTok[1]);
        std::list<int> listaAdyacencias[n];
        std::list<Eje> listaIncidencias;
        int c = 0;
        for(int i = 0; i <= m; i++){
            int v1, v2;
            getline(input, linea);
            strTok.tokenize(linea, ' ');
            v1 = stoi(strTok[0]);
            v2 = stoi(strTok[1]);
            listaAdyacencias[v1].push_back(v2);
            listaAdyacencias[v2].push_back(v1);
            listaIncidencias.push_back({v1,v2});
        }
        DisjointSet uds = DisjointSet(n);
        Clique *cliqueMax = hconstructiva(n, listaAdyacencias);
    }
    input.close();
    return 0;
}
