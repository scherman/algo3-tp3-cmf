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
                grasp(constanteN, i, matrizAdyacencias, 3, 20, 1000);
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

    a_file << "n, m, tiempoTotalHConstructiva, tiempoTotalBLocal, tiempoTotalGraspK5, tiempoTotalGraspK25, tiempoTotalGraspKN" << std::endl;

    int m = minN*(minN-1)/2;
    std::cout << "Variando n: {m=" << m << "} => " << minN << " <= n <= " << maxN << std::endl;
    for (int i = minN; i <= maxN; i+=saltarDeA) {

        long long tiempoTotalHConstructiva = 0;
        long long tiempoTotalBLocal = 0;
        long long tiempoTotalGraspK5 = 0;
        long long tiempoTotalGraspK25 = 0;
        long long tiempoTotalGraspKN = 0;
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
                grasp(i, m, matrizAdyacencias, 5, 20, 1000);
                auto tpf = std::chrono::high_resolution_clock::now();
                auto tiempo = std::chrono::duration_cast<std::chrono::nanoseconds>(tpf-tpi).count();
                tiempoTotalGraspK5+= tiempo;
            }
            {
                // Tomar tiempo grasp
                auto tpi = std::chrono::high_resolution_clock::now();
                grasp(i, m, matrizAdyacencias, 25, 20, 1000);
                auto tpf = std::chrono::high_resolution_clock::now();
                auto tiempo = std::chrono::duration_cast<std::chrono::nanoseconds>(tpf-tpi).count();
                tiempoTotalGraspK25+= tiempo;
            }
            {
                // Tomar tiempo grasp
                auto tpi = std::chrono::high_resolution_clock::now();
                grasp(i, m, matrizAdyacencias, i, 20, 1000);
                auto tpf = std::chrono::high_resolution_clock::now();
                auto tiempo = std::chrono::duration_cast<std::chrono::nanoseconds>(tpf-tpi).count();
                tiempoTotalGraspKN+= tiempo;
            }
        }
        tiempoTotalHConstructiva = tiempoTotalHConstructiva/ cantInstanciasPorN;
        tiempoTotalBLocal = tiempoTotalBLocal / cantInstanciasPorN;
        tiempoTotalGraspK5 = tiempoTotalGraspK5/ cantInstanciasPorN;
        tiempoTotalGraspK25 = tiempoTotalGraspK25 / cantInstanciasPorN;
        tiempoTotalGraspKN = tiempoTotalGraspKN / cantInstanciasPorN;
        std::cout << i << ", "<< m << ", " << tiempoTotalHConstructiva << ", " << tiempoTotalBLocal << ", " << tiempoTotalGraspK5 << ", " << tiempoTotalGraspK25  << ", " << tiempoTotalGraspKN << std::endl ;
        a_file << i << ", "<< m << ", " << tiempoTotalHConstructiva << ", " << tiempoTotalBLocal << ", "  << tiempoTotalGraspK5 << ", " << tiempoTotalGraspK25  << ", " << tiempoTotalGraspKN << std::endl ;
    }

    a_file.close();
    std::cout << "Listo!" << std::endl;
}

void compararFronteras(int cantInstanciasPorM, int constanteN, int saltarDeA){
    std::string nombreArchivo = "frontera-todos-variando-m";

    std::stringstream ss;
    ss <<  "/home/jscherman/CLionProjects/algo3-tp3-cmf/datos/" << nombreArchivo << ".csv";
    std::ofstream a_file (ss.str());

    a_file << "n, m, fronteraHConstructiva, fronteraBLocal, fronteraGraspK5, fronteraGraspK25, fronteraGraspKN" << std::endl;
    int maxM = (constanteN*(constanteN-1))/2;
    int minM = 1; // Para no dividir por 0 en el promedio
    std::cout << "Variando m: {n=" << constanteN << "} => " << minM << " <= m <= " << maxM << std::endl;
    for (int i = minM; i <= maxM; i+=saltarDeA) {
        long long fronteraBLocalTotal = 0;
        long long fronteraHConstructivaTotal = 0;
        long long fronteraGraspK5Total = 0;
        long long fronteraGraspK25Total = 0;
        long long fronteraGraspKNTotal = 0;
        for (int j = 0; j < cantInstanciasPorM; ++j) {
            std::vector<std::list<int>> listaAdyacencias = Utils::generarListaAdyacencias(constanteN, i, false, 0, 0);
            std::vector<std::vector<bool>> matrizAdyacencias = Utils::aMatrizAdyacencias(listaAdyacencias);

            Clique *cliqueConstructiva = hconstructiva(matrizAdyacencias, listaAdyacencias, -1);
            int fronteraCliqueConstrutiva = cliqueConstructiva->frontera;

            Clique busquedaLocal = busquedaLocalExtendiendoClique(*cliqueConstructiva, matrizAdyacencias, listaAdyacencias);

            Clique graspCliqueK5 = grasp(constanteN, i, matrizAdyacencias, 5, 20, 1000);
            Clique graspCliqueKN = grasp(constanteN, i, matrizAdyacencias, constanteN, 20, 1000);
            Clique graspCliqueK25 = grasp(constanteN, i, matrizAdyacencias, 25, 20, 1000);


            fronteraBLocalTotal+= busquedaLocal.frontera;
            fronteraHConstructivaTotal+= fronteraCliqueConstrutiva;
            fronteraGraspK5Total+= graspCliqueK5.frontera;
            fronteraGraspK25Total+= graspCliqueK25.frontera;
            fronteraGraspKNTotal+= graspCliqueKN.frontera;

            delete cliqueConstructiva;
        }

        fronteraBLocalTotal = fronteraBLocalTotal/ cantInstanciasPorM;
        fronteraHConstructivaTotal = fronteraHConstructivaTotal/cantInstanciasPorM;
        fronteraGraspK5Total = fronteraGraspK5Total/cantInstanciasPorM;
        fronteraGraspK25Total = fronteraGraspK25Total/cantInstanciasPorM;
        fronteraGraspKNTotal = fronteraGraspKNTotal/cantInstanciasPorM;
        std::cout << constanteN << ", "<<  i << ", " << fronteraHConstructivaTotal << ", " << fronteraBLocalTotal << ", " << fronteraGraspK5Total << ", "<< fronteraGraspK25Total << ", " << fronteraGraspKNTotal << std::endl;
        a_file << constanteN << ", "<<  i << ", " << fronteraHConstructivaTotal << ", " << fronteraBLocalTotal << ", " << fronteraGraspK5Total << ", "<< fronteraGraspK25Total << ", " << fronteraGraspKNTotal << std::endl;
    }
    a_file.close();
    std::cout << "Listo!" << std::endl;
}

