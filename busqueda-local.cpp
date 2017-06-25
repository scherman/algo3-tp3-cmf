//
// Created by jscherman on 20/06/17.
//

#include "busqueda-local.h"
#include <sstream>
#include <chrono>
#include "cmf-algo-exacto.h"
//O(n^3)
Clique& busquedaLocalExtendiendoClique(Clique &clique,
                                       std::vector<std::vector<bool>> &matrizAdyacencias,
                                       std::vector<std::list<int>> &listaIncidencias) {
    int n = listaIncidencias.size();

    bool nodosEnClique[n] = {false};
    for (std::list<int>::iterator it = clique.vertices.begin(); it != clique.vertices.end(); ++it) {
        nodosEnClique[*it] = true;
    }

    int mejoraSolucion = true;
    while (mejoraSolucion) { // O(n)
        mejoraSolucion = false;
        for (int i = 0; i < n; ++i) {
            if (!nodosEnClique[i] && mejoraFrontera(clique, i, listaIncidencias) && extiendeClique(clique, i, matrizAdyacencias, listaIncidencias)) { // O(n^2)
                // Extiendo clique
                clique.frontera = clique.frontera - 2*clique.vertices.size() + listaIncidencias[i].size();
                clique.vertices.push_back(i);
                nodosEnClique[i] = true;
                mejoraSolucion = true;
                break;
            }
        }
    }
    return clique;
}

void porcentajeErrorVariandoMBusquedaLocal(int cantInstanciasPorM, int constanteN, int saltarDeA){
    std::string nombreArchivo = "prom-acierto-blocal-con-heuristica-mayor-grado-variando-m";

    std::stringstream ss;
    ss <<  "/home/jscherman/CLionProjects/algo3-tp3-cmf/datos/" << nombreArchivo << ".csv";
    std::ofstream a_file (ss.str());

    a_file << "n, m, promedioAciertoBLocal, promedioAciertoHConstructiva, promedioMejoraHConstructiva" << std::endl;
    int maxM = (constanteN*(constanteN-1))/2;
    int minM = 1; // Para no dividir por 0 en el promedio
    std::cout << "Variando m: {n=" << constanteN << "} => " << minM << " <= m <= " << maxM << std::endl;
    long long promedioErrorGlobal = 0;
    long long promedioMejoraHConstructivaGlobal = 0;
    long long cantValores = 0;
    for (int i = minM; i <= maxM; i+=saltarDeA) {
        long long promedioAciertoBLocal = 0;
        long long promedioAciertoHConstructiva = 0;
        long long promedioMejoraHConstructiva = 0;
        for (int j = 0; j < cantInstanciasPorM; ++j) {
            std::vector<std::list<int>> listaAdyacencias = Utils::generarListaAdyacencias(constanteN, i, false, 0, 0);
            std::vector<std::vector<bool>> matrizAdyacencias = Utils::aMatrizAdyacencias(listaAdyacencias);

            Clique *cliqueConstructiva = hconstructiva(matrizAdyacencias, listaAdyacencias, -1);
            int fronteraCliqueConstrutiva = cliqueConstructiva->frontera;

            Clique busquedaLocal = busquedaLocalExtendiendoClique(*cliqueConstructiva, matrizAdyacencias, listaAdyacencias);
            promedioMejoraHConstructiva += (busquedaLocal.frontera*100)/fronteraCliqueConstrutiva - 100;

            Clique cliqueExacto = exactoBTVertices(matrizAdyacencias, listaAdyacencias);
            promedioAciertoBLocal+= (busquedaLocal.frontera*100)/(cliqueExacto.frontera);
            promedioAciertoHConstructiva+= (fronteraCliqueConstrutiva*100)/(cliqueExacto.frontera);
            delete cliqueConstructiva;
        }

        promedioAciertoBLocal = promedioAciertoBLocal/ cantInstanciasPorM;
        promedioAciertoHConstructiva = promedioAciertoHConstructiva/cantInstanciasPorM;
        promedioMejoraHConstructiva= promedioMejoraHConstructiva/ cantInstanciasPorM;
        promedioErrorGlobal += promedioAciertoBLocal;
        promedioMejoraHConstructivaGlobal += promedioMejoraHConstructiva;
        cantValores++;
        std::cout << constanteN << ", "<<  i << ", " << promedioAciertoBLocal << ", " << promedioAciertoHConstructiva << ", " << promedioMejoraHConstructiva << std::endl ;
        a_file  << constanteN << ", "<<  i << ", " << promedioAciertoBLocal << ", " << promedioAciertoHConstructiva << ", " << promedioMejoraHConstructiva << std::endl ;
    }
    promedioErrorGlobal = promedioErrorGlobal / cantValores;
    promedioMejoraHConstructivaGlobal= promedioMejoraHConstructivaGlobal / cantValores;

    a_file.close();
    std::cout << "Listo! El promedio de error global es: " <<  promedioErrorGlobal << "%. El promedio de mejora de la busqueda local con respecto a la heurístrica constructiva (mayor grado) es " << promedioMejoraHConstructivaGlobal << std::endl;
}

