#ifndef STRING_MAP_STRING_MAP_H
#define STRING_MAP_STRING_MAP_H

#include <cassert>
#include <string>
#include <vector>
#include <array>
#include <climits>
#include <algorithm>
#include <type_traits>
#include "linear_set.h"

using std::string;
using std::vector;
using std::pair;

/**
 * Implementacion de map<string,T> sobre Trie
 * Asume de T:
 * - tiene constructor por copia (con complejidad copy(T))
 * - tiene operador == (con complejidad cmp(T))
 * - solo permite utilizar el operator[] si T tiene constructor por defecto
 */
template < typename T >
class string_map {
public:
    //static_assert(std::is_copy_constructible< typename std::decay<T>::type >::value, "T must be copy constructible");
    //region types

    typedef string key_type;
    typedef T mapped_type;
    typedef std::pair<const key_type, mapped_type> value_type;
    typedef size_t size_type;

    /**
     * Foward declaration
     */
    class iterator;
    class const_iterator;
    class output_iterator;
    /**
     * Hack necesario para que CLion / Windows no arroje warning
     */
    using iterator = iterator;
    using const_iterator = const_iterator;
    using output_iterator = output_iterator;
    //endregion

    //region method
    /** @brief Construye mapa vacio
     *
     * \pre true
     * \post \P{this} = vacio
     *
     * \complexity{\O(1)}
     */
    string_map();

    /** @brief Destruye mapa
     *
     * \pre true
     * \post el diccionario deja de existir y se libera toda la memoria reservada por el mismo
     *
     * \complexity{\O(sn * S)}
     */
    ~string_map();

    /** @brief Constructor por copia
     *
     * \pre true
     * \post \P{this} es una copia de otro. No hay aliasing
     *
     * \complexity{\O(sn * (S + \DEL(T)))}
     */
    string_map(const string_map & otro);

    /** @brief Operador de asignacion
     *
     * \pre true
     * \post \P{res} = \P{this}
     *
     * \complexity{\O(sn * (S + copy(T)))}
     */
    string_map& operator=(const string_map &);

    /** @brief Devuelve true si los diccionarios son iguales
     *
     * \pre true
     * \post \P{res} = (\P{this} = otro)
     *
     * \complexity{\O(sn * (S + \CMP(T)))}
     */
    bool operator==(const string_map& otro) const;

    /** @brief Devuelve true si los diccionarios son distintos
     *
     * \pre true
     * \post \P{res} = \LNOT (\P{this} = otro)
     *
     * \complexity{\O(sn * (S + \CMP(T)))}
     */
    bool operator!=(const string_map& otro) const {return !(*this == otro);}

    /** @brief Cantidad de apariciones de la clave (0 o 1)
     *  @param key clave a buscar
     *
     *  \pre true
     *  \post \IF def?(key, \P{this}) \THEN \P{res} = 1 \ELSE \P{res} = 0 \FI
     *
     *  \complexity{\O(S)}
     */
    size_type count(const key_type &key) const;

    /** @brief Devuelve cantidad de claves definidas
     *
     * \pre true
     * \post \P{res} = #(claves(\P{this}))
     *
     * \complexity{\O(1)}
     */
    size_t size() const;

    /** @brief devuelve true si size() == 0
     *
     * \pre true
     * \post \P{res} = (#(claves(\P{this})) = 0)
     *
     * \complexity{\O(1)}
     */
    bool empty() const;


    /** @brief Acceso / definición de pares clave/valor
     *  @param key clave a acceder, si no existe, se crea
     *  @returns una referencia a la definicion.
     *
     *  \pre d = \P{this}
     *  \post \IF def?(key, d) \THEN \P{this} = d \ELSE \P{this} = (definir(key, M, d)) \FI)
     *
     *  \complexity{\O(S)}
     */
    mapped_type &operator[](const key_type &key);

