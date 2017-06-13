#include <iostream>
#include "Clique.h"
#include "Eje.h"
#include "DisjointSet.h"
#include <list>

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}

Clique & cmf(DisjointSet &uds, std::list<Eje> &ejesNoAgregados){
//    if (ejesNoAgregados.size() == 0) {
//
//    } else if (ejesNoAgregados.size() == 1) {
//
//    } else {
//        Eje &e = ejesNoAgregados.front();
//        ejesNoAgregados.pop_front();
//
//        DisjointSet udsConE = DisjointSet(uds);
//        DisjointSet udsSinE = DisjointSet(uds);
//        udsConE.unify(e);
//
//        std::list<Clique> cliquesCandidatos;
//
//        Clique &cmfSinE = cmf(udsConE, ejesNoAgregados);
//        Clique &cmfConE = cmf(udsSinE, ejesNoAgregados);
//
//        // Chequeo si se forma clique cuando agrego e
//        DisjointSet::Subset &setOrigen = uds.find(e.origen);
//        DisjointSet::Subset &setDestino = uds.find(e.destino);
//        if (setDestino == setOrigen) {
//            int nSet = setOrigen.size;
//            int mSet = setOrigen.edges.size();
//            if  (mSet == (nSet*(nSet-1))/2 - 1) {
//
//                // Forma clique. Tenerlo en cuenta para devolver
//                if (cmfConE.frontera > cmfSinE.frontera) {
//                    if (cmfConE.frontera > uds)
//                } else {
//
//                }
//
//            }
//        }
//
//
//        if ()
//    }
}