void escribirTiemposVariandoNBLocal(int cantInstanciasPorN, int minN, int maxN, int saltarDeA){
    std::string nombreArchivo = "tiempos-blocal-mayor-grado-variando-n";

    std::stringstream ss;
    ss <<  "/home/jscherman/CLionProjects/algo3-tp3-cmf/datos/" << nombreArchivo << ".csv";
    std::ofstream a_file (ss.str());

    a_file << "n, m, tiempoTotal" << std::endl;

    int m = minN*(minN-1)/2;
    std::cout << "Variando n: {m=" << m << "} => " << minN << " <= n <= " << maxN << std::endl;
    for (int i = minN; i <= maxN; i+=saltarDeA) {

        long long tiempoTotal = 0;
        for (int j = 0; j < cantInstanciasPorN; ++j) {
            std::vector<std::list<int>> listaAdyacencias = Utils::generarListaAdyacencias(i, m, false, 0, 0);
            std::vector<std::vector<bool>> matrizAdyacencias = Utils::aMatrizAdyacencias(listaAdyacencias);
            Clique *clique = hconstructiva(matrizAdyacencias, listaAdyacencias, -1);
            auto tpi = std::chrono::high_resolution_clock::now();
            busquedaLocalExtendiendoClique(*clique, matrizAdyacencias, listaAdyacencias);
            auto tpf = std::chrono::high_resolution_clock::now();
            auto tiempo = std::chrono::duration_cast<std::chrono::nanoseconds>(tpf-tpi).count();
            tiempoTotal+= tiempo;
            delete clique;
        }

        tiempoTotal = tiempoTotal/ cantInstanciasPorN;
        std::cout << i << ", " << m << ", " <<  tiempoTotal << std::endl ;
        a_file << i <<  ", " << m << ", " << tiempoTotal << std::endl;
    }

    a_file.close();
    std::cout << "Listo!" << std::endl;
}

void escribirTiemposVariandoMBLocal(int cantInstanciasPorM, int constanteN, int saltarDeA){
    std::string nombreArchivo = "tiempos-blocal-mayor-grado-variando-m";

    std::stringstream ss;
    ss <<  "/home/jscherman/CLionProjects/algo3-tp3-cmf/datos/" << nombreArchivo << ".csv";
    std::ofstream a_file (ss.str());

    a_file << "n, m, tiempoTotal" << std::endl;
    int maxM = (constanteN*(constanteN-1))/2;
    int minM = 0;
    std::cout << "Variando m: {n=" << constanteN << "} => " << minM << " <= m <= " << maxM << std::endl;
    for (int i = minM; i <= maxM; i+=saltarDeA) {
        long long tiempoTotal = 0;
        for (int j = 0; j < cantInstanciasPorM; ++j) {
            std::vector<std::list<int>> listaAdyacencias = Utils::generarListaAdyacencias(constanteN, i, false, 0, 0);
            std::vector<std::vector<bool>> matrizAdyacencias = Utils::aMatrizAdyacencias(listaAdyacencias);
            Clique *clique = hconstructiva(matrizAdyacencias, listaAdyacencias, -1);
            auto tpi = std::chrono::high_resolution_clock::now();
            busquedaLocalExtendiendoClique(*clique, matrizAdyacencias, listaAdyacencias);
            auto tpf = std::chrono::high_resolution_clock::now();
            auto tiempo = std::chrono::duration_cast<std::chrono::nanoseconds>(tpf-tpi).count();
            tiempoTotal+= tiempo;
            delete clique;
        }

        tiempoTotal = tiempoTotal/ cantInstanciasPorM;
        std::cout << constanteN << ", "<<  i << ", " << tiempoTotal << std::endl ;
        a_file << constanteN << ", "<< i << ", " << tiempoTotal << std::endl;
    }

    a_file.close();
    std::cout << "Listo!" << std::endl;
}
