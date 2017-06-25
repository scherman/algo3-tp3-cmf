//
// Created by jscherman on 15/06/17.
//

#include "cmf-heuristica-constructiva-golosa.h"
#include "busqueda-local.h"
#include <sstream>
#include <chrono>
#include "cmf-algo-exacto.h"


// O(1)
bool mejoraFrontera(Clique &clique, int vecino, std::vector<std::list<int>> &listaAdyacencias) {
    int gradoVecino = listaAdyacencias[vecino].size();
    return (clique.frontera - 2 * clique.vertices.size() + gradoVecino) > clique.frontera;
}

// O(n)
bool extiendeClique(Clique &clique, int vecino, std::vector<std::vector<bool>> &matrizAdyacencias, std::vector<std::list<int>> &listaAdyacencias) {
    std::list<int> &adyacentesVecino = listaAdyacencias[vecino];
    std::list<int> &verticesClique = clique.vertices;

    for (std::list<int>::const_iterator itVClique = verticesClique.begin();
         itVClique != verticesClique.end(); ++itVClique) { // O(n)
        if (!matrizAdyacencias[vecino][*itVClique]) return false;
    }
    return true;
}

// O(n)
void extenderClique(Clique &clique, int vecino, std::vector<std::vector<bool>> &matrizAdyacencias, std::vector<std::list<int>> & listaAdyacencias) {
    for (std::list<int>::const_iterator itVClique = clique.vertices.begin();
         itVClique != clique.vertices.end(); ++itVClique) {
        int verticeClique = *itVClique;
        matrizAdyacencias[verticeClique][vecino] = true;
        matrizAdyacencias[vecino][verticeClique] = true;
    }
    clique.frontera = clique.frontera - 2 * clique.vertices.size() + listaAdyacencias[vecino].size();
    clique.vertices.push_back(vecino);
}

/*
 * 1. Empezar del nodo con mayor grado
 * 2. Moverse al adyacente con mayor grado.
 *      a. Si extiende la clique del anterior y aumenta la frontera, agregarlo
 *      b. Sino, asignarle una clique nueva y agregarla a la lista de cliques.
 * 3. Repetir 2 hasta que no haya adyacentes
 * 4. Devolver la clique con mayor frontera de la lista de cliques
 */
// O(nm)
Clique* hconstructiva(std::vector<std::vector<bool>> &matrizAdyacencias,
                      std::vector<std::list<int>> &listaAdyacencias,
                      int nodoInicial) {
    int n = listaAdyacencias.size();

    // Busco nodo de mayor grado
    int actual = 0;
    if (nodoInicial == -1) {
        for (int i = 0; i < n; ++i) {
            actual = listaAdyacencias[i].size() > listaAdyacencias[actual].size() ? i : actual;
        }
    } else {
        actual = nodoInicial;
    }

    Clique *cliques[n];
    for(int i = 0; i < n; i++) cliques[i] = nullptr;
    std::list<int> V;
    V.push_back(actual);
    cliques[actual] = new Clique(V, listaAdyacencias[actual].size());

    while (actual != -1) { // O(n^2) (si es completo)
        std::list<int> &adyacentes = listaAdyacencias[actual];

        // Obtener vecinoDisponible de mayor grado
        int vecinoDisponible = -1;
        for (std::list<int>::const_iterator it = adyacentes.begin(); it != adyacentes.end(); ++it) { // O(n)
//            if (matrizAdyacencias[*it][actual]) continue;
            if (cliques[*it] != nullptr) continue;
            if (vecinoDisponible == -1 || listaAdyacencias[vecinoDisponible].size() < listaAdyacencias[*it].size()) {
                vecinoDisponible = *it;
            }
        }

        if (vecinoDisponible != -1) {
            // Puedo agregar otro nodo
            Clique *cliqueActual = cliques[actual];

            if (mejoraFrontera(*cliqueActual, vecinoDisponible, listaAdyacencias) // O(1)
                && extiendeClique(*cliqueActual, vecinoDisponible, matrizAdyacencias, listaAdyacencias)) {  // O(n^2)
                // vecinoActual esta en la clique de actual y aumenta la frontera. Lo agrego a la clique
                extenderClique(*cliqueActual, vecinoDisponible, matrizAdyacencias, listaAdyacencias); // O(n)
                cliques[vecinoDisponible] = cliqueActual;
            } else {
                // Le asigno una clique nueva a vecinoActual
                std::list<int> V;
                V.push_back(vecinoDisponible);
                Clique *cliqueNueva = new Clique(V, listaAdyacencias[vecinoDisponible].size());
                cliques[vecinoDisponible] = cliqueNueva;
            }
        }
        actual = vecinoDisponible;
    }

    // Busco entre las cliques encontradas la que tenga mayor frontera y borro las otras
    Clique *maxClique = nullptr;
    for (int j = 0; j < n; ++j) {
        Clique *cliqueActual = cliques[j];
        if (cliqueActual == nullptr) continue;
//        if (cliques[j] == nullptr){
//            continue;
//        }  else {
//            std::cout << "v=" << j << ":" << *cliques[j] << std::endl;
//        }
        if (maxClique == nullptr) maxClique = cliqueActual;

        if (maxClique->frontera < cliqueActual->frontera) {
            for (std::list<int>::iterator it = maxClique->vertices.begin(); it != maxClique->vertices.end(); ++it) {
                cliques[*it] = nullptr;
            }
            delete maxClique;

            maxClique = cliqueActual;
        } else {
            if (maxClique != cliqueActual) {
                for (std::list<int>::iterator it = cliqueActual->vertices.begin();
                     it != cliqueActual->vertices.end(); ++it) {
                    cliques[*it] = nullptr;
                }
                delete cliqueActual;
            }
        }
    }

    return maxClique;
}

