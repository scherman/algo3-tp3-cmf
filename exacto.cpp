//
// Created by jscherman on 20/06/17.
//

#include "cmf-algo-exacto.h"

int main(int argc, char** argv) {
    escribirTiemposVariandoNMCompleto(1, 1, 1000);
//    escribirTiemposVariandoNSinEjes(3, 1, 1000);
//    escribirTiemposVariandoN(3, 20, 1000);
//    escribirTiemposVariandoM(3, 30);

    unsigned n, m;
    stringTokenizer strTok;
    string linea;
    fstream input(argv[1], fstream::in);
    while(getline(input, linea)){
        if(linea == "-1 -1") continue;
        strTok.tokenize(linea, ' ');
        n = stoi(strTok[0]);
        m = stoi(strTok[1]);
        std::vector<std::list<int>> listaAdyacencias(n);
        std::vector<std::vector<bool>> matrizAdyacencias(n, std::vector<bool>(n));
        for (int j = 0; j < n; ++j) {
            for (int i = 0; i < n; ++i) {
                matrizAdyacencias[j][i] = false;
            }
        }

        int c = 0;
        for(int i = 0; i < m; i++){
            int v1, v2;
            getline(input, linea);
            strTok.tokenize(linea, ' ');
            v1 = stoi(strTok[0]);
            v2 = stoi(strTok[1]);
            listaAdyacencias[v1].push_back(v2);
            listaAdyacencias[v2].push_back(v1);
            matrizAdyacencias[v1][v2] = true;
            matrizAdyacencias[v2][v1] = true;
        }
        Clique cliqueMax = exactoBTVertices(matrizAdyacencias, listaAdyacencias);

        // Output
        std::cout << cliqueMax.frontera << " " << cliqueMax.vertices.size();
        for (std::list<int>::const_iterator it = cliqueMax.vertices.begin(); it != cliqueMax.vertices.end(); ++it) {
            std::cout << " " << *it ;
        }
        std::cout << std::endl;
    }
    input.close();
    return 0;
}