#ifndef string_map_iterators_h
#define string_map_iterators_h

#include "string_map.h"

template <typename T >
class string_map<T>::output_iterator {
public:
    using value_type = const string_map<T>::value_type;
    using iterator_category = std::output_iterator_tag;
    using reference = value_type &;
    using pointer = value_type *;
    using difference_type = std::ptrdiff_t;


    output_iterator(string_map<T> *tree){
        this->tree = tree;
    }
    /**
     * It must be copy-constructible
     */
    output_iterator(const output_iterator & other){
        this->tree = other.tree;
    }
    /**
     * It must be move-constructible (Lvalues are swappable. Since C++ 11)
     */
    output_iterator(const output_iterator && other){
        this->tree = other.tree;
    }
    /**
     * It must be copy-assignable
     */
    output_iterator operator=(const output_iterator &other){
        this->tree = other.tree;
    };
    /**
     * It must be move-assignable (Lvalues are swappable. Since C++ 11)
     */
    output_iterator operator=(const output_iterator &&other){
        this->tree = other.tree;
    };
    /**
     * HYPER PROXY
     */
    output_iterator& operator=(const value_type &value){
        this->tree->insert(value);
        return *this;
    };
    /**
     * Can be incremented.
     */
    output_iterator &operator++(){
        /**
         * No me interesa que devuelve, ya que el concepto de incrementar en mi diccionario no tiene mayor importancia,
         * ya que arranque con el contenedor vacio, por lo tanto el "siguiente" no existe.
         */
        return *this;
    }
    /**
     * Can be incremented.
     */
    output_iterator &operator++(int){
        /**
         * No me interesa que devuelve, ya que el concepto de incrementar en mi diccionario no tiene mayor importancia,
         * ya que arranque con el contenedor vacio, por lo tanto el "siguiente" no existe.
         */
        return *this;
    }
    /**
     * Can be dereferenced as an lvalue (if in a dereferenceable state). It shall only be dereferenced as the left-side
     * of an assignment statement. Once dereferenced, its iterator value may no longer be dereferenceable
     */
    output_iterator& operator*(){
        return *this;
    }
private:
    friend class string_map<T>;
    friend class string_map<T>::iterator;
    string_map<T> *tree;
};

template <typename T >
class string_map<T>::iterator {

public:
    using value_type = const string_map<T>::value_type;
    using iterator_category = std::input_iterator_tag;
    using reference = value_type &;
    using pointer = value_type *;
    using difference_type = std::ptrdiff_t;
    /**
     * @brief Constructor del iterador.
     *
     * \pre ninguna
     * \post ninguna
     *
     * \complexity{\O(S)}
     */
    iterator(Node * root){
        //region O = O(1) + max(O(1), O(S))
        if(root != nullptr){
            //region O = O(1) + max(O(1), O(S)) = O(S)
            if(root->element != nullptr){
                this->actual = root;
            } else {
                this->actual = Node::nextChild(root);
            }
            //endregion
        } else {
            this->actual = nullptr;
        }
        //endregion
    };
    /**
     * @brief Constructor por copia del iterador.
     *
     * \pre ninguna
     * \post pisa los datos existentes por el parametro pasado
     *
     * \complexity{\O(1)}
     */
    iterator(const iterator &other){
        this->actual = other.actual;
    };
    /**
     * @brief Asignacion de un iterador a otro
     *
     * \pre ninguna
     * \post genera una nueva instancia
     *
     * \complexity{\O(1)}
     */
    iterator& operator=(const iterator &other){
        this->actual = other.actual;
    };
    /**
     * @brief Destructor del iterador
     *
     * \pre el objeto debe estar vivo
     * \post el objeto es destruido
     *
     * \complexity{\O(1)}
     */
    ~iterator() {
        this->actual = nullptr;
    };
    /**
     * @brief Indica si dos iteradores son iguales
     *
     * \pre Los iteradores estan en la misma coleccion
     * \post True sii los iteradores apuntan al mismo elemento
     *
     * \complexity{\O(1)}
     */
    bool operator==(const iterator &other) const {
        return this->actual == other.actual;
    };
    /**
     * @brief Indica si dos iteradores son distintos
     *
     * \pre Los iteradores estan en la misma coleccion
     * \post True sii los iteradores no apuntan al mismo elemento
     *
     * \complexity{\O(1)}
     */
    bool operator!=(const iterator &other) const {
        //region O = O(1) + O(1) + O(1) = O(1)
        return !(*this == other);
        //endregion
    };
    /**
     * @brief Indica si dos iteradores son distintos
     *
     * \pre El iterador no esta en la posición pasando-el-último
     * \post El valor resultado es una referencia al valor apuntado.
     *
     * \complexity{\O(1)}
     */
    value_type& operator*(){
        if(this->actual->pair->second != *this->actual->element){
            this->actual->pair->second = *this->actual->element;
        }
        return *this->actual->pair;
    };
     /**
     * @brief Operador flechita
     *
     * El valor devuelvo tiene aliasing dentro de la colección.
     *
     * \pre El iterador no debe estar en la posición pasando-el-último.
     * \post El valor resultado es un puntero al valor apuntado.
     *
     * \complexity{\O(1)}
     */
    value_type* operator->(){
        if(this->actual->pair->second != *this->actual->element){
            this->actual->pair->second = *this->actual->element;
        }
        return this->actual->pair;
    };
    /**
     * @brief Operador siguiente
     * Avanza el iterador una posicion
     *
     * \pre El iterador no debe estar en la posición pasando-el-último.
     * \post \P{res} es una referencia a \P{this}. \P{this} apunta a la posición siguiente.
     *
     * \complexity{\O(1)}
     */
    iterator operator++(){
        this->actual = Node::sucesor(this->actual);
        return iterator(this->actual);
    };
    /**
     * @brief Operador siguiente
     * Avanza el iterador la cantidad de posiciones indicada por el parámetro
     *
     * \pre El iterador no debe estar en una posición tal que la posicion actual + i no pase el último.
     * \post \P{res} es una referencia a \P{this}. \P{this} apunta a la posición actual + i.
     *
     * \complexity{\O(1)}
     */
    iterator operator++(int i) {
        this->actual = Node::sucesor(this->actual);
        return iterator(this->actual);
    };
private:
    friend class string_map<T>::const_iterator;
    friend class string_map<T>;
    Node * actual;
};