    /** @brief Acceso a una clave sin modificar mapa
     *  @param key clave a acceder que debe existir previamente
     *  @returns una referencia a la definicion.
     *
     *  \pre def?(key, \P{this})
     *  \post \P{res} = obtener(key, \P{this})
     *
     *  \complexity{\O(S)}
     */
    mapped_type& at(const key_type& key);

    /** @brief Acceso a una clave sin modificar mapa
     *  @param key clave a acceder que debe existir previamente
     *  @returns una referencia const a la definicion.
     *
     *  \pre def?(key, \P{this})
     *  \post \P{res} = obtener(key, \P{this})
     *
     *  \complexity{\O(S)}
     */
    const mapped_type& at(const key_type& key) const;

    /** @brief Vacia el mapa
     *
     * \pre true
     * \post \P{this} = vacio
     *
     * \complexity{\O(sn * (S + delete(T)))}
     */
    void clear();

    // Accesos con iteradores

    /** @brief iterador al primer par <clave,significado> en orden lexicografico
     *  @returns iterador al elemento o end() si el mapa era vacio
     *
     *  \pre true
     *  \post \P{res} apunta al principio del diccionario
     *
     *  \complexity{\O(S)}
     */
    iterator begin();

    /*  @brief iterador al fin de la coleccion
     *
     *  \pre true
     *  \post \P{res} apunta a la posición pasando-el-último
     *
     *  \complexity{\O(S)}
     */
    iterator end();

    /// Versiones const de begin/end
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;

    /** @brief busca una clave
     *  @param key clave a buscar
     *  @returns un iterador al par <clave, significado>
     *
     *  \pre def?(key, \P{this})
     *  \post \IF def?(key, \P{this}) \THEN \P{res} apunta a obtener(key, \P{this}) \ELSE \P{res} apunta a la posición pasando-al-último \FI
     *
     *  \complexity{\O(S)}
     */
    iterator find(const key_type &key);

    /** @brief busca una clave
     *  @param key clave a buscar
     *  @returns un iterador const al par <clave, significado>
     *
     *  \pre true
     *  \post \IF def?(key, \P{this}) \THEN \P{res} apunta a obtener(key, \P{this}) \ELSE \P{res} apunta a la posición pasando-al-último \FI
     *
     *  \complexity{\O(S)}
     */
    const_iterator find(const key_type &key) const;

    /** @brief insercion
     *
     * @param value par <clave,significado> a insertar
     * @returns un par con un iterador al par clave-significado agregado o
     * modificado y un bool que indica si la clave se insertó como una clave
     * nueva.
     *
     * \pre d = \P{this}
     * \post \P{this} = definir(\P1(value), \P2(value), d) \LAND \P2(\P{res}) = \LNOT def?(\P1(value), d) \LAND \P1(\P{res}) apunta a obtener(\P1(value), \P{this})
     *
     * \complexity{\O(S + copy(value_type))}
     */
    pair<iterator, bool> insert(const value_type &value);

    /** @brief insercion rapida
     *
     * @param value par <clave,significado> a insertar
     * @returns un iterador al par clave-significado agregado o
     * modificado y un bool que indica si la clave se insertó como una clave
     * nueva.
     *
     * \pre d = \P{this}
     * \post \P{this} = definir(\P1(value), \P2(value), d) \LAND \P2(\P{res}) = \LNOT def?(\P1(value), d) \LAND \P1(\P{res}) apunta a obtener(\P1(value), \P{this})
     *
     * \complexity{\O(S + copy(value_type))}
     */
    iterator fast_insert(const value_type& value);


    /** @brief eliminar una clave
     *  @param key clave a eliminar
     *  @returns cantidad de elementos eliminados
     *
     *  \pre d = \P{this} \LAND def?(key, d)
     *  \post \IF def?(key, d) \THEN (\P{this} = borrar(key, d) \LAND \P{res} = 1) \ELSE (\P{this} = d \LAND \P{res} = 0) \FI
     *
     *  \complexity{\O(S + delete(T))}
     */
    size_type erase(const key_type& key);

