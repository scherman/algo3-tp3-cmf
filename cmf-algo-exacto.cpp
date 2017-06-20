
#include <iostream>
#include "Clique.h"
#include "Eje.h"
#include "DisjointSet.h"
#include "Utils.h"
#include <fstream>
#include "stringTokenizer.hpp"

Clique * filtrarPeorClique(Clique *cliqueA, Clique *cliqueB) {
    if (cliqueA->frontera >= cliqueB->frontera) {
        delete cliqueB;
        return cliqueA;
    } else {
        delete cliqueA;
        return cliqueB;
    }
}

int calcularFrontera(const std::list<int> &nodos, std::list<int> listaAdyacencias[]){
    int frontera = -(nodos.size() * (nodos.size() - 1));
    for (std::list<int>::const_iterator it = nodos.begin(); it != nodos.end(); ++it) {
        frontera += listaAdyacencias[*it].size();
    }
    return frontera;
}

Clique * cmfExacto(DisjointSet &uds, std::list<Eje> ejesNoAgregados, std::list<int> *listaAdyacencias){
    if (ejesNoAgregados.size() == 0) {
        // m=0 => Todos los vertices son aislados (o sea G=nK1). Como n>=1, tomamos a la clique K1 = 0 con la frontera 0 (porque no hay ejes)
        std::list<int> V;
        V.push_back(0);
        return new Clique(V, 0);
    } else {
        Clique *cmfMax = nullptr;

        Eje e = ejesNoAgregados.front();
        ejesNoAgregados.pop_front();

        // Ver si la componente conexa que forma e es una clique.
        DisjointSet udsConE = DisjointSet(uds);
        DisjointSet::Subset unionSets = udsConE.unify(e);
        if (unionSets.esClique()) {
            cmfMax = new Clique(unionSets.nodos, calcularFrontera(unionSets.nodos, listaAdyacencias));
        }

        // Llamadas a las 2 ramas (con y sin e respectivamente)
        Clique *cmfConE = cmfExacto(udsConE, ejesNoAgregados, listaAdyacencias);
        Clique *cmfSinE = cmfExacto(uds, ejesNoAgregados, listaAdyacencias);

        // Filtrar entre la mejor clique entre cmfConE, cmfSinE, cmfMax
        cmfMax = cmfMax != nullptr ? filtrarPeorClique(cmfConE, cmfMax) : cmfConE;
        cmfMax = filtrarPeorClique(cmfMax, cmfSinE);
        return cmfMax;
    }
}

Clique * cmfExacto(int n, int m, std::list<Eje> &listaIncidencias) {
    std::list<int> listaAdyacencias[n];
    for (std::list<Eje>::iterator it = listaIncidencias.end(); it != listaIncidencias.end(); ++it) {
        Eje &eje = *it;
        listaAdyacencias[eje.origen].push_back(eje.destino);
        listaAdyacencias[eje.destino].push_back(eje.origen);
    }
    DisjointSet uds = DisjointSet(n);
    return cmfExacto(uds, listaIncidencias, listaAdyacencias);
}

//int main(int argc, char** argv) {
//    int n = 5, m = 10;
//    std::cout << "asd" << std::endl;
//    std::list<Eje> grafo = Utils::generarGrafo(n, m, false, 0, 0);
//    std::cout << "Exacto: " << cmfExacto(n, m, grafo) << std::endl;
////    std::cout << "Constructiva: " << heuristicaConstructiva(n, grafo) << std::endl;
//
//    return 0;
//}


int main(int argc, char** argv) {
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
        for(int i = 0; i < m; i++){
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
        Clique *cliqueMax = cmfExacto(uds, listaIncidencias, listaAdyacencias);

        // Output
        std::cout << cliqueMax->frontera << " " << cliqueMax->vertices.size();
        for (std::list<int>::const_iterator it = cliqueMax->vertices.begin(); it != cliqueMax->vertices.end(); ++it) {
            std::cout << " " << *it ;
        }
        std::cout << std::endl;


        delete cliqueMax;
    }
    input.close();
    return 0;
}