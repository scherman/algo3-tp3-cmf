
#include "cmf-algo-exacto.h"

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
    for (std::list<Eje>::iterator it = listaIncidencias.begin(); it != listaIncidencias.end(); ++it) {
        Eje &eje = *it;
        listaAdyacencias[eje.origen].push_back(eje.destino);
        listaAdyacencias[eje.destino].push_back(eje.origen);
    }
    DisjointSet uds = DisjointSet(n);
    return cmfExacto(uds, listaIncidencias, listaAdyacencias);
}