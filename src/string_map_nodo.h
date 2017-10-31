#ifndef STRING_MAP_NODO_H
#define STRING_MAP_NODO_H

#include "string_map.h"

template < typename T >
string_map<T>::Node::Node() {
    parent = nullptr;
    element = nullptr;
    children = {nullptr};
    isLeaf = true;
    count_children = 0;
    pair = nullptr;
    index = 0;
};

template < typename T >
string_map<T>::Node::~Node() {
    delete element;
    element = nullptr;
    key = nullptr;
    delete pair;
    pair = nullptr;
    for(size_t i = 0; i < CHAR_MAX; i ++){
        delete children[i];
        children[i] = nullptr;
    }
};

template < typename T >
typename string_map<T>::Node * string_map<T>::Node::sucesor(typename string_map<T>::Node *node){
    //region O = O(1) + O(1) = O(1)
    Node* tmp = node, *parent = nullptr, *candidate = nullptr, *child = nullptr;
    if(tmp == nullptr){
        return nullptr;
    }
    //endregion
    /**
     * Caso 1: empieza a buscar el sucesor en los hijos que tiene definido el nodo. El peor caso que te puede
     * pasar es ir hasta la ultima posicion del array del hijo y traer el sucesor del mismo
     */
    //region O = O(1) + O(S) = O(S)
    if(!tmp->isLeaf and tmp->count_children > 0){
        //region O = (127 - 32) + O(S) = O(S)
        for(int i = 32; i < CHAR_MAX; i++){
            //region O = O(S)
            tmp = nextChild(node->children[i]);
            if(tmp != nullptr){
                return tmp;
            }
            //endregion
        }
        //endregion
    }
    //endregion
    /**
     * Caso 2, como no tengo hijos o no tengo ningun hijo como sucesor, no me queda otra que empezar a ir viendo
     * que padre puede llegar a darme un sucesor.
     */
    //region O = O(S)
    //region O = O(1)
    tmp = node, parent = tmp->parent;
    //endregion
    //region O = O(S) + O(S) = O(S)
    while(parent != nullptr){
        /**
         * Subcaso 1: Una vez que encuentro a mi padre, tengo que buscar el proximo sucesor, en este caso lo que
         * me puede pasar es tener que recorrer hasta el final del array para encontrar mi sucesor
         */
        //region O = O(S)
        for(int i = (tmp->index + 1); i < CHAR_MAX; i++){
            tmp = nextChild(parent->children[i]);
            if(tmp != nullptr){
                return tmp;
            }
        }
        tmp = parent, parent = parent->parent;
        //endregion
    }
    //endregion
    //endregion
    return nullptr;
};

template < typename T >
typename string_map<T>::Node * string_map<T>::Node::nextChild(typename string_map<T>::Node *node) {
    //region O = O1() + O(S) + O(1) = O(S)
    //region O = O(1) + O(1) = O(1)
    //region O = O(1)
    if(node == nullptr){
        return nullptr;
    }
    //endregion
    //region O = O(1)
    if(node->element != nullptr){
        return node;
    }
    //endregion
    //endregion
    //region O = O(1) + O(S) = O(S)
    if(!node->isLeaf){
        for(int i = 32; i < CHAR_MAX; i++){
            Node* tmp = nextChild(node->children[i]);
            if(tmp != nullptr){
                return tmp;
            }
        }
    }
    //endregion
    //region O = O(1)
    return nullptr;
    //endregion
    //endregion
};
#endif //STRING_MAP_NODO_H
