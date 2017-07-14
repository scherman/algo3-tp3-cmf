//
// Created by jscherman on 08/07/17.
//

#include "graspv2.h"
#include "busqueda-local.h"
#include <algorithm>
#include <sstream>
#include <chrono>
#include "cmf-algo-exacto.h"

class Sorter {
    bool *usados;
    std::vector<std::list<int>> &listaAdyacencias;
public:
    Sorter(bool *usados, std::vector<std::list<int>> &lista) : usados(usados), listaAdyacencias(lista) {}
    bool operator()(const int o1, const int o2) const {
        return (!usados[o1] && usados[o2]) || (listaAdyacencias[o1].size() > listaAdyacencias[o2].size()) ;
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
    std::stable_sort(nodos.begin(), nodos.end(), sorter);
    int actual = nodos[rand() % std::min(RCL, n)];
//    for (int i = 0; i < nodos.size(); ++i) {
//        std::cout << nodos[i] << "(" << usados[nodos[i]] << ", " << listaAdyacencias[nodos[i]].size() << ") ";
//    }
//    std::cout << std::endl;

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

void variarRCL(int cantInstanciasPorRCL, int minRCL, int maxRCL, int iterations, int n, int m, int saltarDeA){
    std::string nombreArchivo = "grasp-mayor-grado-variando-rcl";

    std::stringstream ss;
    ss <<  "/home/jscherman/CLionProjects/algo3-tp3-cmf/datos/" << nombreArchivo << ".csv";
    std::ofstream a_file (ss.str());

    a_file << "n, m, RCL, iterations, tiempoTotal, promedioAciertoRandom, promedioAciertoCasoMalo, promedioAciertoTotal" << std::endl;

    std::cout << "Variando RCL: {n = " << n << ", m=" << m << "} => " << minRCL << " <= RCL <= " << maxRCL << std::endl;
    for (int i = minRCL; i <= maxRCL; i+=saltarDeA) {

        long long tiempoTotal = 0;
        long long promedioAciertoTotal = 0;
        long long promedioAciertoRandom = 0;
        long long promedioAciertoCasoMalo = 0;
        for (int j = 0; j < cantInstanciasPorRCL; ++j) {
            {
                std::vector<std::list<int>> listaAdyacencias = Utils::generarListaAdyacencias(n, m, false, 0, 0);
                std::vector<std::vector<bool>> matrizAdyacencias = Utils::aMatrizAdyacencias(listaAdyacencias);
                Clique cliqueExacta = exactoBTVertices(matrizAdyacencias, listaAdyacencias);
                auto tpi = std::chrono::high_resolution_clock::now();
                Clique cliqueEncontrada = grasp2(matrizAdyacencias, listaAdyacencias, i, iterations);
                auto tpf = std::chrono::high_resolution_clock::now();
                auto tiempo = std::chrono::duration_cast<std::chrono::nanoseconds>(tpf-tpi).count();
                tiempoTotal += tiempo;
                promedioAciertoTotal += (cliqueEncontrada.frontera*100)/cliqueExacta.frontera;
                promedioAciertoRandom += (cliqueEncontrada.frontera*100)/cliqueExacta.frontera;
            }
            {
                std::vector<std::list<int>> listaAdyacencias = Utils::genCasoMaloBLocal((n-1)/3);
                std::vector<std::vector<bool>> matrizAdyacencias = Utils::aMatrizAdyacencias(listaAdyacencias);
                Clique cliqueExacta = exactoBTVertices(matrizAdyacencias, listaAdyacencias);
                auto tpi = std::chrono::high_resolution_clock::now();
                Clique cliqueEncontrada = grasp2(matrizAdyacencias, listaAdyacencias, i, iterations);
                auto tpf = std::chrono::high_resolution_clock::now();
                auto tiempo = std::chrono::duration_cast<std::chrono::nanoseconds>(tpf-tpi).count();
                tiempoTotal += tiempo;
                promedioAciertoTotal += (cliqueEncontrada.frontera*100)/cliqueExacta.frontera;
                promedioAciertoCasoMalo += (cliqueEncontrada.frontera*100)/cliqueExacta.frontera;
            }
        }

        tiempoTotal = tiempoTotal / (2*cantInstanciasPorRCL);
        promedioAciertoTotal= promedioAciertoTotal / (2*cantInstanciasPorRCL);
        promedioAciertoRandom= promedioAciertoRandom / cantInstanciasPorRCL;
        promedioAciertoCasoMalo= promedioAciertoCasoMalo/ cantInstanciasPorRCL;
        std::cout << n << ", " << m << ", " <<  i << ", " << iterations << ", " << tiempoTotal << ", " << promedioAciertoRandom << ", " << promedioAciertoCasoMalo << ", " << promedioAciertoTotal << std::endl;
        a_file << n << ", " << m << ", " <<  i << ", " << iterations << ", " << tiempoTotal << ", " << promedioAciertoRandom << ", " << promedioAciertoCasoMalo << ", " << promedioAciertoTotal << std::endl;
    }

    a_file.close();
    std::cout << "Listo!" << std::endl;
}

void variarIterations(int cantInstanciasPorIterations, int minIterations, int maxIterations, int RCL, int n, int m, int saltarDeA){
    std::string nombreArchivo = "grasp-mayor-grado-variando-iterations";

    std::stringstream ss;
    ss <<  "/home/jscherman/CLionProjects/algo3-tp3-cmf/datos/" << nombreArchivo << ".csv";
    std::ofstream a_file (ss.str());

    a_file << "n, m, RCL, iterations, tiempoTotal, promedioAciertoRandom, promedioAciertoCasoMalo, promedioAciertoTotal" << std::endl;

    std::cout << "Variando iterations: {n = " << n << ", m=" << m << "} => " << minIterations << " <= iterations <= " << maxIterations << std::endl;
    for (int i = minIterations; i <= maxIterations; i+=saltarDeA) {

        long long tiempoTotal = 0;
        long long promedioAciertoTotal = 0;
        long long promedioAciertoRandom = 0;
        long long promedioAciertoCasoMalo = 0;
        for (int j = 0; j < cantInstanciasPorIterations; ++j) {
            {
                std::vector<std::list<int>> listaAdyacencias = Utils::generarListaAdyacencias(n, m, false, 0, 0);
                std::vector<std::vector<bool>> matrizAdyacencias = Utils::aMatrizAdyacencias(listaAdyacencias);
                Clique cliqueExacta = exactoBTVertices(matrizAdyacencias, listaAdyacencias);
                auto tpi = std::chrono::high_resolution_clock::now();
                Clique cliqueEncontrada = grasp2(matrizAdyacencias, listaAdyacencias, RCL, i);
                auto tpf = std::chrono::high_resolution_clock::now();
                auto tiempo = std::chrono::duration_cast<std::chrono::nanoseconds>(tpf-tpi).count();
                tiempoTotal += tiempo;
                promedioAciertoTotal += (cliqueEncontrada.frontera*100)/cliqueExacta.frontera;
                promedioAciertoRandom += (cliqueEncontrada.frontera*100)/cliqueExacta.frontera;
            }
            {
                std::vector<std::list<int>> listaAdyacencias = Utils::genCasoMaloBLocal((n-1)/3);
                std::vector<std::vector<bool>> matrizAdyacencias = Utils::aMatrizAdyacencias(listaAdyacencias);
                Clique cliqueExacta = exactoBTVertices(matrizAdyacencias, listaAdyacencias);
                auto tpi = std::chrono::high_resolution_clock::now();
                Clique cliqueEncontrada = grasp2(matrizAdyacencias, listaAdyacencias, RCL, i);
                auto tpf = std::chrono::high_resolution_clock::now();
                auto tiempo = std::chrono::duration_cast<std::chrono::nanoseconds>(tpf-tpi).count();
                tiempoTotal += tiempo;
                promedioAciertoTotal += (cliqueEncontrada.frontera*100)/cliqueExacta.frontera;
                promedioAciertoCasoMalo += (cliqueEncontrada.frontera*100)/cliqueExacta.frontera;
            }
        }

        tiempoTotal = tiempoTotal / (2*cantInstanciasPorIterations);
        promedioAciertoTotal= promedioAciertoTotal / (2*cantInstanciasPorIterations);
        promedioAciertoRandom= promedioAciertoRandom / cantInstanciasPorIterations;
        promedioAciertoCasoMalo= promedioAciertoCasoMalo/ cantInstanciasPorIterations;
        std::cout << n << ", " << m << ", " <<  RCL << ", " << i << ", " << tiempoTotal << ", " << promedioAciertoRandom << ", " << promedioAciertoCasoMalo << ", " << promedioAciertoTotal << std::endl;
        a_file << n << ", " << m << ", " <<  RCL << ", " << i<< ", " << tiempoTotal << ", " << promedioAciertoRandom << ", " << promedioAciertoCasoMalo << ", " << promedioAciertoTotal << std::endl;
    }

    a_file.close();
    std::cout << "Listo!" << std::endl;
}

void escribirTiemposVariandoNGrasp(int cantInstanciasPorN, int minN, int maxN, int RCL, int iterations, int saltarDeA){
    std::string nombreArchivo = "tiempos-grasp-mayor-grado-variando-n";

    std::stringstream ss;
    ss <<  "/home/jscherman/CLionProjects/algo3-tp3-cmf/datos/" << nombreArchivo << ".csv";
    std::ofstream a_file (ss.str());

    a_file << "n, m, RCL, iterations, tiempoTotal" << std::endl;

    int m = minN*(minN-1)/2;
    std::cout << "Variando n: {m=" << m << ", RCL=" << RCL << ", iterations=" << iterations << "} => " << minN << " <= n <= " << maxN << std::endl;
    for (int i = minN; i <= maxN; i+=saltarDeA) {

        long long tiempoTotal = 0;
        for (int j = 0; j < cantInstanciasPorN; ++j) {
            std::vector<std::list<int>> listaAdyacencias = Utils::generarListaAdyacencias(i, m, false, 0, 0);
            std::vector<std::vector<bool>> matrizAdyacencias = Utils::aMatrizAdyacencias(listaAdyacencias);
            auto tpi = std::chrono::high_resolution_clock::now();
            Clique clique = grasp2(matrizAdyacencias, listaAdyacencias, RCL, iterations);
            auto tpf = std::chrono::high_resolution_clock::now();
            auto tiempo = std::chrono::duration_cast<std::chrono::nanoseconds>(tpf-tpi).count();
            tiempoTotal+= tiempo;
        }

        tiempoTotal = tiempoTotal/ cantInstanciasPorN;
        std::cout << i << ", " << m << ", " << RCL << ", " << iterations << ", "<< tiempoTotal << std::endl ;
        a_file << i << ", " << m << ", " << RCL << ", " << iterations << ", "<< tiempoTotal << std::endl ;
    }

    a_file.close();
    std::cout << "Listo!" << std::endl;
}

void escribirTiemposVariandoNM0Grasp(int cantInstanciasPorN, int minN, int maxN, int RCL, int iterations, int saltarDeA){
    std::string nombreArchivo = "tiempos-grasp-mayor-grado-variando-n-m0";

    std::stringstream ss;
    ss <<  "/home/jscherman/CLionProjects/algo3-tp3-cmf/datos/" << nombreArchivo << ".csv";
    std::ofstream a_file (ss.str());

    a_file << "n, m, RCL, iterations, tiempoTotal" << std::endl;

    int m = 0;
    std::cout << "Variando n: {m=" << m << ", RCL=" << RCL << ", iterations=" << iterations << "} => " << minN << " <= n <= " << maxN << std::endl;
    for (int i = minN; i <= maxN; i+=saltarDeA) {

        long long tiempoTotal = 0;
        for (int j = 0; j < cantInstanciasPorN; ++j) {
            std::vector<std::list<int>> listaAdyacencias = Utils::generarListaAdyacencias(i, m, false, 0, 0);
            std::vector<std::vector<bool>> matrizAdyacencias = Utils::aMatrizAdyacencias(listaAdyacencias);
            auto tpi = std::chrono::high_resolution_clock::now();
            Clique clique = grasp2(matrizAdyacencias, listaAdyacencias, RCL, iterations);
            auto tpf = std::chrono::high_resolution_clock::now();
            auto tiempo = std::chrono::duration_cast<std::chrono::nanoseconds>(tpf-tpi).count();
            tiempoTotal+= tiempo;
        }

        tiempoTotal = tiempoTotal/ cantInstanciasPorN;
        std::cout << i << ", " << m << ", " << RCL << ", " << iterations << ", "<< tiempoTotal << std::endl ;
        a_file << i << ", " << m << ", " << RCL << ", " << iterations << ", "<< tiempoTotal << std::endl ;
    }

    a_file.close();
    std::cout << "Listo!" << std::endl;
}

void escribirTiemposVariandoNMCompletoGrasp(int cantInstanciasPorN, int minN, int maxN, int RCL, int iterations, int saltarDeA){
    std::string nombreArchivo = "tiempos-grasp-mayor-grado-variando-n-mcompleto";

    std::stringstream ss;
    ss <<  "/home/jscherman/CLionProjects/algo3-tp3-cmf/datos/" << nombreArchivo << ".csv";
    std::ofstream a_file (ss.str());

    a_file << "n, m, RCL, iterations, tiempoTotal" << std::endl;

    std::cout << "Variando n: {RCL=" << RCL << ", iterations=" << iterations << "} => " << minN << " <= n <= " << maxN << std::endl;
    for (int i = minN; i <= maxN; i+=saltarDeA) {
        int m = i*(i-1)/2;

        long long tiempoTotal = 0;
        for (int j = 0; j < cantInstanciasPorN; ++j) {
            std::vector<std::list<int>> listaAdyacencias = Utils::generarListaAdyacencias(i, m, false, 0, 0);
            std::vector<std::vector<bool>> matrizAdyacencias = Utils::aMatrizAdyacencias(listaAdyacencias);
            auto tpi = std::chrono::high_resolution_clock::now();
            Clique clique = grasp2(matrizAdyacencias, listaAdyacencias, RCL, iterations);
            auto tpf = std::chrono::high_resolution_clock::now();
            auto tiempo = std::chrono::duration_cast<std::chrono::nanoseconds>(tpf-tpi).count();
            tiempoTotal+= tiempo;
        }

        tiempoTotal = tiempoTotal/ cantInstanciasPorN;
        std::cout << i << ", " << m << ", " << RCL << ", " << iterations << ", "<< tiempoTotal << std::endl ;
        a_file << i << ", " << m << ", " << RCL << ", " << iterations << ", "<< tiempoTotal << std::endl ;
    }

    a_file.close();
    std::cout << "Listo!" << std::endl;
}

void escribirTiemposVariandoMGrasp(int cantInstanciasPorM, int constanteN, int RCL, int iterations, int saltarDeA){
    std::string nombreArchivo = "tiempos-grasp-mayor-grado-variando-m";

    std::stringstream ss;
    ss <<  "/home/jscherman/CLionProjects/algo3-tp3-cmf/datos/" << nombreArchivo << ".csv";
    std::ofstream a_file (ss.str());

    a_file << "n, m, RCL, iterations, tiempoTotal" << std::endl;
    int maxM = (constanteN*(constanteN-1))/2;
    int minM = 1;
    std::cout << "Variando m: {n=" << constanteN << ", RCL=" << RCL << ", iterations=" << iterations << "} => " << minM << " <= m <= " << maxM << std::endl;
    for (int i = minM; i <= maxM; i+=saltarDeA) {
        long long tiempoTotal = 0;
        for (int j = 0; j < cantInstanciasPorM; ++j) {
            std::vector<std::list<int>> listaAdyacencias = Utils::generarListaAdyacencias(constanteN, i, false, 0, 0);
            std::vector<std::vector<bool>> matrizAdyacencias = Utils::aMatrizAdyacencias(listaAdyacencias);
            auto tpi = std::chrono::high_resolution_clock::now();
            Clique clique = grasp2(matrizAdyacencias, listaAdyacencias, RCL, iterations);
            auto tpf = std::chrono::high_resolution_clock::now();
            auto tiempo = std::chrono::duration_cast<std::chrono::nanoseconds>(tpf-tpi).count();
            tiempoTotal+= tiempo;
        }

        tiempoTotal = tiempoTotal/ cantInstanciasPorM;
        std::cout << constanteN << ", "<<  i << ", " << RCL << ", " << iterations << ", "<< tiempoTotal << std::endl ;
        a_file << constanteN << ", "<<  i << ", " << RCL << ", " << iterations << ", "<< tiempoTotal << std::endl ;
    }

    a_file.close();
    std::cout << "Listo!" << std::endl;
}

void porcentajeErrorVariandoMGrasp(int cantInstanciasPorM, int constanteN, int RCL, int iterations, int saltarDeA){
    std::string nombreArchivo = "prom-acierto-grasp-mayor-grado-variando-m";

    std::stringstream ss;
    ss <<  "/home/jscherman/CLionProjects/algo3-tp3-cmf/datos/" << nombreArchivo << ".csv";
    std::ofstream a_file (ss.str());

    a_file << "n, m, RCL, iterations, promedioAcierto" << std::endl;
    int maxM = (constanteN*(constanteN-1))/2;
    int minM = 1; // Para no dividir por 0 en el promedio
    std::cout << "Variando m: {n=" << constanteN << ", RCL=" << RCL << ", iterations=" << iterations << "} => " << minM << " <= m <= " << maxM << std::endl;
    long long promedioErrorGlobal = 0;
    long long cantValores = 0;
    for (int i = minM; i <= maxM; i+=saltarDeA) {
        long long promedioErrorTotal = 0;
        for (int j = 0; j < cantInstanciasPorM; ++j) {
            std::vector<std::list<int>> listaAdyacencias = Utils::generarListaAdyacencias(constanteN, i, false, 0, 0);
            std::vector<std::vector<bool>> matrizAdyacencias = Utils::aMatrizAdyacencias(listaAdyacencias);
            Clique cliqueObtenida = grasp2(matrizAdyacencias, listaAdyacencias, RCL, iterations);
            Clique cliqueExacto = exactoBTVertices(matrizAdyacencias, listaAdyacencias);
            promedioErrorTotal+= (cliqueObtenida.frontera*100)/(cliqueExacto.frontera);
        }

        promedioErrorTotal = promedioErrorTotal/ cantInstanciasPorM;
        promedioErrorGlobal += promedioErrorTotal;
        cantValores++;
        std::cout << constanteN << ", "<<  i << ", " << RCL << ", " << iterations << ", " << promedioErrorTotal << std::endl ;
        a_file << constanteN << ", "<< i << ", " << RCL << ", " << iterations << ", " << promedioErrorTotal << std::endl ;
    }
    promedioErrorGlobal = promedioErrorGlobal / cantValores;

    a_file.close();
    std::cout << "Listo! El promedio de error global es: " <<  promedioErrorGlobal << "%" << std::endl;
}

void fronteraEnCasoMaloGrasp(int initK, int maxK, int RCL, int iterations, int step){
    //Probaremos un grafo construido a partir de un número k.
    //En todos los casos, la cmf tiene frontera 2(k-1) y el grafo tiene 3k+1 nodos

    std::string nombreArchivo = "datos/frontera-grasp-variando-k-caso-malo.csv";
    std::ofstream output(nombreArchivo);

    output << "k,frontera_hallada,frontera_maxima" << endl;
    cout << "k,frontera_hallada,frontera_maxima" << endl;
    for(int k = initK; k <= maxK; k+= step){
        auto listaAdyacencia = Utils::genCasoMaloGrasp(k, RCL);
        std::vector<std::vector<bool>> matrizAdyacencias(listaAdyacencia.size(), std::vector<bool>(listaAdyacencia.size(), false));
        Clique clique = grasp2(matrizAdyacencias, listaAdyacencia, RCL, iterations);
        output << k << "," << clique.frontera << "," << 2*(k-1) << endl;
        cout << k << "," << clique.frontera << "," << 2*(k-1) << endl;
    }


    output.close();

}

void porcentajeErrorVariandoNCasoBuenoGraspMayorGrado(int cantInstanciasPorN, int minN, int maxN, int saltarDeA, int RCL, int iterations){
    std::string nombreArchivo = "prom-acierto-grasp-mayor-grado-variando-n-caso-bueno";

    std::stringstream ss;
    ss <<  "/home/jscherman/CLionProjects/algo3-tp3-cmf/datos/" << nombreArchivo << ".csv";
    std::ofstream a_file (ss.str());

    a_file << "n, promedioAcierto" << std::endl;
    long long promedioErrorGlobal = 0;
    long long cantValores = 0;
    for (int i = minN; i <= maxN; i+=saltarDeA) {
        long long promedioErrorTotal = 0;
        for (int j = 0; j < cantInstanciasPorN; ++j) {

            std::vector<std::list<int>> listaAdyacencias = Utils::generarListaAdyacencias(i, (i*(i-1))/2, false, 0, 0);
            std::vector<std::vector<bool>> matrizAdyacencias = Utils::aMatrizAdyacencias(listaAdyacencias);
            Clique cliqueConstructiva = grasp2(matrizAdyacencias, listaAdyacencias, RCL, iterations);
            Clique cliqueExacto = exactoBTVertices(matrizAdyacencias, listaAdyacencias);
            promedioErrorTotal+= (cliqueConstructiva.frontera*100)/(cliqueExacto.frontera);
        }

        promedioErrorTotal = promedioErrorTotal/ cantInstanciasPorN;
        promedioErrorGlobal += promedioErrorTotal;
        cantValores++;
        std::cout << i << ", " << promedioErrorTotal << std::endl ;
        a_file << i << ", " << promedioErrorTotal << std::endl;
    }
    promedioErrorGlobal = promedioErrorGlobal / cantValores;

    a_file.close();
    std::cout << "Listo! El promedio de error global es: " <<  promedioErrorGlobal << "%" << std::endl;
}

void porcentajeErrorVariandoKCasoMaloGraspMayorGrado(int cantInstanciasPorK, int minK, int maxK, int saltarDeA, int RCL, int iterations){
    std::string nombreArchivo = "prom-acierto-grasp-mayor-grado-variando-k-caso-malo";

    std::stringstream ss;
    ss <<  "/home/jscherman/CLionProjects/algo3-tp3-cmf/datos/" << nombreArchivo << ".csv";
    std::ofstream a_file (ss.str());

    a_file << "k, promedioAcierto" << std::endl;
    long long promedioErrorGlobal = 0;
    long long cantValores = 0;
    for (int i = minK; i <= maxK; i+=saltarDeA) {
        long long promedioErrorTotal = 0;
        for (int j = 0; j < cantInstanciasPorK; ++j) {

            std::vector<std::list<int>> listaAdyacencias = Utils::genCasoMaloGrasp(i, RCL+1);
            std::vector<std::vector<bool>> matrizAdyacencias = Utils::aMatrizAdyacencias(listaAdyacencias);
            Clique cliqueObtenida = grasp2(matrizAdyacencias, listaAdyacencias, RCL, iterations);
            promedioErrorTotal+= (cliqueObtenida.frontera*100)/(2*(i-1));
        }

        promedioErrorTotal = promedioErrorTotal/ cantInstanciasPorK;
        promedioErrorGlobal += promedioErrorTotal;
        cantValores++;
        std::cout << i << ", " << promedioErrorTotal << std::endl ;
        a_file << i << ", " << promedioErrorTotal << std::endl;
    }
    promedioErrorGlobal = promedioErrorGlobal / cantValores;

    a_file.close();
    std::cout << "Listo! El promedio de error global es: " <<  promedioErrorGlobal << "%" << std::endl;
}

