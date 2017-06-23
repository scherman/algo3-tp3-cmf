
#include "cmf-algo-exacto.h"
#include <sstream>
#include <chrono>

Clique& maxFrontera(Clique &cliqueA, Clique &cliqueB) {
//    std::cout << cliqueA << std::endl;
//    std::cout << cliqueB << std::endl;
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

Clique exactoBTVertices(Clique clique,
                         std::vector<std::vector<bool>> &matrizAdyacencias,
                         std::vector<std::list<int>> &listaAdyacencias,
                         int verticeActual) {
    int n = listaAdyacencias.size();
    if (verticeActual >= n) {
        return clique;
    } else {

        // Primer llamada recursiva (sin agregar vecinoActual a la clique)
        Clique cliqueSinVerticeActual = exactoBTVertices(clique, matrizAdyacencias, listaAdyacencias, verticeActual + 1);

        if (extiendeClique(clique, matrizAdyacencias, listaAdyacencias, verticeActual)) {
            clique.frontera = clique.frontera - 2 * clique.vertices.size() + listaAdyacencias[verticeActual].size();
            clique.vertices.push_back(verticeActual);
            // Segunda llamada recursiva (agregando vecinoActual a la clique)
            Clique cliqueConVerticeActual = exactoBTVertices(clique, matrizAdyacencias, listaAdyacencias, verticeActual + 1);
            return maxFrontera(cliqueConVerticeActual, cliqueSinVerticeActual);
        } else {
            return cliqueSinVerticeActual;
        }
    }
}

Clique exactoBTVertices(std::vector<std::vector<bool>> &matrizAdyacencias,
                        std::vector<std::list<int>> &listaAdyacencias) {
    std::list<int> V;
    return exactoBTVertices(Clique(V, 0), matrizAdyacencias, listaAdyacencias, 0);
}

void escribirTiemposVariandoN(int cantInstanciasPorN, int minN, int maxN){
    std::string nombreArchivo = "tiempos-exacto-variando-n";

    std::stringstream ss;
    ss <<  "/home/jscherman/CLionProjects/algo3-tp3-cmf/datos/" << nombreArchivo << ".csv";
    std::ofstream a_file (ss.str());

    a_file << "n, m, tiempoTotal" << std::endl;

    int m = minN*(minN-1)/2;
    std::cout << "Variando n: {m=" << m << "} => " << minN << " <= n <= " << maxN << std::endl;
    for (int i = minN; i <= maxN; ++i) {

        long long tiempoTotal = 0;
        for (int j = 0; j < cantInstanciasPorN; ++j) {
            std::vector<std::list<int>> listaAdyacencias = Utils::generarListaAdyacencias(i, m, false, 0, 0);
            std::vector<std::vector<bool>> matrizAdyacencias = Utils::aMatrizAdyacencias(listaAdyacencias);
            auto tpi = std::chrono::high_resolution_clock::now();
            Clique clique = exactoBTVertices(matrizAdyacencias, listaAdyacencias);
            auto tpf = std::chrono::high_resolution_clock::now();
            auto tiempo = std::chrono::duration_cast<std::chrono::nanoseconds>(tpf-tpi).count();
            tiempoTotal+= tiempo;
        }

        tiempoTotal = tiempoTotal/ cantInstanciasPorN;
        std::cout << i << ", " << m << ", " <<  tiempoTotal << std::endl ;
        a_file << i <<  ", " << m << ", " << tiempoTotal << std::endl;
    }

    a_file.close();
    std::cout << "Listo!" << std::endl;
}

void escribirTiemposVariandoM(int cantInstanciasPorM, int constanteN){
    std::string nombreArchivo = "tiempos-exacto-variando-m";

    std::stringstream ss;
    ss <<  "/home/jscherman/CLionProjects/algo3-tp3-cmf/datos/" << nombreArchivo << ".csv";
    std::ofstream a_file (ss.str());

    a_file << "n, m, tiempoTotal" << std::endl;
    int maxM = (constanteN*(constanteN-1))/2;
    int minM = 0;
    std::cout << "Variando m: {n=" << constanteN << "} => " << minM << " <= m <= " << maxM << std::endl;
    for (int i = minM; i <= maxM; ++i) {
        long long tiempoTotal = 0;
        for (int j = 0; j < cantInstanciasPorM; ++j) {
            std::vector<std::list<int>> listaAdyacencias = Utils::generarListaAdyacencias(constanteN, i, false, 0, 0);
            std::vector<std::vector<bool>> matrizAdyacencias = Utils::aMatrizAdyacencias(listaAdyacencias);
            auto tpi = std::chrono::high_resolution_clock::now();
            Clique clique = exactoBTVertices(matrizAdyacencias, listaAdyacencias);
            auto tpf = std::chrono::high_resolution_clock::now();
            auto tiempo = std::chrono::duration_cast<std::chrono::nanoseconds>(tpf-tpi).count();
            tiempoTotal+= tiempo;
        }

        tiempoTotal = tiempoTotal/ cantInstanciasPorM;
        std::cout << constanteN << ", "<<  i << ", " << tiempoTotal << std::endl ;
        a_file << constanteN << ", "<< i << ", " << tiempoTotal << std::endl;
    }

    a_file.close();
    std::cout << "Listo!" << std::endl;
}

void escribirTiemposVariandoNSinEjes(int cantInstanciasPorN, int minN, int maxN){
    std::string nombreArchivo = "tiempos-exacto-variando-n-m0";

    std::stringstream ss;
    ss <<  "/home/jscherman/CLionProjects/algo3-tp3-cmf/datos/" << nombreArchivo << ".csv";
    std::ofstream a_file (ss.str());

    a_file << "n, m, tiempoTotal" << std::endl;

    int m = 0;
    std::cout << "Variando n: {m=" << m << "} => " << minN << " <= n <= " << maxN << std::endl;
    for (int i = minN; i <= maxN; ++i) {

        long long tiempoTotal = 0;
        for (int j = 0; j < cantInstanciasPorN; ++j) {
            std::vector<std::list<int>> listaAdyacencias = Utils::generarListaAdyacencias(i, m, false, 0, 0);
            std::vector<std::vector<bool>> matrizAdyacencias = Utils::aMatrizAdyacencias(listaAdyacencias);
            auto tpi = std::chrono::high_resolution_clock::now();
            Clique clique = exactoBTVertices(matrizAdyacencias, listaAdyacencias);
            auto tpf = std::chrono::high_resolution_clock::now();
            auto tiempo = std::chrono::duration_cast<std::chrono::nanoseconds>(tpf-tpi).count();
            tiempoTotal+= tiempo;
        }

        tiempoTotal = tiempoTotal/ cantInstanciasPorN;
        std::cout << i << ", " << m << ", " <<  tiempoTotal << std::endl ;
        a_file << i <<  ", " << m << ", " << tiempoTotal << std::endl;
    }

    a_file.close();
    std::cout << "Listo!" << std::endl;
}