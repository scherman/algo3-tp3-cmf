#include "grasp.h"

using namespace std;

typedef long long int64;
//int const MAXN = 1000;


//int adyacenciacantidad;
//int matriz[MAXN][MAXN];
//int x,y;
//int mejortotal;

char* getCmdOption(char ** begin, char ** end, const string & option)
{
    char ** itr = find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return 0;
}

bool cmdOptionExists(char** begin, char** end, const string& option)
{
    return find(begin, end, option) != end;
}

struct mayor
{
    template<class T>
    bool operator()(T const &a, T const &b) const { return a > b; }
};

double randomizedgreedy(std::vector<std::vector<bool>> &matriz, int mejorescandidatos, int n, int m, int incluido[], int &adyacenciacantidad) //para hacer greedy normal llamarlo con el parametro en 1
{
    int agrandar=1;
    for (int z=0;z<n;z++)
    {
        incluido[z]=false;
    }
    int mejortotal=0;
    int numerodebucles=0;
    while (agrandar==1)
    {
        agrandar=0;

        //criterio de seleccion en el RCL. Para armar el RCL voy a usar un sort donde ordeno la lista de elementos desde el que tiene mas nodos adyacentes al resto al que tiene menos.
        vector <pair<int,int>> RCL;
        for (int z=0; z<n; z++)
        {
            if (incluido[z]==false)
            {

                bool adyacenteatodos=true;
                for (int i=0;i<n;i++)
                {
                    if ((incluido[i]==true)&&(matriz[i][z]==0))
                    {
                        adyacenteatodos=false;
                        break;
                    }
                }
                if (adyacenteatodos==true)
                {
                    adyacenciacantidad=0;
                    for (int i=0;i<n;i++)
                    {
                        if (((incluido[i]==false)&&(i!=z))&&(matriz[i][z]==1)) adyacenciacantidad++;


                    }
                    //ahora toca sumarle el total de la anterior y restarle una cantidad igual al numero de bucles ya que las n aristas del n clique al nuevo vertice antes formaban parte de la frotera y ahora son parte del n+1 clique
                    adyacenciacantidad+=mejortotal;
                    adyacenciacantidad-=numerodebucles;
                    agrandar=1;
                    pair<int,int> foo (adyacenciacantidad, z);
                    RCL.push_back(foo);
                }


            }
        }
        if (agrandar==1)
        {
            sort(RCL.begin(),RCL.end(), mayor() );
            int tamano=RCL.size();
            int aleatorio=rand()%min(tamano,mejorescandidatos);
            mejortotal=RCL[aleatorio].first;
            incluido[RCL[aleatorio].second]=1;
            numerodebucles++;
        }

    }
    return mejortotal;
}

