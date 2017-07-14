//
// Created by jscherman on 20/06/17.
//

#include "cmf-heuristica-constructiva-golosa.h"
#include "busqueda-local.h"
#include "meta-heuristica.h"
#include "cmf-algo-exacto.h"
#include "graspv2.h"
#include <chrono>
#include <random>
#include <sstream>

void compararAlgoritmosConExactoVariandoM(int cantInstanciasPorM, int constanteN, int saltarDeA) {
    std::string nombreArchivo = "comp-algoritmos-con-exacto-variando-m";

    std::stringstream ss;
    ss <<  "/home/jscherman/CLionProjects/algo3-tp3-cmf/datos/" << nombreArchivo << ".csv";
    std::ofstream a_file (ss.str());

    a_file << "n, m, fronteraExacto, fronteraHConstructiva, fronteraBLocal, fronteraGrasp, tiempoExacto, tiempoHConstructiva, tiempoBLocal, tiempoGrasp" << std::endl;
    int maxM = (constanteN*(constanteN-1))/2;
    int minM = 1;
    std::cout << "Variando m: {n=" << constanteN << "} => " << minM << " <= m <= " << maxM << std::endl;
    for (int i = minM; i <= maxM; i+=saltarDeA) {
        long long tiempoTotalExacto = 0;
        long long tiempoTotalHConstructiva = 0;
        long long tiempoTotalBLocal = 0;
        long long tiempoTotalGrasp = 0;
        long long promFronteraExacto= 0;
        long long promFronteraHConstructiva= 0;
        long long promFronteraBLocal= 0;
        long long promFronteraGrasp= 0;
        for (int j = 0; j < cantInstanciasPorM; ++j) {
            std::vector<std::list<int>> listaAdyacencias = Utils::generarListaAdyacencias(constanteN, i, false, 0, 0);
            std::vector<std::vector<bool>> matrizAdyacencias = Utils::aMatrizAdyacencias(listaAdyacencias);

            {
                // Exacto
                auto tpi = std::chrono::high_resolution_clock::now();
                Clique clique = exactoBTVertices(matrizAdyacencias, listaAdyacencias);
                auto tpf = std::chrono::high_resolution_clock::now();
                auto tiempo = std::chrono::duration_cast<std::chrono::nanoseconds>(tpf-tpi).count();
                tiempoTotalExacto += tiempo;
                promFronteraExacto += clique.frontera;
            }
            {
                // H. Constructiva
                auto tpi = std::chrono::high_resolution_clock::now();
                Clique *clique = hconstructiva(matrizAdyacencias, listaAdyacencias, -1);
                auto tpf = std::chrono::high_resolution_clock::now();
                auto tiempo = std::chrono::duration_cast<std::chrono::nanoseconds>(tpf-tpi).count();
                tiempoTotalHConstructiva += tiempo;
                promFronteraHConstructiva += clique->frontera;
                delete clique;
            }
            {
                // B. Local
                Clique *clique = hconstructiva(matrizAdyacencias, listaAdyacencias, -1);
                auto tpi = std::chrono::high_resolution_clock::now();
                Clique cliqueMejorada = busquedaLocalExtendiendoClique(*clique, matrizAdyacencias, listaAdyacencias);
                auto tpf = std::chrono::high_resolution_clock::now();
                auto tiempo = std::chrono::duration_cast<std::chrono::nanoseconds>(tpf-tpi).count();
                tiempoTotalBLocal += tiempo;
                promFronteraBLocal += cliqueMejorada.frontera;
                delete clique;
            }
            {
                // Grasp
                int RCL=3, iterations=20;
                auto tpi = std::chrono::high_resolution_clock::now();
                Clique clique = grasp2(matrizAdyacencias, listaAdyacencias, RCL, iterations);
                auto tpf = std::chrono::high_resolution_clock::now();
                auto tiempo = std::chrono::duration_cast<std::chrono::nanoseconds>(tpf-tpi).count();
                tiempoTotalGrasp += tiempo;
                promFronteraGrasp += clique.frontera;
            }
        }

        tiempoTotalExacto = tiempoTotalExacto/ cantInstanciasPorM;
        tiempoTotalHConstructiva = tiempoTotalHConstructiva/ cantInstanciasPorM;
        tiempoTotalBLocal = tiempoTotalBLocal / cantInstanciasPorM;
        tiempoTotalGrasp = tiempoTotalGrasp/ cantInstanciasPorM;
        promFronteraExacto = promFronteraExacto/ cantInstanciasPorM;
        promFronteraHConstructiva = promFronteraHConstructiva/ cantInstanciasPorM;
        promFronteraBLocal = promFronteraBLocal / cantInstanciasPorM;
        promFronteraGrasp = promFronteraGrasp/ cantInstanciasPorM;
        std::cout << constanteN << ", "<<  i << ", " << promFronteraExacto << ", " << promFronteraHConstructiva << ", " << promFronteraBLocal << ", " << promFronteraGrasp << ", " << tiempoTotalExacto << ", " << tiempoTotalHConstructiva << ", " << tiempoTotalBLocal << ", " << tiempoTotalGrasp << std::endl ;
        a_file << constanteN << ", "<<  i << ", " << promFronteraExacto << ", " << promFronteraHConstructiva << ", " << promFronteraBLocal << ", " << promFronteraGrasp << ", " << tiempoTotalExacto << ", " << tiempoTotalHConstructiva << ", " << tiempoTotalBLocal << ", " << tiempoTotalGrasp << std::endl ;
    }

    a_file.close();
    std::cout << "Listo!" << std::endl;
}

