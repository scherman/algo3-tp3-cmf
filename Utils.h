//
// Created by jscherman on 18/06/17.
//

#ifndef ALGO3_TP3_CMF_UTILS_H
#define ALGO3_TP3_CMF_UTILS_H

#include "Eje.h"
#include <list>
#include <iostream>
#include <stdexcept>
#include <random>

class Utils {

public:
    static std::vector<std::vector<bool>> aMatrizAdyacencias(std::vector<std::list<int>> &listaAdyacencias) {
        int n = listaAdyacencias.size();
        std::vector<std::vector<bool>> matrizAdyacencias(n, std::vector<bool>(n));

        for (int j = 0; j < n; ++j) {
            for (int i = 0; i < n; ++i) {
                matrizAdyacencias[j][i] = false;
            }
        }

        for (int i = 0; i < n; ++i) {
            for (std::list<int>::iterator it = listaAdyacencias[i].begin(); it != listaAdyacencias[i].end(); ++it) {
                matrizAdyacencias[i][*it] = true;
                matrizAdyacencias[*it][i] = true;
            }
        }
        return matrizAdyacencias;
    }

    static std::vector<std::vector<bool>> aMatrizAdyacencias(int n, std::list<Eje> &listaIncidencias) {
        std::vector<std::vector<bool>> matrizAdyacencias(n, std::vector<bool>(n));

        for (int j = 0; j < n; ++j) {
            for (int i = 0; i < n; ++i) {
                matrizAdyacencias[j][i] = false;
            }
        }

        for (std::list<Eje>::iterator it = listaIncidencias.begin(); it != listaIncidencias.end(); ++it) {
            matrizAdyacencias[it->origen][it->destino] = true;
            matrizAdyacencias[it->destino][it->origen] = true;
        }
        return matrizAdyacencias;
    }

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

        std::vector<std::vector<bool>> matrizAdyacencias = aMatrizAdyacencias(listaAdyacencias);

        std::list<Eje> listaIncidencias;
        for (int j = 0; j < n; ++j) {
            for (std::list<int>::iterator it = listaAdyacencias[j].begin(); it != listaAdyacencias[j].end(); ++it) {
                if (matrizAdyacencias[j][*it]) listaIncidencias.push_back({j, *it, 0});
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

    static void imprimirMatrizAdyacencias(int n, std::vector<std::vector<bool>> &matrizAdyacencias) {
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

    static std::vector<std::list<int>> genCasoMaloBLocal(int k){

        std::vector<std::list<int>> listaAdyacencias((k+1) + 2*k, std::list<int>());

        //el nodo 0 estará conectado con los siguientes k nodos
        for(int i = 1; i <= k; i++){
            listaAdyacencias[0].push_back(i);
            listaAdyacencias[i].push_back(0);
        }

        //el nodo k+1 estará conectado con los siguientes k-1 nodos
        for(int i = k + 2; i <= 2*k; i++){
            listaAdyacencias[k+1].push_back(i);
            listaAdyacencias[i].push_back(k+1);
        }

        //el nodo 2k+1 estará conectado con los siguientes k-1
        for(int i = 2*k+2; i <= 3*k; i++){
            listaAdyacencias[2*k+1].push_back(i);
            listaAdyacencias[i].push_back(2*k+1);
        }

        //los nodos k+1 y 2k+1 estarán conectados entre sí
        listaAdyacencias[k+1].push_back(2*k+1);
        listaAdyacencias[2*k+1].push_back(k+1);

        return listaAdyacencias;
    }

    static std::vector<std::list<int>> genCasoMaloGrasp(int k, int cantGEstrellas){

        std::vector<std::list<int>> listaAdyacencias(cantGEstrellas*(k+1) + 2*k, std::list<int>());

        // Genero grafos estrella
        for (int j = 0; j < cantGEstrellas; ++j) {
            // El nodo j estará conectado con los siguientes k nodos
//            std::cout << "Adyacencias " << j << ":";
            for(int i = j*k+j+1; i <= (j+1)*k+j; i++){
                listaAdyacencias[j*(k+1)].push_back(i);
                listaAdyacencias[i].push_back(j*(k+1));
//                std::cout << " " << i;
            }
//            std::cout << std::endl;
        }

        //el nodoCMF1  estará conectado con los siguientes k-1 nodos
        int nodoCMF1 = cantGEstrellas*(k+1);
//        std::cout << "Adyacencias nodoCMF1 " << nodoCMF1 << ":";
        for(int i = nodoCMF1 + 1; i <= nodoCMF1+k-1; i++){
            listaAdyacencias[nodoCMF1].push_back(i);
            listaAdyacencias[i].push_back(nodoCMF1);
//            std::cout << " " << i;
        }
//        std::cout << std::endl;

        //el nodoCMF1  estará conectado con los siguientes k-1 nodos
        int nodoCMF2 = nodoCMF1 + k;
//        std::cout << "Adyacencias nodoCMF2 " << nodoCMF2 << ":";
        for(int i = nodoCMF2 + 1; i <= nodoCMF2+k-1; i++){
            listaAdyacencias[nodoCMF2].push_back(i);
            listaAdyacencias[i].push_back(nodoCMF2);
//            std::cout << " " << i;
        }
//        std::cout << std::endl;

        //los nodos nodoCMF1 y nodoCMF2 estarán conectados entre sí
//        std::cout << "Conectar nodoCMF1 nodoCMF2: (" << nodoCMF1 << ", " << nodoCMF2 << ")" << std::endl;
        listaAdyacencias[nodoCMF1].push_back(nodoCMF2);
        listaAdyacencias[nodoCMF2].push_back(nodoCMF1);

        return listaAdyacencias;
    }

private:

};


#endif //ALGO3_TP3_CMF_UTILS_H
