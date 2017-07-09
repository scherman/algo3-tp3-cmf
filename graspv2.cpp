//
// Created by jscherman on 08/07/17.
//

#include "graspv2.h"
#include "busqueda-local.h"
#include <algorithm>

class Sorter {
    bool *usados;
    std::vector<std::list<int>> &listaAdyacencias;
public:
    Sorter(bool *usados, std::vector<std::list<int>> &lista) : usados(usados), listaAdyacencias(lista) {}
    bool operator()(const int o1, const int o2) const {
        return (!usados[o1] && usados[o2]) || (listaAdyacencias[o1].size() > listaAdyacencias[o2].size());
    }
};

Clique* randomizedGreedy(std::vector<std::vector<bool>> &matrizAdyacencias,
                      std::vector<std::list<int>> &listaAdyacencias,
                      int RCL) {
    int n = listaAdyacencias.size();

    bool usados[n] = {false};
    Clique *cliques[n] = {nullptr};
    std::vector<int> nodos(n);
    for (int k = 0; k < n; ++k) nodos[k] = k;

    Sorter sorter(usados, listaAdyacencias);

    // Agarro un nodo en RCL
    std::sort(nodos.begin(), nodos.end(), sorter);
    int actual = rand() % std::min(RCL, n);

    // Le asigno clique nueva
    std::list<int> V;
    V.push_back(actual);
    usados[actual] = true;
    cliques[actual] = new Clique(V, listaAdyacencias[actual].size());

    bool hayVecinosDisponibles = true;
    while (hayVecinosDisponibles) {
//        std::cout << "Actual: " << actual << " | ";
        std::list<int> &adyacentes = listaAdyacencias[actual];

        // Obtener vecinosDisponibles
        std::vector<int> vecinosDisponibles;
        for (std::list<int>::const_iterator it = adyacentes.begin(); it != adyacentes.end(); ++it) { // O(n)
            if (!usados[*it]) vecinosDisponibles.push_back(*it);
        }

        if (!vecinosDisponibles.empty()) {
            // Se puede agregar otro nodo
            Clique *cliqueActual = cliques[actual];

            // Ordeno por grado y tomo uno de RCL
            std::sort(vecinosDisponibles.begin(), vecinosDisponibles.end(), sorter);
            int vecino = vecinosDisponibles[rand() % std::min(RCL, static_cast<int>(vecinosDisponibles.size()))];
//            for (int i = 0; i < vecinosDisponibles.size(); ++i) {
//                std::cout << vecinosDisponibles[i] << "(" << usados[vecinosDisponibles[i]] << ", " << listaAdyacencias[vecinosDisponibles[i]].size() << ") ";
//            }
//            std::cout << " | Elegido(entre 0 y " << std::min(RCL, static_cast<int>(vecinosDisponibles.size())) << "): " << vecino << std::endl;

            if (mejoraFrontera(*cliqueActual, vecino, listaAdyacencias) // O(1)
                && extiendeClique(*cliqueActual, vecino, matrizAdyacencias, listaAdyacencias)) {  // O(n^2)
                // vecinoActual esta en la clique de actual y aumenta la frontera. Lo agrego a la clique
                extenderClique(*cliqueActual, vecino, matrizAdyacencias, listaAdyacencias); // O(n)
                cliques[vecino] = cliqueActual;
            } else {
                // Le asigno una clique nueva a vecinoActual
                std::list<int> V;
                V.push_back(vecino);
                Clique *cliqueNueva = new Clique(V, listaAdyacencias[vecino].size());
                cliques[vecino] = cliqueNueva;
            }
            usados[vecino] = true;

            actual = vecino;
        } else {
            hayVecinosDisponibles = false;
//            std::cout << "No hay mas disponibles" << std::endl;
        }
    }

    return maxFrontera(n, cliques);
}

Clique grasp2(std::vector<std::vector<bool>> &matriz,
              std::vector<std::list<int>> &listaAdyacencias,
              int RCL,
              int iterations) {
    Clique cmf;
    for (int i = 0; i < iterations; ++i) {
        Clique *candidato = randomizedGreedy(matriz, listaAdyacencias, RCL);
        Clique candidatoMejorado = busquedaLocalExtendiendoClique(*candidato, matriz, listaAdyacencias);
        if (candidatoMejorado.frontera > cmf.frontera) cmf = candidatoMejorado;
        delete candidato;
    }
    return cmf;
}