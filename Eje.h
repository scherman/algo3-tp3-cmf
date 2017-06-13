//
// Created by jscherman on 13/06/17.
//

#ifndef ALGO3_TP3_CMF_EJE_H
#define ALGO3_TP3_CMF_EJE_H

#include <ostream>

struct Eje {
    int origen, destino, peso;
    bool operator<(const Eje& r) {return peso < r.peso;}

    friend std::ostream & operator<<(std::ostream & os, const Eje& e) {
        os << "(" << e.origen << ", " << e.destino << ", " << e.peso << ")";
    }
};

#endif //ALGO3_TP3_CMF_EJE_H