template < class T >
class string_map<T>::const_iterator {

public:
    using value_type = const string_map<T>::value_type;
    using iterator_category = std::input_iterator_tag;
    using reference = value_type &;
    using pointer = value_type *;
    using difference_type = std::ptrdiff_t;
    /**
     * constructor
     */
    const_iterator(Node * root){
        if(root != nullptr){
            if(root->element != nullptr){
                this->actual = root;
            } else {
                this->actual = Node::nextChild(root);
            }
            if(this->actual != nullptr){
                if(this->actual->pair->second != *this->actual->element){
                    this->actual->pair->second = *this->actual->element;
                }
            }
        } else {
            this->actual = nullptr;
        }
    };
    /**
     * copy-constructible
     */
    const_iterator(const iterator &other){
        this->actual = other.actual;
    };
    /**
     * copy-constructible
     */
    const_iterator(const const_iterator &other){
        this->actual = other.actual;
    };
    /**
     * copy-assignable
     */
    const_iterator& operator=(const const_iterator &other){
        this->actual = other.actual;
    };
    /**
     * destructible
     */
    ~const_iterator() {
        this->actual = nullptr;
    };
    /**
     * @brief Indica si dos iteradores son iguales
     *
     * \pre Los iteradores estan en la misma coleccion
     * \post True sii los iteradores apuntan al mismo elemento
     *
     * \complexity{\O(1)}
     */
    bool operator==(const typename string_map<T>::const_iterator &other) const {
        return this->actual == other.actual;
    };
    /**
     * @brief Indica si dos iteradores son diferentes
     *
     * \pre Los iteradores estan en la misma coleccion
     * \post True sii los iteradores no apuntan al mismo elemento
     *
     * \complexity{\O(1)}
     */
    bool operator!=(const typename string_map<T>::const_iterator &other) const {
        return !(*this == other);
    };
    /**
     * @brief Indica si dos iteradores son distintos
     *
     * \pre El iterador no esta en la posición pasando-el-último
     * \pos El valor resultado es una referencia al valor apuntado.
     *
     * \complexity{\O(1)}
     */
    const value_type& operator*() const {
        return *this->actual->pair;
    };
    /**
    * @brief Operador flechita
    *
    * El valor devuelto tiene aliasing dentro de la colección.
    *
    * \pre El iterador no debe estar en la posición pasando-el-último.
    * \post El valor resultado es un puntero al valor apuntado.
    *
    * \complexity{\O(1)}
    */
    const value_type* operator->() const {
        return this->actual->pair;
    };
    /**
     * @brief Operador siguiente
     * Avanza el iterador una posicion
     *
     * \pre El iterador no debe estar en la posición pasando-el-último.
     * \post \P{res} es una referencia a \P{this}. \P{this} apunta a la posición siguiente.
     *
     * \complexity{\O(1)}
     */
    const_iterator operator++(){
        this->actual = Node::sucesor(this->actual);
        if(this->actual == nullptr){
            return const_iterator(this->actual);
        }
        if(this->actual->pair->second != *this->actual->element){
            this->actual->pair->second = *this->actual->element;
        }
        return const_iterator(this->actual);
    };
    /**
     * @brief Operador siguiente
     * Avanza el iterador la cantidad de posiciones indicada por el parámetro
     *
     * \pre El iterador no debe estar en una posición tal que la posición actual + i no pase el último.
     * \post \P{res} es una referencia a \P{this}. \P{this} apunta a la posición actual + i.
     *
     * \complexity{\O(1)}
     */
    const_iterator operator++(int i) {
        this->actual = Node::sucesor(this->actual);
        if(this->actual == nullptr){
            return const_iterator(this->actual);
        }
        if(this->actual->pair->second != *this->actual->element){
            this->actual->pair->second = *this->actual->element;
        }
        return const_iterator(this->actual);
    };
private:
    friend class string_map<T>;
    Node * actual;
};


#endif //string_map_iterators_h
