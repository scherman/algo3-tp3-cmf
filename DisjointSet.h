//
// Created by jscherman on 24/05/17.
//

#ifndef ALGO3_TP3_CMF_DISJOINTSET_H
#define ALGO3_TP3_CMF_DISJOINTSET_H


#include "Eje.h"
#include <list>
#include <iostream>

class DisjointSet {

public:

    struct Subset {
        int representante;
        int cantEjes;
        int rank;
        std::list<int> nodos;

        bool esClique(){
            return cantEjes == ((nodos.size()*(nodos.size()-1))/2);
        }

        ~Subset(){
//            std::cout << "Borrando subset: " << this << std::endl;
        }

        Subset(){};

        Subset(const Subset &set) {
            representante = set.representante;
            cantEjes = set.cantEjes;
            rank = set.rank;
            nodos = std::list<int>(set.nodos);
        };

        friend std::ostream& operator<<(std::ostream& os, const Subset& s){
            os << "[Subset] representante: " << s.representante << ", rank: " << s.rank << ", cantEjes: " << s.cantEjes << ", nodos: { ";
            for (std::list<int>::const_iterator it = s.nodos.begin(); it != s.nodos.end(); ++it) {
                os << *it << " ";
            }
            os << "}";
            return os;
        }

        bool operator==(const Subset& s){
            return representante == s.representante;
        }

        bool operator!=(const Subset& s){
            return representante != s.representante;
        }

    };

    DisjointSet(int n) : cantNodos(n) {
        subsets = new Subset*[n]();
        for (int i = 0; i < n; ++i) {
            makeSet(i);
        }
    };

    DisjointSet(const DisjointSet &uds) : cantNodos(uds.cantNodos) {
        subsets = new Subset*[cantNodos]();
        for (int i = 0; i < cantNodos; ++i) {
            subsets[i] = new Subset(*uds.subsets[i]);
        }
    };

    ~DisjointSet(){
        for (int i = 0; i < cantNodos; ++i) {
            delete subsets[i];
        }
        delete[] subsets;
    }

//    std::list<Eje> & edges() {return totalEdges;};

    void makeSet(int v) {
        subsets[v] = new Subset();
        subsets[v]->representante = v;
        subsets[v]->rank = 0;
        subsets[v]->nodos = std::list<int>();
        subsets[v]->nodos.push_back(v);
        subsets[v]->cantEjes = 0;
    }

    Subset & find(int v) const {
        if (subsets[v]->representante == v) return *subsets[v];
        else find(subsets[v]->representante);
    }

    Subset& unify(const Eje & eje) {
        Subset &srcSet = find(eje.origen);
        Subset &destSet = find(eje.destino);

        if (srcSet.rank > destSet.rank) {
            if (srcSet.representante != destSet.representante){
                srcSet.nodos.splice(srcSet.nodos.begin(), destSet.nodos);
                srcSet.cantEjes += destSet.cantEjes;
                destSet.representante = srcSet.representante;
            }
            srcSet.cantEjes++;
            return srcSet;
        } else if (srcSet.rank < destSet.rank) {
            if (srcSet.representante != destSet.representante){
                destSet.nodos.splice(destSet.nodos.begin(), srcSet.nodos);
                destSet.cantEjes += srcSet.cantEjes;
                srcSet.representante = destSet.representante;
            }
            destSet.cantEjes++;
            return destSet;
        } else {
            if (srcSet.representante != destSet.representante){
                destSet.nodos.splice(destSet.nodos.begin(), srcSet.nodos);
                destSet.cantEjes += srcSet.cantEjes;
                srcSet.representante = destSet.representante;
                destSet.rank++;
            }
            destSet.cantEjes++;
            return destSet;
        }
//        totalEdges.push_back(eje);
    };

    std::list<Subset*> sets() const {
        bool seen [cantNodos] = {false};

        std::list<Subset*> ss;
        for (int i = 0; i < cantNodos; ++i) {
            Subset &subset = find(i);
            if (!seen[subset.representante]) {
                seen[subset.representante] = true;
                ss.push_back(&subset);
            }
        }
        return ss;
    }

    friend std::ostream& operator<<(std::ostream& os, const DisjointSet& d){
        std::list<Subset*> subsets = d.sets();
        os << "[DisjointSet] cantNodos: "<< d.cantNodos << ", " << subsets.size() << " subsets: " << std::endl;
        for (std::list<Subset*>::const_iterator it = subsets.begin(); it != subsets.end(); it++) {
            os << "--> " << **it << std::endl;
        }
        return os;
    }

private:

    int cantNodos;
//    std::list<Eje> totalEdges;
    Subset ** subsets;

};


#endif //ALGO3_TP3_CMF_DISJOINTSET_H
