//
// Created by jscherman on 15/06/17.
//

#include <iostream>
#include "Clique.h"
#include "Eje.h"
#include "DisjointSet.h"
#include <list>
#include "stringTokenizer.hpp"
#include <fstream>

Clique * cmfHeuristicoConstructivo(){

}

int main(int argc, char** argv) {
    unsigned n, m;
    stringTokenizer strTok;
    string linea;
    fstream input(argv[1], fstream::in);
    while(getline(input, linea)){
        if(linea == "-1 -1") continue;
        strTok.tokenize(linea, ' ');
        n = stoi(strTok[0]);
        m = stoi(strTok[1]);
        std::list<int> listaAdyacencias[n];
        std::list<Eje> listaIncidencias;
        int c = 0;
        for(int i = 0; i <= m; i++){
            int v1, v2;
            getline(input, linea);
            strTok.tokenize(linea, ' ');
            v1 = stoi(strTok[0]);
            v2 = stoi(strTok[1]);
            listaAdyacencias[v1].push_back(v2);
            listaAdyacencias[v2].push_back(v1);
            listaIncidencias.push_back({v1,v2});
        }
        DisjointSet uds = DisjointSet(n);
//        Clique *cliqueMax = cmf(uds, listaIncidencias, listaAdyacencias);

//         Output
//        std::cout << cliqueMax->frontera << " " << cliqueMax->vertices.size();
//        for (std::list<int>::const_iterator it = cliqueMax->vertices.begin(); it != cliqueMax->vertices.end(); ++it) {
//            std::cout << " " << *it ;
//        }
//        std::cout << std::endl;


//        delete cliqueMax;
    }
    input.close();
    return 0;
}





int main(int argc, char** argv) {
    unsigned n, m;
    stringTokenizer strTok;
    string linea;
    fstream input(argv[1], fstream::in);
    while(getline(input, linea)){
        if(linea == "-1 -1") continue;
        strTok.tokenize(linea, ' ');
        n = stoi(strTok[0]);
        m = stoi(strTok[1]);
        std::list<int> listaAdyacencias[n];
        std::list<Eje> listaIncidencias;
        int c = 0;
        for(int i = 0; i <= m; i++){
            int v1, v2;
            getline(input, linea);
            strTok.tokenize(linea, ' ');
            v1 = stoi(strTok[0]);
            v2 = stoi(strTok[1]);
            listaAdyacencias[v1].push_back(v2);
            listaAdyacencias[v2].push_back(v1);
            listaIncidencias.push_back({v1,v2});
        }
        DisjointSet uds = DisjointSet(n);
        Clique *cliqueMax = cmf(uds, listaIncidencias, listaAdyacencias);

        // Output
        std::cout << cliqueMax->frontera << " " << cliqueMax->vertices.size();
        for (std::list<int>::const_iterator it = cliqueMax->vertices.begin(); it != cliqueMax->vertices.end(); ++it) {
            std::cout << " " << *it ;
        }
        std::cout << std::endl;


        delete cliqueMax;
    }
    input.close();
    return 0;
}
