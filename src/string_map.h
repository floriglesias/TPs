#ifndef STRING_MAP_STRING_MAP_H
#define STRING_MAP_STRING_MAP_H

#include <cassert>
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::pair;


//TODO (Flor) : La estructura está bien
// TODO (Flor): Falta completar la documentación de la clase. Por ejemplo falta la descripción de la clase, se explica con TAD ???, rep, abs...
// TODO (Flor): Falta terminar la documentación de cada operación. Por ejemplo falta pre y post, mejorar las descripciones y en algunos casos falta la complejidad
// TODO (Flor): Faltan impletar varias operaciones
//TODO (Flor) : utilizan en varias implementaciones el código necesario para buscar una clave. Recuerden que es una buena práctica no repetir código sino reutilizar. (Pueden usar funciones auxiliares para eso)


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
    struct Node {
        //bool _tiene_significado;   //  opcional (significado == NULL es la alternativa)

        //Voy a preguntar si significado es NULL/nullptr para saber si tiene o no.
        //Asumimos que una hoja del trie necesariamente tiene significado
        //Asumimos que cuando se borra un significado , si el nodo no es necesario para otra palabra dicho nodo se borra tmb.
        Node* _children[256];
        T* _significado;
        Node* _padre;
        Nodo() :  (_children(NULL), _significado(NULL), _padre(NULL)){;} //TODO (Flor) : hay varios errores de sintaxis de c++ en está línea de código
        //TODO (Flor) : ¿Qué significa _children(NULL)? Asi no se inicializa un arreglo
    };

    Node* _root; //Esto lo usamos para empezar a recorrer el trie. (Posiblemente el iterator parta de aca tmb.)
    int _size; //Cuenta la cantidad de claves

    void recorrerHijos(string_map::Node* original, const string_map::Node*  aCopiar);
};

#endif //STRING_MAP_STRING_MAP_H

template < typename T >
string_map<T>::string_map() {
    _root = NULL;
    _size = 0;
}

template < typename T >
string_map<T>::~string_map() {
    clear();
}


template < typename T >
void string_map<T>::recorrerHijos(string_map<T>::Node* original, const string_map<T>::Node*  aCopiar) {
    for (int i = 0; i < 256; ++i) {
        if (aCopiar->_children[i] == NULL) {
            original->_children[i] = NULL;
        } else {
            original->_children[i] = new Node();
            if (aCopiar->_children[i]->_significado != NULL) { //Si tiene significado se lo asigno.
                original->_children[i]->_significado = new T; //ToDO(Flor) : Si T no tiene constructor por default, entonces explota
                original->_children[i]->_significado = aCopiar->_children[i]->_significado;
                if (aCopiar->_children[i]->_children != NULL) {
                    recorrerHijos(original->_children[i], aCopiar->_children[i]);
                }
            }
            else{
                original->_children[i]->_significado = NULL;
                //TODO(Flor): ¿Y si aCopiar->_children[i]->_children != NULL no sigo copiando?
            }
        }
    }
}


template < typename T >
string_map<T>::string_map(const string_map &copia){
    //Hago la copia del trie que me llega por parametro.

    if (copia._root == NULL) {
        this->_root = NULL;
        this->_size=0;
    }

        //TODO(Flor): la idea de la copia parece estar bien, pero ¿por qué no utilizan la operación insertar?
    else{ //SI TIENE ALGUN ELEMENTO
        this->_root = new Node();
        for (int i = 0; i < 256; ++i) {
            if (copia._root->_children[i] == NULL){ //Si el que estoy mirando es nulo.
                this->_root->_children[i] = NULL;
            }
            else{
                recorrerHijos(this->_root->_children[i], copia._root->_children[i]);

            }
        }
    }
}


template < typename T >
size_t string_map<T>::size() const{
    return _size;
}


template < typename T >
bool string_map<T>::empty() const {
    return _size == 0;
}


