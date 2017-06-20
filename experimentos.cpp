//
// Created by jscherman on 20/06/17.
//

#include "cmf-heuristica-constructiva-golosa.h"
#include "busqueda-local.h"
#include "cmf-algo-exacto.h"
#include <chrono>
#include <random>
#include <sstream>

int main(int argc, char** argv) {
    int n = 30, m = 300;
    std::list<Eje> grafo = Utils::generarGrafo(n, m, false, 0, 0);
//    std::cout << "Exacto: " << *cmfExacto(n, m, grafo) << std::endl;
    Clique clique = heuristicaConstructiva(n, grafo);
    std::cout << "Constructiva: " <<  clique << std::endl;
    std::cout << "Busqueda local: " << busquedaLocal2(n, clique, grafo) << std::endl;
    return 0;
}

void variarM(int cantInstanciasPorM, int constanteN){
    std::string nombreArchivo = "resultados-exacto-hconstructiva-blocal";

    std::stringstream ss;
    ss <<  "/home/jscherman/CLionProjects/algo3-tp3-cmf/datos/" << nombreArchivo << ".csv";
    std::ofstream a_file (ss.str());

    a_file << "m, tiempoTotal" << std::endl;
    int maxM = (constanteN*(constanteN-1))/2;
    int minM = constanteN;
    int constantePesoMinimo = 0;
    int constantePesoMaximo = 0;
    std::cout << "Variando m: {n=" << constanteN << ", C=" << constantePesoMaximo << "} => " << minM << " <= m <= " << maxM << std::endl;
    for (int i = minM; i <= maxM; ++i) {
        long long tiempoTotal = 0;
        for (int j = 0; j < cantInstanciasPorM; ++j) {
            std::list<Eje> grafo = Utils::generarGrafo(constanteN, i, false, constantePesoMinimo, constantePesoMaximo);

            auto tpi = std::chrono::high_resolution_clock::now();
//            int res = problema_dos(grafo.first, constanteN, grafo.first.size(), grafo.second);
            auto tpf = std::chrono::high_resolution_clock::now();
            auto tiempo = std::chrono::duration_cast<std::chrono::nanoseconds>(tpf-tpi).count();
            tiempoTotal+= tiempo;
        }

        tiempoTotal = tiempoTotal/ cantInstanciasPorM;
        std::cout << i << ", " << tiempoTotal << std::endl ;
        a_file << i << ", " << tiempoTotal << std::endl;
    }

    a_file.close();
    std::cout << "Listo!" << std::endl;
}

