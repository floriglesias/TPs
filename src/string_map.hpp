#include "string_map.h"
#include "string_map_iterators.h"

//TODO  (Flor): ojo que así como está, el compilador no entiende que esta implementación es de strig_map.h
//TODO (Flor) : utilizan en varias implementaciones el código necesario para buscar una clave. Recuerden que es una buena práctica no repetir código sino reutilizar. (Pueden usar funciones auxiliares para eso)


template < typename T >
string_map(){
    Nodo* n = new Nodo(); //TODO (Flor) : ¿Por qué hacen new de un nodo y no directamente ponen _raiz = null?
    raiz_ = n;
    claves = linear_set();
}




/** @brief Destruye mapa
 *
 * \complexity{\O(sn * S)}
 *
 * TODO (Flor): Falta la implementación del destructor del string_map
 */
template < typename T >
~string_map(){}



/** @brief Constructor por copia
 *
 * \complexity{\O(sn * S * copy(T))}
 */
template < typename T >
string_map(const string_map &otro){
//TODO  (Flor): ¿Qué pasa si this no es vacio? Pierden memoria!!!
    Nodo* n = new Nodo();
    raiz_ = n;
    linear_map<string>::const_iterator it = otro.; //TODO  (Flor): otro. ????? (me refiero al punto despué de otro)
    while (it.HaySiguiente()){
        String k = it.Siguiente();
        Definir(k, otro.Significado(k));
        it.Avanzar();
    }

}

/** @brief Operador de asignacion
 *
 * \complexity{\O(sn * S * copy(T))}
 */
template < typename T >
string_map& operator=(const string_map &otro){
    claves = otro.claves;
    raiz_ = otro.raiz_;
    //TODO  (Flor): this y otro debería ser independientes después de la asignación. Su implementación no lo permite.
    //El peligro de su implementación es que si otro deja de existir (o se modifica), lo mismo pasa con this.
}



/** @brief Operadores de comparacion
 *
 * \complexity{\O(sn * S * cmp(T))}
 */
template < typename T >
bool operator==(const string_map& otro) const{
    return claves == otro.claves && raiz_ == otro.raiz_;
    //TODO  (Flor): ¿Y los significados del diccionario pueden ser distintos?
}
template < typename T >
bool operator!=(const string_map& otro) const {return !(*this == otro);}




/** @brief Cantidad de apariciones de la clave (0 o 1)
 *  @param key clave a buscar
 *
 *  \complexity{\O(S)}
 */
template < typename T >
size_type count(const key_type &key) const{
    int i = 0;
    Nodo* actual = raiz_;
    while (i < key.size() && actual->chars[(int)key[i]] != NULL){
        actual = actual->chars[ (int)key[i] ];
        i++;
    }
    if(i == key.size() && actual -> val != nullptr){
        return 1;
    }else{
        return 0;
    }
}

/** @brief Devuelve cantidad de claves definidas */
template < typename T >
size_t size() const{
    return claves.size();
}

/** @brief devuelve true si size() == 0 */
template < typename T >
bool empty() const{
    return size() == 0;
}


/** @brief Acceso / definición de pares clave/valor
 *  @param key clave a acceder, si no existe, se crea
 *  @returns una referencia a la definicion.
 *
 *  \complexity{\O(S)}
 */
template < typename T >
mapped_type &operator[](const key_type &key){
    //TODO (Flor): Esta implementación es incorrecta. Revean que pasa si la clave no está, porque da segmentation fault
    //TODO (Flor): Esta implementación está incompleta.
    int i = 0;
    Nodo* actual = raiz_;
    while (i < key.size()){
        actual = actual->chars[ (int)key[i] ];
        i++;
    }
    if(actual == nullptr){
        Nodo* nuevaClave = new Nodo();
        actual = nuevaClave;

    }
    return actual->val;
    //O(completar)
}


/** @brief Acceso a una clave sin modificar mapa
 *  @param key clave a acceder que debe existir previamente
 *  @returns una referencia a la definicion.
 *
 *  \complexity{\O(S)}
 */
template < typename T >
mapped_type& at(const key_type& key){
    int i = 0;
    Nodo* actual = raiz_;
    while (i < key.size() && actual != nullptr){ //TODO (Flor) : El chequeo de actual != nullptr no es necesario ya que la clave existe previamente
        actual = actual->chars[ (int)key[i] ];
        i++;
    }
    return actual -> val;
    //O(completar)

}



/** @brief Acceso a una clave sin modificar mapa
 *  @param key clave a acceder que debe existir previamente
 *  @returns una referencia const a la definicion.
 *
 *  \complexity{\O(S)}
 */
template < typename T >
const mapped_type& at(const key_type& key) const{
    int i = 0;
    Nodo* actual = raiz_;
    while (i < key.size() && actual != nullptr){ //TODO (Flor) : El chequeo de actual != nullptr no es necesario ya que la clave existe previamente
        actual = actual->chars[ (int)key[i] ];
        i++;
    }
    return actual -> val;
    //O(completar)
}