void compararAlgoritmosSinExactoVariandoM(int cantInstanciasPorM, int constanteN, int saltarDeA) {
    std::string nombreArchivo = "comp-algoritmos-sin-exacto-variando-m";

    std::stringstream ss;
    ss <<  "/home/jscherman/CLionProjects/algo3-tp3-cmf/datos/" << nombreArchivo << ".csv";
    std::ofstream a_file (ss.str());

    a_file << "n, m, fronteraHConstructiva, fronteraBLocal, fronteraGrasp, tiempoHConstructiva, tiempoBLocal, tiempoGrasp" << std::endl;
    int maxM = (constanteN*(constanteN-1))/2;
    int minM = 1;
    std::cout << "Variando m: {n=" << constanteN << "} => " << minM << " <= m <= " << maxM << std::endl;
    for (int i = minM; i <= maxM; i+=saltarDeA) {
        long long tiempoTotalHConstructiva = 0;
        long long tiempoTotalBLocal = 0;
        long long tiempoTotalGrasp = 0;
        long long promFronteraHConstructiva= 0;
        long long promFronteraBLocal= 0;
        long long promFronteraGrasp= 0;
        for (int j = 0; j < cantInstanciasPorM; ++j) {
            std::vector<std::list<int>> listaAdyacencias = Utils::generarListaAdyacencias(constanteN, i, false, 0, 0);
            std::vector<std::vector<bool>> matrizAdyacencias = Utils::aMatrizAdyacencias(listaAdyacencias);
            {
                // H. Constructiva
                auto tpi = std::chrono::high_resolution_clock::now();
                Clique *clique = hconstructiva(matrizAdyacencias, listaAdyacencias, -1);
                auto tpf = std::chrono::high_resolution_clock::now();
                auto tiempo = std::chrono::duration_cast<std::chrono::nanoseconds>(tpf-tpi).count();
                tiempoTotalHConstructiva += tiempo;
                promFronteraHConstructiva += clique->frontera;
                delete clique;
            }
            {
                // B. Local
                Clique *clique = hconstructiva(matrizAdyacencias, listaAdyacencias, -1);
                auto tpi = std::chrono::high_resolution_clock::now();
                Clique cliqueMejorada = busquedaLocalExtendiendoClique(*clique, matrizAdyacencias, listaAdyacencias);
                auto tpf = std::chrono::high_resolution_clock::now();
                auto tiempo = std::chrono::duration_cast<std::chrono::nanoseconds>(tpf-tpi).count();
                tiempoTotalBLocal += tiempo;
                promFronteraBLocal += cliqueMejorada.frontera;
                delete clique;
            }
            {
                // Grasp
                int RCL=3, iterations=20;
                auto tpi = std::chrono::high_resolution_clock::now();
                Clique clique = grasp2(matrizAdyacencias, listaAdyacencias, RCL, iterations);
                auto tpf = std::chrono::high_resolution_clock::now();
                auto tiempo = std::chrono::duration_cast<std::chrono::nanoseconds>(tpf-tpi).count();
                tiempoTotalGrasp += tiempo;
                promFronteraGrasp += clique.frontera;
            }
        }

        tiempoTotalHConstructiva = tiempoTotalHConstructiva/ cantInstanciasPorM;
        tiempoTotalBLocal = tiempoTotalBLocal / cantInstanciasPorM;
        tiempoTotalGrasp = tiempoTotalGrasp/ cantInstanciasPorM;
        promFronteraHConstructiva = promFronteraHConstructiva/ cantInstanciasPorM;
        promFronteraBLocal = promFronteraBLocal / cantInstanciasPorM;
        promFronteraGrasp = promFronteraGrasp/ cantInstanciasPorM;
        std::cout << constanteN << ", "<<  i << ", " << promFronteraHConstructiva << ", " << promFronteraBLocal << ", " << promFronteraGrasp << ", " << tiempoTotalHConstructiva << ", " << tiempoTotalBLocal << ", " << tiempoTotalGrasp << std::endl ;
        a_file << constanteN << ", "<<  i << ", " << promFronteraHConstructiva << ", " << promFronteraBLocal << ", " << promFronteraGrasp << ", " << tiempoTotalHConstructiva << ", " << tiempoTotalBLocal << ", " << tiempoTotalGrasp << std::endl ;
    }

    a_file.close();
    std::cout << "Listo!" << std::endl;
}

