//
// Created by jscherman on 20/06/17.
//

#include "cmf-heuristica-constructiva-golosa.h"
#include "busqueda-local.h"
#include "meta-heuristica.h"
#include "cmf-algo-exacto.h"
#include "grasp.h"
#include <chrono>
#include <random>
#include <sstream>

void compararTiemposVariandoM(int cantInstanciasPorM, int constanteN, int saltarDeA){

    std::string nombreArchivo = "tiempos-todos-variando-m";

    std::stringstream ss;
    ss <<  "/home/jscherman/CLionProjects/algo3-tp3-cmf/datos/" << nombreArchivo << ".csv";
    std::ofstream a_file (ss.str());

    a_file << "n, m, tiempoTotalHConstructiva, tiempoTotalBLocal, tiempoTotalGrasp" << std::endl;
    int maxM = (constanteN*(constanteN-1))/2;
    int minM = 0;
    std::cout << "Variando m: {n=" << constanteN << "} => " << minM << " <= m <= " << maxM << std::endl;
    for (int i = minM; i <= maxM; i+=saltarDeA) {
        long long tiempoTotalHConstructiva = 0;
        long long tiempoTotalBLocal = 0;
        long long tiempoTotalGrasp = 0;
        for (int j = 0; j < cantInstanciasPorM; ++j) {
            std::vector<std::list<int>> listaAdyacencias = Utils::generarListaAdyacencias(constanteN, i, false, 0, 0);
            std::vector<std::vector<bool>> matrizAdyacencias = Utils::aMatrizAdyacencias(listaAdyacencias);
            {
                // Tomar tiempo heuristica constructiva golosa
                auto tpi = std::chrono::high_resolution_clock::now();
                Clique *clique = hconstructiva(matrizAdyacencias, listaAdyacencias, -1);
                auto tpf = std::chrono::high_resolution_clock::now();
                auto tiempo = std::chrono::duration_cast<std::chrono::nanoseconds>(tpf-tpi).count();
                tiempoTotalHConstructiva+= tiempo;

                delete clique;
            }
            {
                // Tomar tiempo busqueda local
                Clique *clique = hconstructiva(matrizAdyacencias, listaAdyacencias, -1);
                auto tpi = std::chrono::high_resolution_clock::now();
                busquedaLocalExtendiendoClique(*clique, matrizAdyacencias, listaAdyacencias);
                auto tpf = std::chrono::high_resolution_clock::now();
                auto tiempo = std::chrono::duration_cast<std::chrono::nanoseconds>(tpf-tpi).count();
                tiempoTotalBLocal+= tiempo;

                delete clique;
            }
            {
                // Tomar tiempo grasp
                auto tpi = std::chrono::high_resolution_clock::now();
                randomizedgreedy(3, constanteN, i);
                auto tpf = std::chrono::high_resolution_clock::now();
                auto tiempo = std::chrono::duration_cast<std::chrono::nanoseconds>(tpf-tpi).count();
                tiempoTotalGrasp+= tiempo;

            }
        }

        tiempoTotalHConstructiva = tiempoTotalHConstructiva/ cantInstanciasPorM;
        tiempoTotalBLocal = tiempoTotalBLocal / cantInstanciasPorM;
        tiempoTotalGrasp = tiempoTotalGrasp/ cantInstanciasPorM;
        std::cout << constanteN << ", "<<  i << ", " << tiempoTotalHConstructiva << ", " << tiempoTotalBLocal << ", " << tiempoTotalGrasp << std::endl ;
        a_file << constanteN << ", "<<  i << ", " << tiempoTotalHConstructiva << ", " << tiempoTotalBLocal << ", " << tiempoTotalGrasp << std::endl;
    }

    a_file.close();
    std::cout << "Listo!" << std::endl;
}

void compararTiemposVariandoN(int cantInstanciasPorN, int minN, int maxN, int saltarDeA){
    std::string nombreArchivo = "tiempos-todos-variando-n";

    std::stringstream ss;
    ss <<  "/home/jscherman/CLionProjects/algo3-tp3-cmf/datos/" << nombreArchivo << ".csv";
    std::ofstream a_file (ss.str());

    a_file << "n, m, tiempoTotal" << std::endl;

    int m = minN*(minN-1)/2;
    std::cout << "Variando n: {m=" << m << "} => " << minN << " <= n <= " << maxN << std::endl;
    for (int i = minN; i <= maxN; i+=saltarDeA) {

        long long tiempoTotalHConstructiva = 0;
        long long tiempoTotalBLocal = 0;
        long long tiempoTotalGrasp = 0;
        for (int j = 0; j < cantInstanciasPorN; ++j) {
            std::vector<std::list<int>> listaAdyacencias = Utils::generarListaAdyacencias(i, m, false, 0, 0);
            std::vector<std::vector<bool>> matrizAdyacencias = Utils::aMatrizAdyacencias(listaAdyacencias);
            {
                // Tomar tiempo heuristica constructiva golosa
                auto tpi = std::chrono::high_resolution_clock::now();
                Clique *clique = hconstructiva(matrizAdyacencias, listaAdyacencias, -1);
                auto tpf = std::chrono::high_resolution_clock::now();
                auto tiempo = std::chrono::duration_cast<std::chrono::nanoseconds>(tpf-tpi).count();
                tiempoTotalHConstructiva+= tiempo;

                delete clique;
            }
            {
                // Tomar tiempo busqueda local
                Clique *clique = hconstructiva(matrizAdyacencias, listaAdyacencias, -1);
                auto tpi = std::chrono::high_resolution_clock::now();
                busquedaLocalExtendiendoClique(*clique, matrizAdyacencias, listaAdyacencias);
                auto tpf = std::chrono::high_resolution_clock::now();
                auto tiempo = std::chrono::duration_cast<std::chrono::nanoseconds>(tpf-tpi).count();
                tiempoTotalBLocal+= tiempo;

                delete clique;
            }
            {
                // Tomar tiempo grasp
                auto tpi = std::chrono::high_resolution_clock::now();
                randomizedgreedy(3, i, m);
                auto tpf = std::chrono::high_resolution_clock::now();
                auto tiempo = std::chrono::duration_cast<std::chrono::nanoseconds>(tpf-tpi).count();
                tiempoTotalGrasp+= tiempo;
            }
        }
        tiempoTotalHConstructiva = tiempoTotalHConstructiva/ cantInstanciasPorN;
        tiempoTotalBLocal = tiempoTotalBLocal / cantInstanciasPorN;
        tiempoTotalGrasp = tiempoTotalGrasp/ cantInstanciasPorN;
        std::cout << i << ", "<< m << ", " << tiempoTotalHConstructiva << ", " << tiempoTotalBLocal << ", " << tiempoTotalGrasp << std::endl ;
        a_file << i << ", "<< m << ", " << tiempoTotalHConstructiva << ", " << tiempoTotalBLocal << ", " << tiempoTotalGrasp << std::endl;
    }

    a_file.close();
    std::cout << "Listo!" << std::endl;
}

int main(int argc, char** argv) {
    compararTiemposVariandoM(3, 10, 1);
    compararTiemposVariandoN(3, 10, 100, 1);
    return 0;
}
