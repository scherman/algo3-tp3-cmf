//
// Created by jscherman on 20/06/17.
//

#include "cmf-algo-exacto.h"

int main(int argc, char** argv) {
    unsigned n, m;
    stringTokenizer strTok;
    string linea;

    while(getline(std::cin, linea)){
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
            getline(std::cin, linea);
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

    return 0;
}