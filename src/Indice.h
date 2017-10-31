#ifndef TP2_INDICE_H
#define TP2_INDICE_H

#include <string>
#include "Registro.h"
#include "linear_set.h"
#include "BaseDeDatos.h"

using namespace std;
using conjReg = linear_set<Registro>;

/**
 * @brief Un Indice es una estructura auxiliar que se utiliza en una base de datos
 * para acceder a registros por valor, con el fin de mejorar la velocidad de las operaciones.
 *
 * Por ejemplo, en una tabla Alumnos, de campos {LU: String, Nombre: String, Nacimiento: Nat},
 * un índice para el campo LU tiene como claves el conjunto de libretas, y como significado los registros
 * que tienen, en su campo LU, esa libreta como dato.
 *
 * **se explica con** TAD Diccionario(String, Conj(Registro))
 */
class BaseDeDatos::Indice {

public:
    /**
    * @brief Constructor de Indice
    *
    * \pre true
    * \post \P{res} = "indice vacio"
    *
    * \complexity {\O(1)}
    */
    Indice(){};

    /**
    * @brief Constructor de Indice con valor string
    *
    * @param campo nombre del campo sobre el que se crea el indice
    *
    * \pre true
    * \post \P{res} = "indice vacio con campo indexado"
    *
    * \complexity {\O(1)}
    */
    Indice(const string &campo);

    /**
    * @brief Constructor por copia de Indice
    *
    * @param indice valido a copiar
    *
    * \pre true
    * \post \P{res} = "indice parametro"
    *
    * \complexity {\O(1)}
    */
    Indice(const Indice&);
    Indice& operator=(const Indice &ind);


    /**
     * @brief conjunto de registros para un valor de Dato dado.
     *
     * @param dato nombre del campo sobre el que se crea el indice
     *
     * \pre dato esta definido en el indice.
     * \post \P{res} = "conjunto de registros que contienen al dato definidos en el indice"
     *
     * \complexity {\O(L)}
     */
    conjReg & registrosAsociados(const Dato &dato);

    /**
     * @brief modifica el indice, agregando un registro en el conjunto
     * de registros correspondiente al significado del dato.
     *
     * @param registro
     *
     * \pre campoindexado pertenece a registro.campos()
     * \post define el dato en registro en campoindexado y a registro en el map.
     *
     * \complexity{\O(L)}
     */
    void actualizarIndice(const Registro &registro);

    /**
     * @brief Analiza la igualdad de los indices
     *
     * @param Indice
     *
     * \pre true
     * \post true sii los indices son iguales.
     *
     * \complexity{\O(sn * (S + \CMP(T)))}
     */
    bool operator==(const Indice& otro) const;

    /**
     * @brief Analiza la igualdad de los indices
     *
     * @param Indice
     *
     * \pre true
     * \post true sii los indices son NO iguales.
     *
     * \complexity{\O(sn * (S + \CMP(T)))}
     */
    bool operator!=(const Indice& otro) const {return !(*this == otro);}

    /** @brief Acceso al conjunto de registros definido por un dato
    *  @param key clave a acceder, si no existe, se crea
    *  @returns una referencia a la definicion.
    *
    *  \pre true
    *  \post si la clave esta definida, devuelve el conjunto de registro, si no, define la clave y devuelve el conj vacio
    *
    *  \complexity{\O(S)}
    */
    conjReg &operator[](const string &key){
        return indice[key];
    }

    string_map< conjReg > indice;
    conjReg& vacia();
    conjReg listaVacia;

private:
    friend BaseDeDatos;
    string campoIndexado;
};
#endif //TP2_INDICE_H
