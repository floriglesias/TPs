#ifndef linear_set_iterators_h
#define linear_set_iteratros_h

#include "string_map.h"
#include <iostream>

using namespace std;

//TODO(Flor): Falta  implementar todo la clase
//TODO(Flor): Revisen que estructura van a utilizar para el iterador porque no les va a servir el código de linear_set_iterator
//TODO(Flor): Falta documentar todo la clase

template<class T>
class string_map<T>::const_iterator {
public:
    using value_type = string_map::value_type;
    using iterator_category = std::forward_iterator_tag;
    using reference = value_type&;
    using pointer = value_type*;
    using difference_type = std::ptrdiff_t;


    /**
     * @brief Constructor por copia del iterador.
     *
     * \complexity{\O(1)}
     */
    const_iterator(const typename string_map<T>::const_iterator&);

    /**
     * @brief Conversión desde iterator
     *
     * \complexity{\O(1)}
     */
    const_iterator(const typename string_map<T>::iterator&);

    /**
     * @brief Avanza el iterador una posición.
     *
     * \pre El iterador no debe estar en la posición pasando-el-último.
     * \post \P{res} es una referencia a \P{this}. \P{this} apunta a la posición
     * siguiente.
     *
     * \complexity{\O(1)}
     */
    string_map::const_iterator &operator++();

    /**
     * @brief Desreferencia el puntero
     *
     * El valor devuelto tiene aliasing dentro de la colección.
     *
     * \pre El iterador no debe estar en la posición pasando-el-último.
     * \post El valor resultado es una referencia constante al valor apuntado.
     *
     * \complexity{\O(1)}
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
     * \complexity{\O(1)}
     */
    const value_type* operator->() const;

    /**
     * @brief Comparación entre iteradores
     *
     * \pre ambos iteradores refieren a la misma colección
     * \post true sii los iteradores apuntan al mismo elemento
     *
     * \complexity{\O(1)}
     */
    bool operator==(const string_map<T>::const_iterator &other) const;

    /**
     * @brief Comparación entre iteradores
     *
     * \pre ambos iteradores refieren a la misma colección
     * \post true sii los iteradores no apuntan al mismo elemento
     *
     * \complexity{\O(1)}
     */
    bool operator!=(const string_map<T>::const_iterator &other) const;

private:
    friend class string_map<T>;

    /**
     * @brief Constructor del iterador a partir de un iterador interno.
     */
    const_iterator(const typename linear_map<T, bool>::const_iterator&);

    typename linear_map<T, bool>::const_iterator it;
};

template<class T>
class string_map<T>::iterator {
public:
    using value_type = const string_map::value_type;
    using iterator_category = std::forward_iterator_tag;
    using reference = value_type&;
    using pointer = value_type*;
    using difference_type = std::ptrdiff_t;

    /**
     * @brief Constructor por copia del iterador.
     *
     * \complexity{\O(1)}
     */
    iterator(const typename string_map<T>::iterator&);

    /**
     * @brief Avanza el iterador una posición.
     *
     * \pre El iterador no debe estar en la posición pasando-el-último.
     * \post \P{res} es una referencia a \P{this}. \P{this} apunta a la posición
     * siguiente.
     *
     * \complexity{\O(1)}
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
     * \complexity{\O(1)}
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
     * \complexity{\O(1)}
     */
    const value_type* operator->() const;

    /**
     * @brief Comparación entre iteradores
     *
     * \pre ambos iteradores refieren a la misma colección
     * \post true sii los iteradores apuntan al mismo elemento
     *
     * \complexity{\O(1)}
     */
    bool operator==(const string_map<T>::iterator &other) const;

    /**
     * @brief Comparación entre iteradores
     *
     * \pre ambos iteradores refieren a la misma colección
     * \post true sii los iteradores no apuntan al mismo elemento
     *
     * \complexity{\O(1)}
     */
    bool operator!=(const string_map<T>::iterator &other) const;

private:
    friend class string_map<T>;

    /**
     * @brief Constructor del iterador a partir de un iterador interno.
     */
    iterator(const typename linear_map<T, bool>::iterator&);

    typename linear_map<T, bool>::iterator it;
};


// const_iterator methods

template<typename T>
string_map<T>::const_iterator::const_iterator(
        const typename string_map<T>::const_iterator &other) : it(other.it) {}

template<typename T>
string_map<T>::const_iterator::const_iterator(
        const typename string_map<T>::iterator &other) : it(other.it) {}

template<typename T>
typename string_map<T>::const_iterator &string_map<T>::const_iterator::operator++() {
    it++;
    return *this;
}

template<typename T>
const typename string_map<T>::const_iterator::value_type &string_map<T>::const_iterator::operator*() const {
    return it->first;
}

template<typename T>
const typename string_map<T>::const_iterator::value_type *string_map<T>::const_iterator::operator->() const {
    return &it->first;
}

template<typename T>
bool string_map<T>::const_iterator::operator==(const string_map<T>::const_iterator &other) const {
    return it == other.it;
}

template<typename T>
bool string_map<T>::const_iterator::operator!=(const string_map<T>::const_iterator &other) const {
    return not (*this == other);
}

template<typename T>
string_map<T>::const_iterator::const_iterator(
        const typename linear_map<T, bool>::const_iterator& _it)
        : it(_it) {};


// iterator methods

template<typename T>
string_map<T>::iterator::iterator(const typename string_map<T>::iterator& other)
        : it(other.it) {}

template<typename T>
string_map<T>::iterator::iterator(const typename linear_map<T, bool>::iterator& _it)
        : it(_it) {};

template<typename T>
typename string_map<T>::iterator &string_map<T>::iterator::operator++() {
    it++;
    return *this;
}

template<typename T>
const typename string_map<T>::iterator::value_type &string_map<T>::iterator::operator*() const {
    return it->first;
}

template<typename T>
const typename string_map<T>::iterator::value_type *string_map<T>::iterator::operator->() const {
    return &it->first;
}

template<typename T>
bool string_map<T>::iterator::operator==(const string_map<T>::iterator &other) const {
    return it == other.it;
}

template<typename T>
bool string_map<T>::iterator::operator!=(const string_map<T>::iterator &other) const {
    return not (*this == other);
}

#endif // linear_set_iterators_h
