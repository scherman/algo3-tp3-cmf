//
// Created by jscherman on 20/06/17.
//

#include "cmf-heuristica-constructiva-golosa.h"
#include "cmf-algo-exacto.h"
#include <chrono>
#include <random>


int main(int argc, char** argv) {
    int n = 7, m = 12;
    std::list<Eje> grafo = Utils::generarGrafo(n, m, false, 0, 0);
    std::cout << "Exacto: " << *cmfExacto(n, m, grafo) << std::endl;
    std::cout << "Constructiva: " << heuristicaConstructiva(n, grafo) << std::endl;
    return 0;
}

//void escribirComparacionResultados(int cantInstanciasPorN, int minN, int maxN){
//    std::string nombreArchivo = "resultados-exacto-hconstructiva";
//
//    std::stringstream ss;
//    ss <<  "/home/jscherman/CLionProjects/algo3-tp3-cmf/datos/" << nombreArchivo << ".csv";
//    std::ofstream a_file (ss.str());
//
//    a_file << "n, tiempoTotal" << std::endl;
//
//    int m = minN*(minN-1)/2;
//    std::cout << "Variando n: {m=" << m << ", C=" << constantePesoMax << "} => " << minN << " <= n <= " << maxN << std::endl;
//    for (int i = minN; i <= maxN; ++i) {
//
//        long long tiempoTotal = 0;
//        for (int j = 0; j < cantInstanciasPorN; ++j) {
//            std::pair<std::list<Eje> , int> grafo = generarGrafo(i, m, false, constantePesoMax);
//            auto tpi = std::chrono::high_resolution_clock::now();
//            int res = problema_dos(grafo.first, i, grafo.first.size(), grafo.second);
//            auto tpf = std::chrono::high_resolution_clock::now();
//            auto tiempo = std::chrono::duration_cast<std::chrono::nanoseconds>(tpf-tpi).count();
//            tiempoTotal+= tiempo;
//        }
//
//        tiempoTotal = tiempoTotal/ cantInstanciasPorN;
//        std::cout << i << ", " << tiempoTotal << std::endl ;
//        a_file << i <<  ", " << tiempoTotal << std::endl;
//    }
//
//    a_file.close();
//    std::cout << "Listo!" << std::endl;
//}
