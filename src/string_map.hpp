#include "string_map.h"

template < typename T >
string_map<T>::string_map() : length{}, root{} {
    //default constructor
};


template < typename T >
string_map<T>::~string_map() {
    //region O = O(sn * S)
    clear();
    //endregion
}

template < typename T >
string_map<T>::string_map(const string_map<T> &other){
    (*this) = other;
}

template <typename T>
string_map<T>& string_map<T>::operator=(const string_map<T> &other) {
    /**
     * Caso 1: Estoy tratando de asignarme a mi mismo :P
     */
    //region O = O(1) + O(1) = O(1)
    if(&other == this){
        return *this;
    }
    //endregion
    /**
     * Caso 2: Tiene elementos, hay que limpiarlo antes de
     * iniciarlizarlo
     */
    //region O = O(1) + O(sn * S) = O(sn * S)
    if(length != 0){
        clear();
    }
    //endregion
    /**
     * Caso 3: Copio los elementos que tiene un string_map
     * hacia el otro
     */
    std::copy(other.begin(), other.end(), output_iterator(this));
    return *this;
}

template <typename T>
bool string_map<T>::operator==(const string_map<T>& other) const {
    //region O = max(O(1), max(O(1), O(ns * (S + compare(T))) = O(ns * (S + compare(T)))
    /**
     * caso 1: me estoy comparando conmigo mismo :P (puede pasar)
     */
    //region O = O(1) + O(1) = O(1)
    if(&other == this){
        return true;
    }
    //endregion
    /**
     * Caso 2: tienen distinta cantidad de elementos
     */
    //region O = O(1) + O(1) = O(1)
    if(length != other.length){
        return false;
    }
    /**
     * Caso 3: realizo la comparacion entre elementos
     */
    //endregion
    //region O = O(ns * (S + compare(T)))
    return std::equal(other.begin(), other.end(), begin());
    //endregion
    //endregion
}

template <typename T>
size_t string_map<T>::count(const key_type &key) const{
    //region O(1) + max(O(1), O(S)) = O(1) + O(S) = O(S)
    if(length == 0){
        //region O = O(1)
        return 0;
        //endregion
    } else {
        //region O = O(S) + O(1) = O(S)
        //region O = O(S)
        Node* tmp = root;
        for(int i = 0; i < key.size(); i ++){
            if(tmp->children[key[i]] != nullptr){
                tmp = tmp->children[key[i]];
            } else {
                return false;
            }
        }
        //endregion
        //region O = O(1)
        return tmp->element != nullptr;
        //endregion
        //endregion
    }
    //endregion
}

template <typename T>
size_t string_map<T>::size() const {
    return length;
}

template <typename T>
bool string_map<T>::empty() const {
    return length == 0;
}

template <typename T>
T& string_map<T>::operator[](const key_type &key){
    static_assert(std::is_default_constructible< typename std::decay<T>::type >::value, "T must be default constructible");
    //region O = O(S) + O(1) + max(O(S), O(S)) = O(S) + O(1) + O(S) = O(S)
    if(count(key) == 1){
        //region O = O(S)
        return at(key);
        //endregion
    } else {
        //region O = O(S) + O(1) + O(1) = O(S)
        //region O = O(S)
        Node* tmp = createNode(key);
        //endregion
        //region O = O(1) + O(1) + O(1) + O(1) = O(1)
        if(tmp->element != nullptr){
            delete tmp->element;
            tmp->element = nullptr;
        }
        tmp->element = new T();
        tmp->key = &key;
        if(tmp->pair != nullptr){
            delete tmp->pair;
            tmp->pair = nullptr;
        }
        tmp->pair = new value_type(key, T());
        return *(tmp->element);
        //endregion
        //endregion
    }
    //endregion
}

template<typename T>
const T& string_map<T>::at(const key_type& key) const{
    //region O = O(1) + O(S) = O(S)
    return (const_cast< string_map<T>* >(this))->at(key);
    //endregion
};

template<typename T>
T& string_map<T>::at(const key_type& key){
    //region O = O(1) + O(S) = O(S)
    //region O = O(S)
    Node* tmp = root;
    std::for_each(key.begin(), key.end(), [&](const char & key){
        tmp = tmp->children[key];
    });
    //endregion
    //region O = O(1)
    return *tmp->element;
    //endregion
    //endregion
}

template<typename T>
void string_map<T>::clear() {
    //region O = O(sn * S) + O(1) + O(1) = O(sn * S)
    delete root;
    root = nullptr;
    length = 0;
    //endregion
}

template<typename T>
size_t string_map<T>::erase(const key_type& key){
    //region O = O(S) + O(1) + O(S) + O(1) + O(1) + O(1) = O(S)
    //region O = O(1) + O(S) + O(1) = O(S)
    if(!count(key)){
        return false;
    }
    //endregion
    //region O = O(1)
    Node* tmp = root;
    //endregion
    //region O = O(S) * O(1) = O(S)
    std::for_each(key.begin(), key.end(), [&](const char & key){
        //region O = O(1) + O(1) + O(1) = O(1)
        tmp->count_children --;
        tmp = tmp->children[key];
        //endregion
    });
    //endregion
    //region O = O(1) + O(1) + O(1) = O(1)

    //region O = O(1) + O(1)
    if(tmp->element != nullptr){
        delete tmp->element;
    }
    //endregion
    if(tmp->pair != nullptr){
        delete tmp->pair;
    }

    //endregion
    //region O = O(1) + O(1) = O(1)
    tmp->element = nullptr;
    tmp->key = nullptr;
    tmp->pair = nullptr;
    length --;
    //endregion
    return true;
    //endregion
}