template < typename T >
pair<iterator,bool> string_map<T>::insert(const value_type &value) //el value que me llega es un pair<const string, T>
{

    //Todo (Flor) : El código no funciona si la clave que esta definiendo es prefijo de otra. Fijense que en este caso devuelve <it,false>
    // Ej tiene definida la clave "casamiento" y quieren definir la clave "casa".

    int cantLetras = value.first.length();
    bool claveNueva = false;
    Node* aComparar = this->_root;
    iterator it= this->begin(); //Todo (Flor) : Falta mover este iterador para que apunte a lo pedido
     //Esto faltaria , tengo que apuntar al nodo donde meti el significado
    for (int i = 0; i < cantLetras; ++i) {
        char l = value.first[i];


        if (aComparar->_children[toascii(l)] == nullptr){ //Si no existia el nodo en esa posicion lo creo
            aComparar->_children[toascii(l)] = new Node();
            claveNueva = true;  //Si lo creo entonces seguro esa clave no existia
        }

        aComparar = aComparar->_children[toascii(l)]; //Lo avanzo


        //TODO (Flor) : Esta parte del código deberia ir afuera del ciclo. (buena práctica de programación). Sino se llama a este if en cada iteración.
        if (i == cantLetras) { //Si en la proxima iteracion ya termina
            aComparar->_significado = new T; //TODO (Flor) : No estoy segura que esto compile (No funciona seguro si T no tiene constructor por default). Deberían hacer new T(value.second).
            aComparar->_significado = value.second; //Estoy en la ultima letra de la clave -> defino el significado
        }
        //TODO (Flor) : ¿Qué pasa si esa clave ya estaba definida? Pierden memoria!!!
    }



    pair resul = std::make_pair(it,claveNueva);
    return resul;

}



template < typename T >
iterator string_map<T>::find(const key_type &key) {
    Node* aComparar = this->_root;
    int cantLetras = key.length();
    for (int i = 0; i < cantLetras; ++i) {
        char l = key[i];
        if (aComparar->_children[toascii(l)] == nullptr) { //Si no existe el nodo en esa posicion
            return this->end();
        }
        else {
            aComparar = aComparar->_children[toascii(l)];
        }
    }

    //Aca deberia devolver el iterator apuntando a acomparar //TODO(Flor): Sí, falta esta parte
    return
}


template < typename T >
void string_map<T>::clear() {
    //Vacia el mapa.
    //TODO(Flor): Falta implementar
}

template < typename T >
std::size_t string_map<T>::count(const key_type &key) const{
    //0 -> No esta la clave
    //1 -> Esta la clave
    Node* aComparar = this->_root;
    int cantLetras = key.length();
    for (int i = 0; i < cantLetras; ++i) {
        char l = key[i];
        if (aComparar->_children[toascii(l)] == nullptr){
            //No existe la clave
            return 0;
        }
        else {
            aComparar = aComparar->_children[toascii(l)];
        }
    }
    return true;
}

template < typename T >
string_map<T>::mapped_type& string_map<T>::operator[](const string_map<T>::key_type &key) {
    bool claveNueva = false;
    int cantLetras = key.length();
    Node* aComparar = this->_root;
    for (int i = 0; i < cantLetras; ++i) {
        char l = key[i];
        if (aComparar->_children[toascii(l)] == NULL){ //Si no existia el nodo en esa posicion lo creo
            aComparar->_children[toascii(l)] = new Node();
            claveNueva = true; //TODO(Flor): Mismo problema que en insertar. Si la clave es prefijo de otra ya definida, entoces este valor queda en false cuando debería ser true
        }
        aComparar = aComparar->_children[toascii(l)]; //Lo avanzo
    }
    if (claveNueva) {
        return NULL; //TODO(Flor): Si la clave es nueva, el significado no es NULL sino el constructor por defecto de T (y deben agregarlo al diccionario). Sino tiene constructor por defecto se espera que esta función explote
    }
    else {
        return aComparar->_significado;
    }
}


template < typename T >
string_map<T>::mapped_type& string_map<T>::at(const string_map<T>::key_type& key){
    int cantLetras = key.length();
    Node* aComparar = this->_root;
    int i =0;
    while (aComparar->_children != NULL){  //TODO(Flor): Mismo problema que en insertar. ¿Qué pasa si la clave es prefijo de otra?
        char l = key[i];
        aComparar = aComparar->_children[toascii(l)]; //Lo avanzo
        i++;
    }
    return aComparar->_significado;

}

template < typename T >
const string_map<T>::mapped_type& string_map<T>:: at(const string_map<T>::key_type& key) const{
    int cantLetras = key.length();
    Node* aComparar = this->_root;
    int i =0;
    while (aComparar->_children != NULL){ //TODO(Flor): Mismo problema que en insertar. ¿Qué pasa si la clave es prefijo de otra?
        char l = key[i];
        aComparar = aComparar->_children[toascii(l)]; //Lo avanzo
        i++;
    }
    
    const Node* res = aComparar->_significado;
    return res;
}

template < typename T >
string_map<T>::iterator string_map<T>::find(const string_map<T>::key_type &key){
    string_map<T>::iterator it;
    it = it.begin();
    while (it.first != key) {
        ++it;
    }
    return it;
}

template < typename T >
string_map<T>::const_iterator string_map<T>::find(const string_map<T>::key_type &key) const{
    string_map<T>::const_iterator it;
    it = it.begin();
    while (it.first != key) {
        ++it;
    }
    return it;
}
