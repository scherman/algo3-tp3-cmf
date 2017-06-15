#include <iostream>
#include "Clique.h"
#include "Eje.h"
#include "DisjointSet.h"
#include <list>
#include "stringTokenizer.hpp"
#include <fstream>

Clique * filtrarPeorClique(Clique *cliqueA, Clique *cliqueB) {
    if (cliqueA->frontera >= cliqueB->frontera) {
        delete cliqueB;
        return cliqueA;
    } else {
        delete cliqueA;
        return cliqueB;
    }
}

int calcularFrontera(const std::list<int> &nodos, std::list<int> * listaAdyacencias){
    int frontera = -(nodos.size() * (nodos.size() - 1));
    for (std::list<int>::const_iterator it = nodos.begin(); it != nodos.end(); ++it) {
        frontera += listaAdyacencias[*it].size();
    }
    return frontera;
}

void imprimirEjes(std::list<Eje> & ejes) {
    for (std::list<Eje>::iterator it = ejes.begin(); it != ejes.end(); ++it) {
        std::cout << *it << " ";
    }
}


Clique * cmf(DisjointSet &uds, std::list<Eje> ejesNoAgregados, std::list<int> *listaAdyacencias, int &cliquesEncontradas){
    if (ejesNoAgregados.size() == 0) {
        std::list<int> V;
        V.push_back(0);
        return new Clique(V, 0);
    } else {
        Eje e = ejesNoAgregados.front();
        ejesNoAgregados.pop_front();

        DisjointSet udsConE = DisjointSet(uds);

        DisjointSet::Subset unionSets = udsConE.unify(e);
        Clique *cmfMax = nullptr;
        if (unionSets.esClique()) {
            int fronteraNuevaClique = calcularFrontera(unionSets.nodos, listaAdyacencias);
//            std:: cout << cliquesEncontradas++ << ") Frontera " << fronteraNuevaClique << ": ";
//            for (std::list<int>::const_iterator it = unionSets.nodos.begin(); it != unionSets.nodos.end(); ++it) {
//                std::cout << *it << " ";
//            }
//            std::cout << std::endl;
            std::list<int> &nodosNuevaClique = unionSets.nodos;
            cmfMax = new Clique(nodosNuevaClique, fronteraNuevaClique);
        }

        Clique *cmfConE = cmf(udsConE, ejesNoAgregados, listaAdyacencias, cliquesEncontradas);
        Clique *cmfSinE = cmf(uds, ejesNoAgregados, listaAdyacencias, cliquesEncontradas);

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
        std::cout << "n=" << n << ", m=" << m << ", E={ ";
        imprimirEjes(listaIncidencias);
        std::cout << "}" << std::endl;
        int cliques = 0;
		Clique *cliqueMax = cmf(uds, listaIncidencias, listaAdyacencias, cliques);
        std::cout << "CMF: [frontera= " << cliqueMax->frontera << ", nodos={ ";
        for (std::list<int>::const_iterator it = cliqueMax->vertices.begin(); it != cliqueMax->vertices.end(); ++it) {
            std::cout << *it << " ";
        }
        std:: cout << "} ] "<< std::endl;
        delete cliqueMax;
	}
	input.close();
    return 0;
}