template<typename T>
typename string_map<T>::iterator string_map<T>::begin() {
    return iterator(root);
}

template<typename T>
typename string_map<T>::const_iterator string_map<T>::begin() const{
    return const_iterator(root);
}

template<typename T>
typename string_map<T>::const_iterator string_map<T>::end() const {
    return const_iterator(nullptr);
}

template<typename T>
typename string_map<T>::const_iterator string_map<T>::cbegin() const{
    return const_iterator(root);
}

template<typename T>
typename string_map<T>::const_iterator string_map<T>::cend() const{
    return const_iterator(nullptr);
}

template<typename T>
typename string_map<T>::iterator string_map<T>::end() {
    return iterator(nullptr);
}

template<typename T>
typename string_map<T>::iterator string_map<T>::erase(iterator position){
    //region O = O(1) + O(L) = O(S)
    //region O = O(1) + O(1)
    value_type value = *(position);
    //endregion
    //region O = O(1) + O(S) + O(1) = O(S)
    erase(value.first);
    return position ++;
    //endregion
    //endregion
}

template<typename T>
typename string_map<T>::iterator string_map<T>::find(const key_type &key){
    //region O = O(S) + O(1) = O(S)
    //region O = O(S)
    Node* tmp = findNode(key);
    //endregion
    //region O = O(1)
    return iterator(tmp);
    //endregion
    //endregion
}

template<typename T>
typename string_map<T>::const_iterator string_map<T>::find(const key_type &key) const{
    //region O = O(S) + O(1) = O(S)
    //region O = O(1)
    string_map<T>* self = const_cast<string_map<T>*>(this);
    //endregion
    //region O = O(S) + O(1) + O(1) = O(S)
    return const_iterator(self->find(key));
    //endregion
    //endregion
}

template<typename T>
pair<typename string_map<T>::iterator, bool> string_map<T>::insert(const value_type &value){
    //region O = O(1) + O(S) + O(1) = (S)
    //region O = O(1)
    Node *match = nullptr;
    bool inserted = false;
    //endregion
    //region O = O(S) + O(1) + max(O(S), O(S)) = O(S) + O(S) = O(S)
    if(count(value.first) == 1){
        /**
         * En este caso, lo que estamos haciendo es reemplazar el valor del nodo, como el nodo existe, tengo que traer-
         * lo y cambiar los valores.
         */
        //region O = O(S) + O(1) + O(1) = O(S)
        //region O = O(S)
        match = findNode(value.first);
        //endregion
        //region O = O(1) + O(1) + O(1) = O(1)
        //endregion
        //region O = O(1) + O(1) + O(1) = O(1)
        if(match->element != nullptr){
            delete match->element;
            match->element = nullptr;
        }
        match->element = new T(value.second);
        match->key = &value.first;
        if(match->pair != nullptr){
            delete match->pair;
            match->pair = nullptr;
        }
        match->pair = new value_type(value.first, T(value.second));
        inserted = false;
        //endregion
        //endregion
    } else {
        /**
         * En este caso, lo que estamos haciendo es crear un nuevo nodo y agregar el nuevo valor del nodo
         */
        //region O = O(S) + O(1) = O(S)
        //region O = O(S)
        match = createNode(value.first);
        //endregion
        //region O = O(1) + O(1) + O(1) = O(1)
        if(match->element != nullptr){
            delete match->element;
            match->element = nullptr;
        }
        match->element = new T(value.second);
        match->key = &value.first;
        if(match->pair != nullptr){
            delete match->pair;
            match->pair = nullptr;
        }
        match->pair = new value_type(value.first, T(value.second));
        inserted = true;
        //endregion
        //endregion
    }
    //endregion
    //region O = O(1)
    return make_pair(iterator(match), inserted);
    //endregion
    //endregion
}

template<typename T>
typename string_map<T>::iterator string_map<T>::fast_insert(const value_type& value){
    return this->insert(value).first;
}

template < typename T >
typename string_map<T>::Node * string_map<T>::createNode(const key_type &value) {
    //region O = O(S)
    if(root == nullptr){
        root = new Node;
    }
    Node* tmp = root;
    std::for_each(value.begin(), value.end(), [&](const char & key){
        if(tmp->children[key] == nullptr){
            tmp->children[key] = new Node;
            tmp->children[key]->parent = tmp;
            tmp->children[key]->index = key;
            tmp->isLeaf = false;
            tmp->count_children ++;
        }
        tmp = tmp->children[key];
    });
    length ++;
    return tmp;
    //endregion
}

template < typename T >
typename string_map<T>::Node * string_map<T>::findNode(const key_type &clave){
    //region O(S)
    Node* tmp = root;
    std::for_each(clave.begin(), clave.end(), [&](const char & key){
        tmp = tmp->children[key];
    });
    return tmp;
    //endregion
};