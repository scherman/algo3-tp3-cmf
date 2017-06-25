#include "cmf-algo-exacto.h"
#include "Utils.h"

int main(int argc, char** argv) {
    unsigned n, m;
    stringTokenizer strTok;
    string linea;

    while(getline(std::cin, linea)){
        if(linea == "-1 -1") continue;

        strTok.tokenize(linea, ' ');
        n = stoi(strTok[0]);
        m = stoi(strTok[1]);
        std::list<Eje> listaIncidencias;

        for(int i = 0; i < m; i++){
            int v1, v2;
            getline(std::cin, linea);
            strTok.tokenize(linea, ' ');
            v1 = stoi(strTok[0]);
            v2 = stoi(strTok[1]);
            listaIncidencias.push_back({v1,v2});
        }

        std::vector<std::list<int>> listaAdyacencias = Utils::aListaAdyacencias(n, listaIncidencias);
        std::vector<std::vector<bool>> matrizAdyacencias = Utils::aMatrizAdyacencias(listaAdyacencias);
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