double busquedalocal(std::vector<std::vector<bool>> &matriz, int cap,int greedysolucion, int n, int m, int incluido[], int &adyacenciacantidad)
{
    int limite=0;
    int modificado=1;
    double mejortotal=greedysolucion;
    while ((modificado==1)&&(limite<cap))
    {
        modificado=0;
        int mejoreliminacion;
        int resultadodemejoreliminacion=0;
        int mejorswapeliminar;
        int mejorswapagregar;
        int resultadomejorswap=0;

        //principio de la eliminacion de elementos demas. notese que si o si al eliminar se va a mantener la adyacencia a todos
        for (int z=0;z<n;z++)
        {
            if (incluido[z]==true)
            {
                adyacenciacantidad=mejortotal;

                //cuanta la cantidad de vecinos del vértice eliminado que sumaban a la frontera ya que eso reducira al total. Ademas se aumentar una cantidad igual a la cantidad de elementos del nuevo clique ya que todos esos pasan a la frontera
                for (int i=0;i<n;i++)
                {
                    if ((incluido[i]==false)&&(matriz[i][z]==1)) adyacenciacantidad--;
                }
                //esta es la parte de aumentar por el tamaño de la frontera
                for (int i=0;i<n;i++)
                {
                    if ((incluido[i]==true)&&(i!=z)) adyacenciacantidad++;
                }

                //ahora toca sumarle el total de la anterior y restarle una
                if (adyacenciacantidad>resultadodemejoreliminacion)
                {
                    resultadodemejoreliminacion=adyacenciacantidad;
                    mejoreliminacion=z;
                }

            }
        }
        //fin de la eliminacion de elementos demás
        //principio del swap de 2 elementos

        for (int z1=0;z1<n;z1++)
        {
            if (incluido[z1]==true)
            {
                for (int z2=0;z2<n;z2++)
                {
                    if (incluido[z2]==false)
                    {
                        bool adyacenteatodos=true;
                        for (int i=0;i<n;i++)
                        {
                            if (((incluido[i]==true)&&(i!=z1))&&(matriz[i][z2]==0))
                            {
                                adyacenteatodos=false;
                                break;
                            }
                        }

                        if (adyacenteatodos==true)
                        {
                            adyacenciacantidad=mejortotal;
                            //primero se calcula la cantidad de adyacentes que quedan luego de sacar un elemento
                            //cuanta la cantidad de vecinos del vértice eliminado que sumaban a la frontera ya que eso reducira al total. Ademas se aumentar una cantidad igual a la cantidad de elementos del nuevo clique ya que todos esos pasan a la frontera
                            for (int i=0;i<n;i++)
                            {
                                if ((incluido[i]==false)&&(matriz[i][z1]==1)) adyacenciacantidad--;
                            }
                            //esta es la parte de aumentar por el tamaño de la frontera
                            for (int i=0;i<n;i++)
                            {
                                if ((incluido[i]==true)&&(i!=z1)) adyacenciacantidad++;
                            }

                            //ahora en la nueva matriz debo calcularlas aristas que añádo al agregar un elemento
                            for (int i=0;i<n;i++)
                            {
                                if (((incluido[i]==false)||(i==z1))&&(matriz[i][z2]==1)) adyacenciacantidad++;

                            }
                            //debo calcular la cantidad de aristas que pasaron ahora a ser parte del clique por lo cual se debe restar
                            for (int i=0;i<n;i++)
                            {
                                if (((incluido[i]==true)&&(i!=z1))&&(matriz[i][z2]==1)) adyacenciacantidad--;


                            }


                            if (adyacenciacantidad>resultadomejorswap)
                            {
                                resultadomejorswap=adyacenciacantidad;
                                mejorswapagregar=z2;
                                mejorswapeliminar=z1;
                            }
                        }
                    }
                }
            }
        }


        //fin del swap de 2 elementos
        if (resultadodemejoreliminacion>resultadomejorswap)
        {
            if (resultadodemejoreliminacion>mejortotal)
            {
                modificado=1;
                incluido[mejoreliminacion]=false;
                mejortotal=resultadodemejoreliminacion;
            }
        } else
        {
            if (resultadomejorswap>mejortotal)
            {
                modificado=1;
                incluido[mejorswapeliminar]=false;
                incluido[mejorswapagregar]=true;
                mejortotal=resultadomejorswap;
            }

        }

        limite++;

    }
    return mejortotal;
}

Clique grasp(int n, int m, std::vector<std::vector<bool>> &matriz, int RCL, int iterations, int search){
    int const MAXN = 1000;
    int incluido[MAXN];
    int maxFrontera = -1;
    vector<int> cliqueMaxFrontera;
    int adyacenciacantidad = 0;
    for (int it = 0; it < iterations; it++)
    {
        int guardado = randomizedgreedy(matriz, RCL, n, m, incluido, adyacenciacantidad);
        int fronteraActual = busquedalocal(matriz, search,guardado,n,m, incluido, adyacenciacantidad);

        if (fronteraActual > maxFrontera)
        {
            cliqueMaxFrontera.clear();
            for (int i = 0; i < n; ++i)
            {
                if (incluido[i] == 1)
                {
                    cliqueMaxFrontera.push_back(i);
                }
            }
            maxFrontera = fronteraActual;
        }
    }

    std::list<int> v;
    for (vector<int>::iterator cliqueIt = cliqueMaxFrontera.begin(); cliqueIt != cliqueMaxFrontera.end(); ++cliqueIt)
    {
        v.push_back(*cliqueIt);
    }
    v.push_back(1);
    return Clique(v, maxFrontera);
};
