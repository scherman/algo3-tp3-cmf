//
// Created by jscherman on 13/06/17.
//

#ifndef ALGO3_TP3_CMF_CLIQUE_H
#define ALGO3_TP3_CMF_CLIQUE_H

#include <iostream>
#include <list>

struct Clique{
    int frontera;
    std::list<int> vertices;

    Clique() : frontera(0) {};
    Clique(std::list<int> vertices, int frontera) : frontera(frontera), vertices(vertices){
//        std::cout << "Creado " << this << std::endl;
    };

    ~Clique() {
//        std::cout << "Destruido " << this << std::endl;
    }

    friend std::ostream& operator<<(std::ostream& os, const Clique& s){
        os << "[Clique(" << &s << ")] frontera: " << s.frontera << ", nodos: { ";
        for (std::list<int>::const_iterator it = s.vertices.begin(); it != s.vertices.end(); ++it) {
            os << *it << " ";
        }
        os << "}";
        return os;
    }
};

#endif //ALGO3_TP3_CMF_CLIQUE_H