void escribirTiemposVariandoN(int cantInstanciasPorN, int minN, int maxN, int saltarDeA){
    std::string nombreArchivo = "tiempos-hconstructiva-mayor-grado-variando-n";

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
            auto tpi = std::chrono::high_resolution_clock::now();
            Clique *clique = hconstructiva(matrizAdyacencias, listaAdyacencias, -1);
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

void escribirTiemposVariandoM(int cantInstanciasPorM, int constanteN, int saltarDeA){
    std::string nombreArchivo = "tiempos-hconstructiva-mayor-grado-variando-m";

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
            auto tpi = std::chrono::high_resolution_clock::now();
            Clique *clique = hconstructiva(matrizAdyacencias, listaAdyacencias, -1);
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

void porcentajeErrorVariandoMHeuristicaMayorGrado(int cantInstanciasPorM, int constanteN, int saltarDeA){
    std::string nombreArchivo = "prom-acierto-hconstructiva-mayor-grado-variando-m";

    std::stringstream ss;
    ss <<  "/home/jscherman/CLionProjects/algo3-tp3-cmf/datos/" << nombreArchivo << ".csv";
    std::ofstream a_file (ss.str());

    a_file << "n, m, promedioAcierto" << std::endl;
    int maxM = (constanteN*(constanteN-1))/2;
    int minM = 1; // Para no dividir por 0 en el promedio
    std::cout << "Variando m: {n=" << constanteN << "} => " << minM << " <= m <= " << maxM << std::endl;
    long long promedioErrorGlobal = 0;
    long long cantValores = 0;
    for (int i = minM; i <= maxM; i+=saltarDeA) {
        long long promedioErrorTotal = 0;
        for (int j = 0; j < cantInstanciasPorM; ++j) {
            std::vector<std::list<int>> listaAdyacencias = Utils::generarListaAdyacencias(constanteN, i, false, 0, 0);
            std::vector<std::vector<bool>> matrizAdyacencias = Utils::aMatrizAdyacencias(listaAdyacencias);
            Clique *cliqueConstructiva = hconstructiva(matrizAdyacencias, listaAdyacencias, -1);
            Clique cliqueExacto = exactoBTVertices(matrizAdyacencias, listaAdyacencias);
            promedioErrorTotal+= (cliqueConstructiva->frontera*100)/(cliqueExacto.frontera);
            delete cliqueConstructiva;
        }

        promedioErrorTotal = promedioErrorTotal/ cantInstanciasPorM;
        promedioErrorGlobal += promedioErrorTotal;
        cantValores++;
        std::cout << constanteN << ", "<<  i << ", " << promedioErrorTotal << std::endl ;
        a_file << constanteN << ", "<< i << ", " << promedioErrorTotal << std::endl;
    }
    promedioErrorGlobal = promedioErrorGlobal / cantValores;

    a_file.close();
    std::cout << "Listo! El promedio de error global es: " <<  promedioErrorGlobal << "%" << std::endl;
}

