#ifndef TP2_STRING_MAP_ITERATORS_H
#define TP2_STRING_MAP_ITERATORS_H

#include "string_map.h"
#include <cassert>
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::pair;


template<class T>
class string_map<T>::const_iterator {
public:
    using value_type = string_map::value_type;
    using iterator_category = std::forward_iterator_tag;
    using reference = value_type &;
    using pointer = value_type *;
    using difference_type = std::ptrdiff_t;


    /**
     * @brief Constructor por copia del iterador.
     *
     * \complexity{\O(?)}
     */
    const_iterator(const typename string_map<T>::const_iterator&);

    /**
     * @brief Conversión desde iterator
     *
     * \complexity{\O(?)}
     */
    const_iterator(const typename string_map<T>::iterator&);

    /**
     * @brief Avanza el iterador una posición.
     *
     * \pre El iterador no debe estar en la posición pasando-el-último.
     * \post \P{res} es una referencia a \P{this}. \P{this} apunta a la posición
     * siguiente.
     *
     * \complexity{\O(?)}
     */
    string_map::const_iterator &operator++() {
        // El iterador esta parado en un nodo cuyo significado no es nullptr
        // Primero tengo que chequear que el array del nodo actual esta vacio (caso de palabras clave: pelo y pelota)
        // sino tengo que seguir hacia abajo por esa rama del trie

        int i;
        for (i=0; i < 256 && _node->_children[i] == nullptr; i++);
        if (i<256) {
            // Tengo que seguir hacia abajo
            _clave += char(i);
            _node = child[i];
            // Tengo que chequear si tiene significado, si es asi me quedo ahi y sino sigo para abajo y sigue igual
        } else {
            // El array children esta completo de nullptr asi que tengo que subir
            _node = _node->_padre;
            const char lastChar = _clave.at(_clave.size()-1);
            i = lastChar;
            child = *(_node->_children);
            for (++i; i < 256 && child[i] == nullptr; i++);



        }

    };

    /**
     * @brief Desreferencia el puntero
     *
     * El valor devuelto tiene aliasing dentro de la colección.
     *
     * \pre El iterador no debe estar en la posición pasando-el-último.
     * \post El valor resultado es una referencia al valor apuntado.
     *
     * \complexity{\O(?)}
     */
    const value_type &operator*() const {
        T significado = *(_node->_significado);
        const string clave = _clave;
        value_type res=std::make_pair(clave,significado);
        return res;
    };    //en lugar de const value_type & podría usar (chequear) const reference

    /**
     * @brief Operador flechita
     *
     * El valor devuelvo tiene aliasing dentro de la colección.
     *
     * \pre El iterador no debe estar en la posición pasando-el-último.
     * \post El valor resultado es un puntero al valor apuntado.
     *
     * \complexity{\O(?)}
     */
    const value_type* operator->() const;   //en lugar de const value_type * podría usar (chequear) const pointer

    /**
     * @brief Comparación entre iteradores
     *
     * \pre ambos iteradores refieren a la misma colección
     * \post true sii los iteradores apuntan al mismo elemento
     *
     * \complexity{\O(?)}
     */
    bool operator==(const string_map<T>::iterator &other) const;

    /**
     * @brief Comparación entre iteradores
     *
     * \pre ambos iteradores refieren a la misma colección
     * \post true sii los iteradores no apuntan al mismo elemento
     *
     * \complexity{\O(?)}
     */
    bool operator!=(const string_map<T>::iterator &other) const;



private:
    friend class string_map;

    const_iterator(const typename string_map<T>::const_iterator&);

    Node* _node;
    string _clave;

    //typename string_map<T>::const_iterator it;
};

template<class T>
class string_map<T>::iterator {
public:
    using value_type = const string_map::value_type;
    using iterator_category = std::forward_iterator_tag;
    using reference = value_type &;
    using pointer = value_type *;
    using difference_type = std::ptrdiff_t;

    /**
     * @brief Constructor por copia del iterador.
     *
     * \complexity{\O(?)}
     */
    iterator(const typename string_map<T>::iterator&);

    /**
     * @brief Avanza el iterador una posición.
     *
     * \pre El iterador no debe estar en la posición pasando-el-último.
     * \post \P{res} es una referencia a \P{this}. \P{this} apunta a la posición
     * siguiente.
     *
     * \complexity{\O(?)}
     */
    string_map::iterator &operator++();

    /**
     * @brief Desreferencia el puntero
     *
     * El valor devuelto tiene aliasing dentro de la colección.
     *
     * \pre El iterador no debe estar en la posición pasando-el-último.
     * \post El valor resultado es una referencia al valor apuntado.
     *
     * \complexity{\O(?)}
     */
    const value_type &operator*() const;

    /**
     * @brief Operador flechita
     *
     * El valor devuelvo tiene aliasing dentro de la colección.
     *
     * \pre El iterador no debe estar en la posición pasando-el-último.
     * \post El valor resultado es un puntero al valor apuntado.
     *
     * \complexity{\O(?)}
     */
    const value_type* operator->() const;

    /**
     * @brief Comparación entre iteradores
     *
     * \pre ambos iteradores refieren a la misma colección
     * \post true sii los iteradores apuntan al mismo elemento
     *
     * \complexity{\O(?)}
     */
    bool operator==(const string_map<T>::iterator &other) const;

    /**
     * @brief Comparación entre iteradores
     *
     * \pre ambos iteradores refieren a la misma colección
     * \post true sii los iteradores no apuntan al mismo elemento
     *
     * \complexity{\O(?)}
     */
    bool operator!=(const string_map<T>::iterator &other) const;

private:
    friend class string_map<T>;

    /**
     * @brief Constructor del iterador a partir de un iterador interno.
     */
    iterator(const typename string_map<T>::iterator&);

    typename string_map<T>::iterator it;


};

#endif //TP2_STRING_MAP_ITERATORS_H