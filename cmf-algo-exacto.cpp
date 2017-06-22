
#include "cmf-algo-exacto.h"

Clique& maxFrontera(Clique &cliqueA, Clique &cliqueB) {
    if (cliqueA.frontera >= cliqueB.frontera) {
        return cliqueA;
    } else {
        return cliqueB;
    }
}

bool extiendeClique(
        Clique clique,
        std::vector<std::vector<bool>> &matrizAdyacencias,
        std::vector<std::list<int>> &listaAdyacencias,
        int verticeActual) {
    for (std::list<int>::iterator it = clique.vertices.begin(); it != clique.vertices.end(); ++it) {
        if (!matrizAdyacencias[*it][verticeActual]) return false;
    }
    return true;
}

Clique exactoBTVertices(int n,
                         Clique clique,
                         std::vector<std::vector<bool>> &matrizAdyacencias,
                         std::vector<std::list<int>> &listaAdyacencias,
                         int verticeActual) {
    if (verticeActual >= n) {
        return clique;
    } else {

        // Primer llamada recursiva (sin agregar vecinoActual a la clique)
        Clique cliqueSinVerticeActual = exactoBTVertices(n, clique, matrizAdyacencias, listaAdyacencias, verticeActual + 1);

        if (extiendeClique(clique, matrizAdyacencias, listaAdyacencias, verticeActual)) {
            clique.frontera = clique.frontera - 2 * clique.vertices.size() + listaAdyacencias[verticeActual].size();
            clique.vertices.push_back(verticeActual);

            // Segunda llamada recursiva (agregando vecinoActual a la clique)
            Clique cliqueConVerticeActual = exactoBTVertices(n, clique, matrizAdyacencias, listaAdyacencias, verticeActual + 1);

            return maxFrontera(cliqueConVerticeActual, cliqueSinVerticeActual);
        } else {
            return cliqueSinVerticeActual;
        }
    }
}

Clique exactoBTVertices(int n,
                        std::vector<std::vector<bool>> &matrizAdyacencias,
                        std::vector<std::list<int>> &listaAdyacencias) {
    std::list<int> V;
    return exactoBTVertices(n, Clique(V, 0), matrizAdyacencias, listaAdyacencias, 0);
}