void compararAlgoritmosSinExactoVariandoNMCompleto(int cantInstanciasPorM, int minN, int maxN, int saltarDeA) {
    std::string nombreArchivo = "comp-algoritmos-sin-exacto-variando-n-mcompleto";

    std::stringstream ss;
    ss <<  "/home/jscherman/CLionProjects/algo3-tp3-cmf/datos/" << nombreArchivo << ".csv";
    std::ofstream a_file (ss.str());

    a_file << "n, m, fronteraHConstructiva, fronteraBLocal, fronteraGrasp, tiempoHConstructiva, tiempoBLocal, tiempoGrasp" << std::endl;
//    std::cout << "Variando m: {n=" << constanteN << "} => " << minM << " <= m <= " << maxM << std::endl;
    for (int i = minN; i <= maxN; i+=saltarDeA) {
        int m = (i*(i-1))/2;
        long long tiempoTotalHConstructiva = 0;
        long long tiempoTotalBLocal = 0;
        long long tiempoTotalGrasp = 0;
        long long promFronteraHConstructiva= 0;
        long long promFronteraBLocal= 0;
        long long promFronteraGrasp= 0;
        for (int j = 0; j < cantInstanciasPorM; ++j) {
            std::vector<std::list<int>> listaAdyacencias = Utils::generarListaAdyacencias(i, m, false, 0, 0);
            std::vector<std::vector<bool>> matrizAdyacencias = Utils::aMatrizAdyacencias(listaAdyacencias);
            {
                // H. Constructiva
                auto tpi = std::chrono::high_resolution_clock::now();
                Clique *clique = hconstructiva(matrizAdyacencias, listaAdyacencias, -1);
                auto tpf = std::chrono::high_resolution_clock::now();
                auto tiempo = std::chrono::duration_cast<std::chrono::nanoseconds>(tpf-tpi).count();
                tiempoTotalHConstructiva += tiempo;
                promFronteraHConstructiva += clique->frontera;
                delete clique;
            }
            {
                // B. Local
                Clique *clique = hconstructiva(matrizAdyacencias, listaAdyacencias, -1);
                auto tpi = std::chrono::high_resolution_clock::now();
                Clique cliqueMejorada = busquedaLocalExtendiendoClique(*clique, matrizAdyacencias, listaAdyacencias);
                auto tpf = std::chrono::high_resolution_clock::now();
                auto tiempo = std::chrono::duration_cast<std::chrono::nanoseconds>(tpf-tpi).count();
                tiempoTotalBLocal += tiempo;
                promFronteraBLocal += cliqueMejorada.frontera;
                delete clique;
            }
            {
                // Grasp
                int RCL=3, iterations=20;
                auto tpi = std::chrono::high_resolution_clock::now();
                Clique clique = grasp2(matrizAdyacencias, listaAdyacencias, RCL, iterations);
                auto tpf = std::chrono::high_resolution_clock::now();
                auto tiempo = std::chrono::duration_cast<std::chrono::nanoseconds>(tpf-tpi).count();
                tiempoTotalGrasp += tiempo;
                promFronteraGrasp += clique.frontera;
            }
        }

        tiempoTotalHConstructiva = tiempoTotalHConstructiva/ cantInstanciasPorM;
        tiempoTotalBLocal = tiempoTotalBLocal / cantInstanciasPorM;
        tiempoTotalGrasp = tiempoTotalGrasp/ cantInstanciasPorM;
        promFronteraHConstructiva = promFronteraHConstructiva/ cantInstanciasPorM;
        promFronteraBLocal = promFronteraBLocal / cantInstanciasPorM;
        promFronteraGrasp = promFronteraGrasp/ cantInstanciasPorM;
        std::cout << i << ", "<<  m << ", " << promFronteraHConstructiva << ", " << promFronteraBLocal << ", " << promFronteraGrasp << ", " << tiempoTotalHConstructiva << ", " << tiempoTotalBLocal << ", " << tiempoTotalGrasp << std::endl ;
        a_file << i << ", "<<  m << ", " << promFronteraHConstructiva << ", " << promFronteraBLocal << ", " << promFronteraGrasp << ", " << tiempoTotalHConstructiva << ", " << tiempoTotalBLocal << ", " << tiempoTotalGrasp << std::endl ;
    }

    a_file.close();
    std::cout << "Listo!" << std::endl;
}