void compararFronterasVariandoN(int cantInstanciasPorN, int minN, int maxN, int saltarDeA){
    std::string nombreArchivo = "frontera-todos-variando-n";

    std::stringstream ss;
    ss <<  "/home/jscherman/CLionProjects/algo3-tp3-cmf/datos/" << nombreArchivo << ".csv";
    std::ofstream a_file (ss.str());

    a_file << "n, m, fronteraHConstructiva, fronteraBLocal, fronteraGraspK5, fronteraGraspK25, fronteraGraspKN" << std::endl;
    int m = minN*(minN-1)/2;
    std::cout << "Variando n: {m=" << m << "} => " << minN << " <= n <= " << maxN << std::endl;
    for (int i = minN; i <= maxN; i+=saltarDeA) {
        long long fronteraBLocalTotal = 0;
        long long fronteraHConstructivaTotal = 0;
        long long fronteraGraspK5Total = 0;
        long long fronteraGraspK25Total = 0;
        long long fronteraGraspKNTotal = 0;
        for (int j = 0; j < cantInstanciasPorN; ++j) {
            std::vector<std::list<int>> listaAdyacencias = Utils::generarListaAdyacencias(i, m, false, 0, 0);
            std::vector<std::vector<bool>> matrizAdyacencias = Utils::aMatrizAdyacencias(listaAdyacencias);

            Clique *cliqueConstructiva = hconstructiva(matrizAdyacencias, listaAdyacencias, -1);
            int fronteraCliqueConstrutiva = cliqueConstructiva->frontera;

            Clique busquedaLocal = busquedaLocalExtendiendoClique(*cliqueConstructiva, matrizAdyacencias, listaAdyacencias);

            Clique graspCliqueK5 = grasp(i, m, matrizAdyacencias, 5, 20, 1000);
            Clique graspCliqueKN = grasp(i, m, matrizAdyacencias, i, 20, 1000);
            Clique graspCliqueK25 = grasp(i, m, matrizAdyacencias, 25, 20, 1000);


            fronteraBLocalTotal+= busquedaLocal.frontera;
            fronteraHConstructivaTotal+= fronteraCliqueConstrutiva;
            fronteraGraspK5Total+= graspCliqueK5.frontera;
            fronteraGraspK25Total+= graspCliqueK25.frontera;
            fronteraGraspKNTotal+= graspCliqueKN.frontera;

            delete cliqueConstructiva;
        }

        fronteraBLocalTotal = fronteraBLocalTotal/ cantInstanciasPorN;
        fronteraHConstructivaTotal = fronteraHConstructivaTotal/cantInstanciasPorN;
        fronteraGraspK5Total = fronteraGraspK5Total/cantInstanciasPorN;
        fronteraGraspK25Total = fronteraGraspK25Total/cantInstanciasPorN;
        fronteraGraspKNTotal = fronteraGraspKNTotal/cantInstanciasPorN;
        std::cout << i<< ", "<<  m << ", " << fronteraHConstructivaTotal << ", " << fronteraBLocalTotal << ", " << fronteraGraspK5Total << ", "<< fronteraGraspK25Total << ", " << fronteraGraspKNTotal << std::endl;
        a_file << i<< ", "<<  m<< ", " << fronteraHConstructivaTotal << ", " << fronteraBLocalTotal << ", " << fronteraGraspK5Total << ", "<< fronteraGraspK25Total << ", " << fronteraGraspKNTotal << std::endl;
    }
    a_file.close();
    std::cout << "Listo!" << std::endl;
}

int main(int argc, char** argv) {
//    compararFronteras(3, 100, 5);
//    compararTiemposVariandoM(3, 100, 1);
    compararFronterasVariandoN(3, 50, 10000, 20);
    return 0;
}
