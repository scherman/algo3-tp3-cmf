
#include "cmf-heuristica-blocal.h"

void fronteraEnCasoMaloBLocal(int initK, int maxK, int step){
    //Probaremos un grafo construido a partir de un número k. 
    //En todos los casos, la cmf tiene frontera 2(k-1) y el grafo tiene 3k+1 nodos

    std::string nombreArchivo = "datos/fronteras_caso_malo_blocal.csv";
    std::ofstream output(nombreArchivo);

    output << "k,frontera_hallada,frontera_maxima" << endl;
    cout << "k,frontera_hallada,frontera_maxima" << endl;
    for(int k = initK; k <= maxK; k+= step){
        auto listaAdyacencia = Utils::genCasoMaloBLocal(k);
        std::vector<std::vector<bool>> matrizAdyacencias(listaAdyacencia.size(), std::vector<bool>(listaAdyacencia.size(), false));
        Clique* clique = hconstructiva(matrizAdyacencias, listaAdyacencia, -1);
        busquedaLocalExtendiendoClique(*clique, matrizAdyacencias, listaAdyacencia);
        output << k << "," << clique->frontera << "," << 2*(k-2) << endl;
        cout << k << "," << clique->frontera << "," << 2*(k-2) << endl;
    }


    output.close();

    }