void compararAlgoritmosSinExactoVariandoKCasoMaloBLocalHConstructiva(int cantInstanciasPorM, int minK, int maxK, int saltarDeA) {
    std::string nombreArchivo = "comp-algoritmos-sin-exacto-variando-k-caso-malo-blocal-hconstructiva";

    std::stringstream ss;
    ss <<  "/home/jscherman/CLionProjects/algo3-tp3-cmf/datos/" << nombreArchivo << ".csv";
    std::ofstream a_file (ss.str());

    a_file << "k, fronteraExacto, fronteraHConstructiva, fronteraBLocal, fronteraGrasp, tiempoHConstructiva, tiempoBLocal, tiempoGrasp" << std::endl;
//    std::cout << "Variando m: {n=" << constanteN << "} => " << minM << " <= m <= " << maxM << std::endl;
    for (int i = minK; i <= maxK; i+=saltarDeA) {
        long long tiempoTotalHConstructiva = 0;
        long long tiempoTotalBLocal = 0;
        long long tiempoTotalGrasp = 0;
        long long promFronteraHConstructiva= 0;
        long long promFronteraBLocal= 0;
        long long promFronteraGrasp= 0;
        for (int j = 0; j < cantInstanciasPorM; ++j) {
            std::vector<std::list<int>> listaAdyacencias = Utils::genCasoMaloBLocal(i);
            std::vector<std::vector<bool>> matrizAdyacencias = Utils::aMatrizAdyacencias(listaAdyacencias);
            {
                // H. Constructiva
                auto tpi = std::chrono::high_resolution_clock::now();
                Clique *clique = hconstructiva(matrizAdyacencias, listaAdyacencias, -1);
                auto tpf = std::chrono::high_resolution_clock::now();
                auto tiempo = std::chrono::duration_cast<std::chrono::nanoseconds>(tpf-tpi).count();
                tiempoTotalHConstructiva += tiempo;
                promFronteraHConstructiva += clique->frontera;
                delete clique;
            }
            {
                // B. Local
                Clique *clique = hconstructiva(matrizAdyacencias, listaAdyacencias, -1);
                auto tpi = std::chrono::high_resolution_clock::now();
                Clique cliqueMejorada = busquedaLocalExtendiendoClique(*clique, matrizAdyacencias, listaAdyacencias);
                auto tpf = std::chrono::high_resolution_clock::now();
                auto tiempo = std::chrono::duration_cast<std::chrono::nanoseconds>(tpf-tpi).count();
                tiempoTotalBLocal += tiempo;
                promFronteraBLocal += cliqueMejorada.frontera;
                delete clique;
            }
            {
                // Grasp
                int RCL=3, iterations=20;
                auto tpi = std::chrono::high_resolution_clock::now();
                Clique clique = grasp2(matrizAdyacencias, listaAdyacencias, RCL, iterations);
                auto tpf = std::chrono::high_resolution_clock::now();
                auto tiempo = std::chrono::duration_cast<std::chrono::nanoseconds>(tpf-tpi).count();
                tiempoTotalGrasp += tiempo;
                promFronteraGrasp += clique.frontera;
            }
        }

        tiempoTotalHConstructiva = tiempoTotalHConstructiva/ cantInstanciasPorM;
        tiempoTotalBLocal = tiempoTotalBLocal / cantInstanciasPorM;
        tiempoTotalGrasp = tiempoTotalGrasp/ cantInstanciasPorM;
        promFronteraHConstructiva = promFronteraHConstructiva/ cantInstanciasPorM;
        promFronteraBLocal = promFronteraBLocal / cantInstanciasPorM;
        promFronteraGrasp = promFronteraGrasp/ cantInstanciasPorM;
        std::cout << i <<  ", " << 2*(i-1) << ", " << promFronteraHConstructiva << ", " << promFronteraBLocal << ", " << promFronteraGrasp << ", " << tiempoTotalHConstructiva << ", " << tiempoTotalBLocal << ", " << tiempoTotalGrasp << std::endl ;
        a_file << i << ", " << 2*(i-1) << ", " << promFronteraHConstructiva << ", " << promFronteraBLocal << ", " << promFronteraGrasp << ", " << tiempoTotalHConstructiva << ", " << tiempoTotalBLocal << ", " << tiempoTotalGrasp << std::endl ;
    }
    a_file.close();
    std::cout << "Listo!" << std::endl;
}

