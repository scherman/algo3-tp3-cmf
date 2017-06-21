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
    static std::vector<std::list<int>> aListaAdyacencias(int n, std::list<Eje> &listaIncidencias) {
        std::vector<std::list<int>> listaAdyacencias(n);
        for (std::list<Eje>::iterator it = listaIncidencias.begin(); it != listaIncidencias.end(); ++it) {
            Eje &eje = *it;
            listaAdyacencias[eje.origen].push_back(eje.destino);
            listaAdyacencias[eje.destino].push_back(eje.origen);
        }
        return listaAdyacencias;
    }

    static std::list<Eje> aListaIncidencias(std::vector<std::list<int>> &listaAdyacencias) {
        int n = listaAdyacencias.size();

        std::vector<std::vector<bool>> matrizAdyacencias(n, std::vector<bool>(n));
        for (int i = 0; i < n; ++i) {
            for (int r = 0; r < n; ++r) {
                matrizAdyacencias[i][r] = (i == r);
            }
        }

        std::list<Eje> listaIncidencias;
        for (int j = 0; j < n; ++j) {
            for (std::list<int>::iterator it = listaAdyacencias[j].begin(); it != listaAdyacencias[j].end(); ++it) {
                if (!matrizAdyacencias[j][*it]) listaIncidencias.push_back({j, *it, 0});
            }
        }

        return listaIncidencias;
    }

    static void imprimirListaIncidencias(std::list<Eje> &ejes) {
        for (std::list<Eje>::iterator it = ejes.begin(); it != ejes.end(); ++it) {
            std::cout << *it << " ";
        }
    }

    static void imprimirListaAdyacencias(std::vector<std::list<int>> &listaAdyacencias) {
        for (int i = 0; i < listaAdyacencias.size(); ++i) {
            std::cout << "N[" << i << "] = { ";
            for (std::list<int>::iterator it = listaAdyacencias[i].begin(); it != listaAdyacencias[i].end(); ++it) {
                std::cout << *it << " ";
            }
            std::cout <<  "}"<< std::endl;
        }
    }

    static void imprimirMatrizAdyacencias(int n, std::vector<std::vector<int>> &matrizAdyacencias) {
        for (int k = 0; k < n; ++k) {
            for (int i = 0; i < n; ++i) {
                std::cout << matrizAdyacencias[k][i] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    static std::list<Eje> generarListaIncidencias(int n, int m, bool conexo, int pesoMin, int pesoMax) {
        if (m > (n*(n-1))/2) throw std::invalid_argument( "No puede tener tantos ejes!");
        if (conexo && m < n - 1) throw std::invalid_argument( "No puede ser conexo porque m < n - 1");

        std::list<Eje> rutasExistentes;

        std::vector<std::vector<int>> ejesAgregados(n, std::vector<int>(n));
        for (int i = 0; i < n; ++i) {
            for (int r = 0; r < n; ++r) {
                ejesAgregados[i][r] = (i == r);
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
        return rutasExistentes;
    }

    static std::vector<std::list<int>> generarListaAdyacencias(int n, int m, bool conexo, int pesoMin, int pesoMax) {
        std::vector<std::list<int>> listaAdyacencias(n);
        std::list<Eje> listaIncidencias = generarListaIncidencias(n, m, conexo, pesoMin, pesoMax);
        for (std::list<Eje>::iterator it = listaIncidencias.begin(); it != listaIncidencias.end(); ++it) {
            Eje &eje = *it;
            listaAdyacencias[eje.origen].push_back(eje.destino);
            listaAdyacencias[eje.destino].push_back(eje.origen);
        }
        return listaAdyacencias;
    }

private:

};


#endif //ALGO3_TP3_CMF_UTILS_H