    /** @brief eliminar una clave mediante irerador
     *  @param pos iterador apuntando a clave a eliminar
     *  @returns iterador apuntando el proximo de la clave eliminada (o end() si era la ultima)
     *
     *  \pre el iterador no debe estar en la posición pasando-el-último y no debe haberse invalidado
     *  \post borra la clave del diccionario apuntada por el iterador, invalidando todos los iteradores que la apuntan. Devuelve un iterador apuntando a la clave siguiente (o a la posición pasando-el-último, si era la última)
     *
     *  \complexity{\O(S)}
     */
    iterator erase(iterator pos);
    //endregion
    /**
     * Imprime el diccionario, exponiendo las claves de manera ordenada lexicograficamente.
     * pre U DEBE tener sobrecargado el operador <<
     * post muestra por pantalla
     * complejidad O (ns * (S + mostrar(string_map)) )
     */
    friend ostream& operator<<(ostream& os, const string_map<T> &map){
        //region O = O(ns * (L + mostrar(T))
        //region O = O(1)
        os << "[";
        //endregion
        //region O = O(ns * (L + mostrar(T))
        std::for_each(begin(), end(), [&](const value_type & value){
            //region O = O(L) + O(mostrar(T))
            //region O = O(1)
            os << "{ ";
            //endregion

            //region O = O(L)
            os << value.first;
            //endregion

            //region O = O(1)
            os << ", ";
            //endregion

            //region O = O(mostrar(T))
            os << value.second;
            //endregion

            //region O = O(1)
            os << " }";
            //endregion
            //endregion
        });
        //endregion
        //region O = O(1)
        os << "]";
        //endregion
        return os;
        //endregion
    };
private:
    struct Node {
        /**
         *
         * \name Representación:
         *
         * rep: nodo(T) \TO bool\n
         * rep(n) \EQUIV
         *  * isLeaf = \FORALL (i : Nat) 0 \LEQ i \LEQ 127 \IMPLIES_L children[i] = NULL
         *  * parent != NULL \LOR_L &n = parent.children[index]
         *  * isLead != false \LAND_L count_children = cantidadSignificados(&n)
         *  * pair != null \LOR_L (pair.first = key \LAND pair.second = *element)
         */

        /**
         * Indica si el nodo es hoja. Un nodo es hoja cuando no tiene ningun hijo, es decir cuando todos los punteros de
         * children son null. Lo que no quiere decir que si se definio una clave y luego se elimino esto siga siendo nulo.
         */
        bool isLeaf;
        /**
         * Referencia al nodo padre
         */
        Node* parent;
        /**
         * Par ordenado que tiene la clave/valor del nodo
         */
        value_type * pair;
        /**
         * Indice que tiene el nodo en el padre
         */
        size_t index;
        /**
         * Referencia al elemento que contiene el nodo
         */
        mapped_type *element;
        /**
         * Referencia a la clave que tiene el nodo
         */
        const key_type *key;
        /**
         * Array que contiene los punteros a todos los nodos que son parte del trie. Tener en cuenta que cada indice del
         * nodo es una letra en ASCII
         */
        std::array<Node *, CHAR_MAX> children;
        /**
         * Cuenta la cantidad de hijos que tiene el nodo DEFINIDOS. Es decir, si se definieron 4 nodos y luego se eliminaron
         * 3, este tendra un valor de 1. Esto es util, ya que al momento de eliminar un hijo, no se eliminan los nodos
         * asociados al mismo.
         */
        size_t count_children;
        /**
         * @brief Constructor del nodo, inicializa todos sus valores a nulos.
         * \complexity{\O(1)}
         */
        Node();
        /**
         * @bried Destructor del nodo, en caso de que tenga elementos para los cuales fueron reservados memoria de manera
         * dinamica, los elimina.
         * \complexity{\O(sn * S)}
         */
        ~Node();
        /**
         * @brief Dado un nodo, devuelve el sucesor del mismo, el sucesor se obtiene mirando de izquierda a derecha los
         * candidatos como hijos y en caso de no tener, se empieza a mirar al padre del nodo y tomar el proximo sucesor
         * del nodo pasado como parametro.
         * @param node Nodo a buscar el proximo hijo definido
         * \complexity{\O(S)}
         */
        static Node * sucesor(Node *node);
        /**
         * @brief Dado un nodo, devuelve el proximo hijo definido
         * @param node Nodo a buscar el proximo hijo definido
         * \complexity{\O(S)}
         */
        static Node* nextChild(Node *node);
    };