void compararAlgoritmosSinExactoVariandoKCasoMaloTodos(int cantInstanciasPorM, int minK, int maxK, int saltarDeA) {
    std::string nombreArchivo = "comp-algoritmos-sin-exacto-variando-k-caso-malo-todos";

    std::stringstream ss;
    ss <<  "/home/jscherman/CLionProjects/algo3-tp3-cmf/datos/" << nombreArchivo << ".csv";
    std::ofstream a_file (ss.str());

    a_file << "k, fronteraExacto, fronteraHConstructiva, fronteraBLocal, fronteraGrasp, tiempoHConstructiva, tiempoBLocal, tiempoGrasp" << std::endl;
//    std::cout << "Variando m: {n=" << constanteN << "} => " << minM << " <= m <= " << maxM << std::endl;
    for (int i = minK; i <= maxK; i+=saltarDeA) {
        long long tiempoTotalHConstructiva = 0;
        long long tiempoTotalBLocal = 0;
        long long tiempoTotalGrasp = 0;
        long long promFronteraHConstructiva= 0;
        long long promFronteraBLocal= 0;
        long long promFronteraGrasp= 0;
        for (int j = 0; j < cantInstanciasPorM; ++j) {
            std::vector<std::list<int>> listaAdyacencias = Utils::genCasoMaloGrasp(i,4);
            std::vector<std::vector<bool>> matrizAdyacencias = Utils::aMatrizAdyacencias(listaAdyacencias);
            {
                // H. Constructiva
                auto tpi = std::chrono::high_resolution_clock::now();
                Clique *clique = hconstructiva(matrizAdyacencias, listaAdyacencias, -1);
                auto tpf = std::chrono::high_resolution_clock::now();
                auto tiempo = std::chrono::duration_cast<std::chrono::nanoseconds>(tpf-tpi).count();
                tiempoTotalHConstructiva += tiempo;
                promFronteraHConstructiva += clique->frontera;
                delete clique;
            }
            {
                // B. Local
                Clique *clique = hconstructiva(matrizAdyacencias, listaAdyacencias, -1);
                auto tpi = std::chrono::high_resolution_clock::now();
                Clique cliqueMejorada = busquedaLocalExtendiendoClique(*clique, matrizAdyacencias, listaAdyacencias);
                auto tpf = std::chrono::high_resolution_clock::now();
                auto tiempo = std::chrono::duration_cast<std::chrono::nanoseconds>(tpf-tpi).count();
                tiempoTotalBLocal += tiempo;
                promFronteraBLocal += cliqueMejorada.frontera;
                delete clique;
            }
            {
                // Grasp
                int RCL=3, iterations=20;
                auto tpi = std::chrono::high_resolution_clock::now();
                Clique clique = grasp2(matrizAdyacencias, listaAdyacencias, RCL, iterations);
                auto tpf = std::chrono::high_resolution_clock::now();
                auto tiempo = std::chrono::duration_cast<std::chrono::nanoseconds>(tpf-tpi).count();
                tiempoTotalGrasp += tiempo;
                promFronteraGrasp += clique.frontera;
            }
        }

        tiempoTotalHConstructiva = tiempoTotalHConstructiva/ cantInstanciasPorM;
        tiempoTotalBLocal = tiempoTotalBLocal / cantInstanciasPorM;
        tiempoTotalGrasp = tiempoTotalGrasp/ cantInstanciasPorM;
        promFronteraHConstructiva = promFronteraHConstructiva/ cantInstanciasPorM;
        promFronteraBLocal = promFronteraBLocal / cantInstanciasPorM;
        promFronteraGrasp = promFronteraGrasp/ cantInstanciasPorM;
        std::cout << i <<  ", " << 2*(i-1) << ", " << promFronteraHConstructiva << ", " << promFronteraBLocal << ", " << promFronteraGrasp << ", " << tiempoTotalHConstructiva << ", " << tiempoTotalBLocal << ", " << tiempoTotalGrasp << std::endl ;
        a_file << i << ", " << 2*(i-1) << ", " << promFronteraHConstructiva << ", " << promFronteraBLocal << ", " << promFronteraGrasp << ", " << tiempoTotalHConstructiva << ", " << tiempoTotalBLocal << ", " << tiempoTotalGrasp << std::endl ;
    }
    a_file.close();
    std::cout << "Listo!" << std::endl;
}

int main(int argc, char** argv) {
//    compararAlgoritmosConExactoVariandoM(3, 25, 1);
//    compararAlgoritmosSinExactoVariandoM(3, 200, 20);
//    compararAlgoritmosSinExactoVariandoNMCompleto(3, 3, 1000, 5);
    compararAlgoritmosSinExactoVariandoKCasoMaloBLocalHConstructiva(3, 5, 2500, 20);
//    compararAlgoritmosSinExactoVariandoKCasoMaloTodos(3, 5, 1500, 20);
    return 0;
}
