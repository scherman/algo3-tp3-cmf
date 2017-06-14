//
// Created by jscherman on 13/06/17.
//

#ifndef ALGO3_TP3_CMF_CLIQUE_H
#define ALGO3_TP3_CMF_CLIQUE_H

#include <list>

struct Clique{
    int frontera;
    std::list<int> vertices;

    Clique(std::list<int> vertices, int frontera) : frontera(frontera), vertices(vertices){};
};

#endif //ALGO3_TP3_CMF_CLIQUE_H
