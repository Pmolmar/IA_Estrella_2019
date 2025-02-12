#include "tree_t.hpp"

tree_t::tree_t(std::vector<float> A, std::vector<float> B, int C, int D) : ini(C),
                                                                           fin(D),
                                                                           aristas(0)
{
    map(A, B);
    arbol();
}

tree_t::~tree_t()
{
}

void tree_t::map(std::vector<float> A, std::vector<float> B)
{
    int aux = 1;
    n = A[0];

    //redimension de matriz de costes
    costes.resize(n);

    for (int i = 0; i < n; i++)
        costes[i].resize(n);

    // mapeo de costes
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; ++j)
        {
            costes[i][j] = A[aux];
            costes[j][i] = A[aux];
            aux++;
            if (A[aux] != -1)
                aristas++;
        }
        costes[i][i] = -1;
    }

    //mapeo de heuristicos
    for (int i = 0; i < n; ++i)
        heur.push_back(B[i + 1]);
}

//crea el nodo raiz, lo inspecciona y da paso al algoritmo principal
void tree_t::arbol()
{
    node_t *aux;
    aux = new node_t(0, ini, 0, heur[ini]);

    nodos.push_back(aux);
    aux->set_papa(aux);
    camino(aux);
}

void tree_t::camino(node_t *A)
{
    if (A->get_id() != fin)
    {
        if (!ready(A))
        {
            std::cout << std::endl
                      << " Nodo: " << A->get_id() + 1;
            inspec.push_back(A);
            generate_sons(A);

            camino(search());
        }
    }
    else
    {
        inspec.push_back(A);
    }
}

//comprobar que el nodo A no esta inspeccionado
bool tree_t::ready(node_t *A)
{
    for (unsigned int i = 0; i < inspec.size(); i++)
    {
        if ((inspec[i]->get_id() == A->get_id()) && (inspec[i]->get_val() == A->get_val()))
            return true;
    }
    return false;
}

//genera los hijos de un nodo A
void tree_t::generate_sons(node_t *A)
{
    for (int i = 0; i < n; ++i)
    {
        float x = costes[A->get_id()][i];

        if ((x != -1) && (!A->antecesor(i)))
        {
            node_t *nodo;
            nodo = new node_t(A->get_cost() + x, i, A->get_prof() + 1, heur[i]);

            nodo->set_papa(A);
            nodos.push_back(nodo);
            std::cout << " g:" << nodo->get_id() + 1 << "(" << nodo->get_val() << ") ";
            A->set_son(nodo);
        }
    }
}

//busca el nodo con menor coste que no este inspeccionado
node_t *tree_t::search()
{
    node_t *aux;
    node_t *best = NULL;

    for (unsigned int i = 0; i < nodos.size(); ++i)
    {
        aux = nodos[i];

        if ((best == NULL) && (!ready(aux)))
            if ((aux->get_val()) < 9999)
                best = aux;
        if ((!ready(aux)) && (min(aux, best)))
            best = aux;
    }

    return best;
}

//devuelve si los costes totales de A son menores a B
bool tree_t::min(node_t *A, node_t *B)
{
    if (A->get_val() < B->get_val())
        return true;
    else
        return false;
}

void tree_t::mostrar()
{
    bool i = true;
    node_t *aux = inspec[inspec.size() - 1];

    //system("clear");
    std::cout << "-----------------------------" << std::endl;
    std::cout << "Nodos: " << n << std::endl;
    std::cout << "Aristas: " << aristas << std::endl;
    std::cout << "Inicio: " << ini + 1 << std::endl;
    std::cout << "Fin: " << fin + 1 << std::endl;
    std::cout << "Costes: " << inspec[inspec.size() - 1]->get_cost() << std::endl;
    std::cout << "Nodos Generados: " << nodos.size() - 1 << std::endl;
    std::cout << "Nodos Inspeccionados: " << inspec.size() - 1 << std::endl;
    std::cout << "Camino: ";

    while (i)
    {
        if (aux->get_id() != ini)
        {
            std::cout << aux->get_id() + 1 << " --> ";
            aux = aux->get_papa();
        }
        else
        {
            std::cout << aux->get_id() +1 << std::endl;
            i = false;
        }
    }
    std::cout << std::endl;
}