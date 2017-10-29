#ifndef STRING_MAP_STRING_MAP_H
#define STRING_MAP_STRING_MAP_H

#include <cassert>
#include <string>
#include <vector>
#include "linear_map.h"

using std::string;
using std::vector;
using std::pair;

// TODO (Flor): Falta completar la documentación de la clase. Por ejemplo falta la descripción de la clase, se explica con TAD ???, rep, abs...
// TODO (Flor): Falta terminar la documentación de cada operación. Por ejemplo falta pre y post, mejorar las descripciones y en algunos casos falta la complejidad
// TODO (Flor): Documentar  también funciones auxliares. Por ejemplo, los constructores y destructor de Nodo

/**
 * Implementacion de map<string,T> sobre Trie
 * Asume de T:
 * - tiene constructor por copia 
 * - tiene operador ==
 * - solo permite utilizar el operator[] si T tiene constructor por defecto
 */
template < typename T >
class string_map {
public:
    typedef string key_type;
    typedef T mapped_type;
    typedef std::pair<const key_type, mapped_type> value_type;
    typedef size_t size_type;


    class iterator;
    class const_iterator;
    

    /** @brief Construye mapa vacio
     *
     * \complexity{\O(1)}
     */
    string_map();

    /** @brief Destruye mapa
     *
     * \complexity{\O(sn * S)}
     */
    ~string_map();

    /** @brief Constructor por copia
     *
     * \complexity{\O(sn * S)}
     */
    string_map(const string_map &);

    /** @brief Operador de asignacion
     *
     * \complexity{\O(sn * S)}
     */
    string_map& operator=(const string_map &);

    /** @brief Operadores de comparacion
     *
     * \complexity{\O(sn * S)}
     */
    bool operator==(const string_map& otro) const;
    bool operator!=(const string_map& otro) const {return !(*this == otro);}

    /** @brief Cantidad de apariciones de la clave (0 o 1)
     *  @param key clave a buscar
     *
     *  \complexity{\O(S)}
     */
    size_type count(const key_type &key) const;

    /** @brief Devuelve cantidad de claves definidas */
    size_t size() const;

    /** @brief devuelve true si size() == 0 */
    bool empty() const;


    /** @brief Acceso / definición de pares clave/valor
     *  @param key clave a acceder, si no existe, se crea
     *  @returns una referencia a la definicion.
     *
     *  \complexity{\O(S)}
     */
    mapped_type &operator[](const key_type &key);

    /** @brief Acceso a una clave sin modificar mapa
     *  @param key clave a acceder que debe existir previamente
     *  @returns una referencia a la definicion.
     *
     *  \complexity{\O(S)}
     */
    mapped_type& at(const key_type& key);

    /** @brief Acceso a una clave sin modificar mapa
     *  @param key clave a acceder que debe existir previamente
     *  @returns una referencia const a la definicion.
     *
     *  \complexity{\O(S)}
     */ 
    const mapped_type& at(const key_type& key) const;

    /** @brief Vacia el mapa */
    void clear();

    // Accesos con iteradores

    /** @brief iterador al primer par <clave,significado> en orden lexicografico
     *  @returns iterador al elemento o end() si el mapa era vacio
     *
     *  \complexity{\O(S)}
     */
   iterator begin();

    /*  @brief iterador al fin de la coleccion
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
     *  \complexity{\O(S)}
     */
   iterator find(const key_type &key);

    /** @brief busca una clave
     *  @param key clave a buscar
     *  @returns un iterador const al par <clave, significado>
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
     * \complexity{\O(S + copy(value_type))}
     */
    pair<iterator,bool> insert(const value_type &value);

    /** @brief eliminar una clave
     *  @param key clave a eliminar
     *  @returns cantidad de elementos eliminados
     *
     *  \complexity{\O(S)}
     */
    size_type erase(const key_type& key);

    /** @brief eliminar una clave mediante irerador
     *  @param pos iterador apuntando a clave a eliminar
     *  @returns iterador apuntando el proximo de la clave eliminada (o end() si era la ultima)
     *
     *  \complexity{\O(S)}
     */
    iterator erase(iterator pos);

private:
    //linear_set<string> _claves;
     struct Nodo
     {
         