/** @brief Vacia el mapa */
template < typename T >
void clear(); //TODO (Flor): Falta implementar



// Accesos con iteradores

/** @brief iterador al primer par <clave,significado> en orden lexicografico
 *  @returns iterador al elemento o end() si el mapa era vacio
 *
 *  \complexity{\O(S)}
 */
template < typename T >
iterator begin(){
    string primeraClave = claves[0]; //TODO (Flor): ¿Qué es claves[0]? No existe esa operación en un lineal_set; por otro lado, si existia podría generarse segmentation fault si el dicc es vacio
    Nodo* actual = raiz_;
    int i = 0;
    while(i < primeraClave.size()){
        actual = actual->chars[ (int)primeraClave[i] ];
        i++;
    }
     return actual.crearIt();//TODO (Flor) : ¿De dónde sale creatIt? No la encuentro declarada en ningún lado

}


/*  @brief iterador al fin de la coleccion
 *
 *  \complexity{\O(S)}
 */
iterator end(){
    string ultimaClave = claves[claves.size()-1]; //TODO (Flor): ¿Qué es claves[claves.size()-1]? No existe esa operación en un lineal_set; por otro lado, si existia podría generarse segmemtation fault si el dicc es vacio
    Nodo* actual = raiz_;
    int i = 0;
    while(i < ultimaClave.size()){
        actual = actual->chars[ (int)ultimaClave[i] ];
        i++;
    }
    return actual.crearIt();//TODO (Flor) : ¿De dónde sale creatIt? No la encuentro declarada en ningún lado
}

/// Versiones const de begin/end
const_iterator begin() const; //TODO (Flor) : Falta implementar
const_iterator end() const;//TODO (Flor) : Falta implementar
const_iterator cbegin() const;//TODO (Flor) : Falta implementar
const_iterator cend() const;//TODO (Flor) : Falta implementar



/** @brief busca una clave
 *  @param key clave a buscar
 *  @returns un iterador al par <clave, significado>
 *
 *  \complexity{\O(S)}
 */
iterator find(const key_type &key){
    //TODO (Flor) : ¿Qué pasa si la clave no esta definida es precondición o tiene que analizar ese caso en el código?
    Nodo* actual = raiz_;
    int i = 0;
    while(i < key.size()){
        actual = actual->chars[ (int)key[i] ];
        i++;
    }
    return actual.crearIt(); //TODO (Flor) : ¿De dónde sale creatIt? No la encuentro declarada en ningún lado

};

/** @brief busca una clave
 *  @param key clave a buscar
 *  @returns un iterador const al par <clave, significado>
 *
 *  \complexity{\O(S)}
 */
const_iterator find(const key_type &key) const; //TODO (Flor) : Falta implementar

/** @brief insercion
 *
 * @param value par <clave,significado> a insertar
 * @returns un par con un iterador al par clave-significado agregado o
 * modificado y un bool que indica si la clave se insertó como una clave
 * nueva.
 *
 * \complexity{\O(S + copy(T))}
 */
pair<iterator, bool> insert(const value_type &value); //TODO (Flor) : Falta implementar

/** @brief eliminar una clave
 *  @param key clave a eliminar
 *  @returns cantidad de elementos eliminados
 *
 *  \complexity{\O(S)}
 */
size_type erase(const key_type& key){
    Nodo* actual = raiz_;
    int i = 0;
    while(i < key.size()){
        actual = actual->chars[ (int)key[i] ];
        i++;
    }
    iterador = actual.crearIt(); //TODO (Flor) : ¿De dónde sale creatIt? No la encuentro declarada en ningún lado. Además ¿de qué tipo es iterador?
    iterador.erase(); //TODO (Flor) : ¿De dónde sale erase? No la encuentro declarada en ningún lado

    //PARA CUMPLIR ESTA COMPLEJIDAD, NUESTRO ITERADOR DEBERIA APUNTAR AL NODO ESTE, Y A LA CLAVE QUE ESTA EN EL VECTOR
    //TODO (Flor) : Si, es verdad que deben borrar la clave del conjunto de claves de alguna manera cumpliendo la complejidad pedida
    //TODO (Flor) : ¿qué hace erase()? Piensen la función para que cumpla la complejidad pedida
    //TODO (Flor) : Tengan en cuenta que el borrado de una clave puede generar que se borren más de un nodo. Por ejemplo, un dicc que tiene definida solo la clave "casa", al borrar dicho clave, se deben borran 4 nodos.


};

/** @brief eliminar una clave mediante irerador
 *  @param pos iterador apuntando a clave a eliminar
 *  @returns iterador apuntando el proximo de la clave eliminada (o end() si era la ultima)
 *
 *  \complexity{\O(S)}
 */
iterator erase(iterator pos); //TODO (Flor) : Completar

#ifndef TP2_STRING_MAP_HPP
#define TP2_STRING_MAP_HPP

#endif //TP2_STRING_MAP_HPP