    /**
     * \name Representación:
     *
     * rep: string_map(Significado) \TO bool\n
     * rep(d) \EQUIV
     *  * (root = NULL \LAND length = 0) \LOR_L (arbolConsistente(root) \LAND length = cantidadSignificados(root))
     *
     * arbolConsistente : Nodo* \RIGHTARROW bool
     * arbolConsistente(nodo) \EQUIV \IF nodo = NULL \THEN true \ELSE Rep(*nodo) \LAND arbolConsistenteAux(*nodo.children, 0) \FI
     *
     * arbolConsistenteAux : secu(Nodo*) \TIMES Nat n \RIGHTARROW bool { 0 \LEQ n }
     * arbolConsistenteAux(s, i) \EQUIV \IF i \GEQ 128 \THEN true \ELSE arbolConsistente(s[i]) \FI
     *
     * cantidadSignificados : Nodo* \RIGHTARROW Nat
     * cantidadSignificados(nodo) \EQUIV \IF nodo = NULL \THEN 0 \ELSE \IF *nodo.elem != NULL 1 \ELSE 0 \FI + cantidadSignificadosAux(*nodo.children, 0) \FI
     *
     * cantidadSignificadosAux : secu(Nodo*) \TIMES Nat n \RIGHTARROW Nat { 0 \LEQ n }
     * cantidadSignificadosAux(s, i) \EQUIV \IF i \GEQ 128 \THEN 0 \ELSE cantidadSignificados(s[i]) \FI
     *
     * abs: string_map(Significado) \TO Dicc(string, Significado)\n
     * abs(d) \EQUIV d' \|
     *  * \FORALL (c : string) def?(c, d') = \LNOT \EMPTYSET?(buscarEnTrie(c, root))
     *  * \LAND_L obtener(c, d') = dameUno(buscarEnTrie(c, root))
     *
     * TAD string es secu<char>
     *  buscarEnTrie : string \TIMES Node* \RIGHTARROW Significado
     *  buscarEnTrie(str, nodo) \EQUIV \IF vacía(str) \THEN \EMPTYSET \ELSE \IF nodo\RIGHTARROWchildren[prim(s)]\RIGHTARROWelement = nullptr \THEN \EMPTYSET \ELSE \IF \EMPTYSET?(fin(str)) \THEN Ag(*nodo\RIGHTARROWelement, \EMPTYSET) \ELSE buscarEnTrie(fin(str), nodo\RIGHTARROWchildren[prim(str)]) \FI \FI \FI \FI
     */


    /**
     * Raiz del arbol
     */
    Node* root = nullptr;
    /**
     * Dado una nueva clave, crea un nuevo nodo desde la raiz y devuelve el puntero al nodo.
     */
    Node* createNode(const key_type &value);
    /**
     * Dado una clave, busca ese nodo desde la raiz y devuelve el puntero al nodo.
     */
    Node* findNode(const key_type &clave);
    /**
     * Cantidad de elementos que contiene el arbol
     */
    size_type length = 0;
};

#include "string_map.hpp"
#include "string_map_iterators.h"
#include "string_map_nodo.h"

#endif //STRING_MAP_STRING_MAP_H
