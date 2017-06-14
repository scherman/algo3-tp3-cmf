#include <iostream>
#include "Clique.h"
#include "Eje.h"
#include "DisjointSet.h"
#include <list>
#include "stringTokenizer.hpp"
#include <fstream>

Clique * filtrarPeorClique(Clique *cliqueA, Clique *cliqueB) {
    if (cliqueA->frontera >= cliqueA->frontera) {
        delete cliqueB;
        return cliqueA;
    } else {
        delete cliqueA;
        return cliqueB;
    }
}

int calcularFrontera(const DisjointSet::Subset &set, std::list<int> * listaAdyacencias){
    int frontera = set.nodos.size() * (set.nodos.size() - 1);
    for (std::list<int>::const_iterator it = set.nodos.begin(); it != set.nodos.end(); ++it) {
        frontera += listaAdyacencias[*it].size();
    }
}

Clique * cmf(DisjointSet &uds, std::list<Eje> &ejesNoAgregados, std::list<int> *listaAdyacencias){
    if (ejesNoAgregados.size() == 0) {
        std::list<int> V;
        V.push_back(0);
        return new Clique(V, 0);
    } else if (ejesNoAgregados.size() == 1) {
        Eje &e = ejesNoAgregados.front();
        ejesNoAgregados.pop_front();

        std::list<int> V;
        V.push_back(e.origen);
        V.push_back(e.destino);

        return new Clique(V, 0);
    } else {
        Eje &e = ejesNoAgregados.front();
        ejesNoAgregados.pop_front();

        DisjointSet udsConE = DisjointSet(uds);
        udsConE.unify(e);

        DisjointSet::Subset nuevoSet = uds.find(e.origen);
        Clique *cmfMax = nullptr;
        if (nuevoSet.esClique()) {
            int fronteraNuevaClique = calcularFrontera(nuevoSet, listaAdyacencias);
            std::list<int> &nodosNuevaClique = nuevoSet.nodos;
            cmfMax = new Clique(nodosNuevaClique, fronteraNuevaClique);
        }

        Clique *cmfConE = cmf(udsConE, ejesNoAgregados, listaAdyacencias);
        Clique *cmfSinE = cmf(uds, ejesNoAgregados, listaAdyacencias);

        // Filtrar clique max
        cmfMax = cmfMax != nullptr ? filtrarPeorClique(cmfConE, cmfMax) : cmfConE;
        cmfMax = filtrarPeorClique(cmfMax, cmfSinE);
        return cmfMax;
    }
}

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
            int v1, v2, p;
            getline(input, linea);
            strTok.tokenize(linea, ' ');
            v1 = stoi(strTok[0]);
            v2 = stoi(strTok[1]);
            listaAdyacencias[v1].push_back(v2);
            listaAdyacencias[v2].push_back(v1);
            listaIncidencias.push_back({v1,v2});

        }
        DisjointSet uds = DisjointSet(n);
		Clique *cliqueMax = cmf(uds, listaIncidencias, listaAdyacencias);
        std::cout << "CMF: " << cliqueMax->frontera << std::endl;
        delete cliqueMax;
	}
	input.close();
    return 0;
}