        linear_map<char,Nodo*>* _sig; //Falta incluir linear_map.h. ¿Porqué es un puntero a lineal_map y no un lineal_map?
        T* _significado;


        Nodo() : _sig(nullptr), _significado(nullptr) {}

        ~Nodo(){
            if(_sig != nullptr){
                delete *_sig; //a lo que apunta sig?????
            }
            if(_significado != nullptr){
                delete *_significado;// a lo que apunta significado??????????
            }

        }

        Nodo(const Nodo & nodo){
            _sig = nullptr;
            _significado = nullptr;
            if(nodo._sig != nullptr){
                linear_map<char,Nodo*> valor(&nodo._sig); //TODO (Flor) : no es correcto el tipo de dato pasado como parametro al contructor por copia de linear_map
                _sig = *valor;
            }
            if(nodo._significado != nullptr){
                T valor(nodo._significado);
                _significado = *valor; //TODO (Flor) : No es correcta esta asignación
            }            
        }
     };
     Nodo* _comienzo; 
     size_t _size;

};

//implementacion
template<typename T>
string_map<T>::string_map(){
    _comienzo = nullptr;
}

template<typename T>
string_map<T>::~string_map(){
    //Todo (Flor) : ¿Y el resto de los nodos no se borran? Estas perdiendo memoria
    delete &_comienzo;
}

// * copy(t)
template<typename T>
string_map<T>::string_map(const string_map & otro){
    if(!otro.empty()){
        Nodo n = new Nodo(*(otro._comienzo));
        _comienzo = *n;    
    }else{
        _comienzo = nullptr;
    }
    

}

template<typename T>
string_map& string_map<T>::operator=(const string_map & otro){
    return string_map(otro);

}

template<typename T>
bool string_map<T>::operator==(const string_map& otro) const{

}



template<typename T>
size_type string_map<T>::count(const key_type &key) const{
    
    if(_comienzo.empty()) return 0;
    linear_map<char,Nodo*>* n = _comienzo->_sig;
    for (int i = 0; i < key.size(); ++i)
    {
        linear_map<char,Nodo*>::iterator it = n->find(key[i]);
        if(it == n->end()){
            return 0;

        }
        if(i == key.size()-1){
            if(it->second->_significado == nullptr){
                return 0;
            }else{
                return 1;
            }
        }
        n = it->second->_sig;
    }
}


template<typename T>
size_t string_map<T>::size() const{
    return _size;
}


template<typename T>
bool string_map<T>::empty() const{
    return size() == 0;
}

template<typename T>
mapped_type& string_map<T>::operator[](const key_type &key){
    if(count(key) == 1){
        return at(key);
    }else{
        pair<string_map<T>::iterator,bool> aux = insert(pair<key_type,T>(key,T()));
        return aux.first->value;
    }   

}


template<typename T>
mapped_type& string_map<T>::at(const key_type& key){
    linear_map<char,Nodo*>* n = _comienzo->_sig;
    for (int i = 0; i < key.size(); ++i)
    {
        linear_map<char,Nodo*>::iterator it = n->find(key[i]);        
        if(i == key.size()-1){            
            return *(it->second->_significado);
            
        }
        n = it->second->_sig;
    }   
}


template<typename T>
mapped_type& string_map<T>::at(const key_type& key) const{
    linear_map<char,Nodo*>* n = _comienzo->_sig;
    for (int i = 0; i < key.size(); ++i)
    {
        linear_map<char,Nodo*>::const_iterator it = n->find(key[i]);        
        if(i == key.size()-1){            
            return *(it->second->_significado);
            
        }
        n = it->second->_sig;
    }
}


template<typename T>
void string_map<T>::clear(){
    if(empty()) return;
    linear_map<char,Nodo*>::iterator it = begin();
    while(!empty()){
        it = erase(it);
    }
}







#endif //STRING_MAP_STRING_MAP_H
