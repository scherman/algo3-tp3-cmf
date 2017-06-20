//
// Created by jscherman on 18/06/17.
//

#ifndef ALGO3_TP3_CMF_UTILS_H
#define ALGO3_TP3_CMF_UTILS_H

#include "Eje.h"
#include <list>
#include <iostream>
#include <random>

class Utils {

public:
    static void imprimirEjes(std::list<Eje> & ejes) {
        for (std::list<Eje>::iterator it = ejes.begin(); it != ejes.end(); ++it) {
            std::cout << *it << " ";
        }
    }

    static void imprimirMatrizAdyacencias(int n, bool **matrizAdyacencias) {
        for (int k = 0; k < n; ++k) {
            for (int i = 0; i < n; ++i) {
                std::cout << matrizAdyacencias[k][i] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    // Devuelve matriz de adyacencia y de incidencias
    static std::list<Eje> generarGrafo(int n, int m, bool conexo, int pesoMin, int pesoMax) {
        if (m > (n*(n-1))/2) throw std::invalid_argument( "No puede tener tantos ejes!");
        if (conexo && m < n - 1) throw std::invalid_argument( "No puede ser conexo porque m < n - 1");

        std::list<Eje> rutasExistentes;

        int **ejesAgregados= new int*[n];
        for(int i = 0; i < n; ++i) {
            ejesAgregados[i] = new int[n];
            for(int r = 0; r < n; ++r) {
                if (i == r) {
                    ejesAgregados[i][r] = 1;
                } else {
                    ejesAgregados[i][r] = 0;
                }
            }
        }

        std::random_device r;
        std::default_random_engine e1(r());
        std::uniform_int_distribution<int> uniform_dist_vertices(0, n-1);
        std::uniform_int_distribution<int> uniform_dist_pesos(pesoMin, pesoMax);
        if (conexo) {

            int origen = uniform_dist_vertices(e1);
            // Agrego m rutas existentes aleatorias
            while (rutasExistentes.size() < m) {
                int destino = uniform_dist_vertices(e1);
                if (ejesAgregados[origen][destino] == 0) {
                    // Todavia no agregue esta arista. La agrego
                    int peso = uniform_dist_pesos(e1);
                    ejesAgregados[origen][destino] = 1;
                    ejesAgregados[destino][origen] = 1;
                    rutasExistentes.push_back({origen, destino, peso});
                }
                origen = destino;
            }
        } else {
            // Agrego m rutas existentes aleatorias
            while (rutasExistentes.size() < m) {
                int origen = uniform_dist_vertices(e1);
                int destino = uniform_dist_vertices(e1);
                if (ejesAgregados[origen][destino] == 0) {
                    // Todavia no agregue esta arista. La agrego
                    int peso = uniform_dist_pesos(e1);
                    ejesAgregados[origen][destino] = 1;
                    ejesAgregados[destino][origen] = 1;
                    rutasExistentes.push_back({origen, destino, peso});
                }
            }
        }

        for (int j = 0; j < n; ++j) {
            delete ejesAgregados[j];
        }
        delete[] ejesAgregados;

        return rutasExistentes;
    }

private:

};


#endif //ALGO3_TP3_CMF_UTILS_H
