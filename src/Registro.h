#ifndef _REGISTRO_H
#define _REGISTRO_H

#include <vector>
#include <iostream>
#include "Dato.h"
#include "linear_set.h"
#include "linear_map.h"
#include "string_map.h"

using namespace std;

/**
 * @brief Representa un registro de una tabla.
 *
 * Un registro asocia campos identificados con un string con valores
 * específicos.
 *
 * **se explica con** TAD Diccionario(string, Dato)
 */
class Registro {

public:

    /**
     * @brief Genera un nuevo registro con los campos y valores designados.
     *
     * \pre long(campos) = long(datos)
     * \post \P{res} = nuevoRegistro(campos, datos)
     *
     * \complexity{\O(long(campos) * (copy(campo) + copy(dato)))}
     */
    Registro(const vector<string>& campos, const vector<Dato>& datos);

    /**
     * @brief Genera un nuevo registro fusionando los dos registros pasados por parametro.
     *
     * No genera aliasing
     *
     * \pre true
     * \post campos(\P{res}) = campos(r1) U campos(r2) \LAND_L ( (\FORALL c1,c2: string) (c1 \IN campos(r1) \LAND c2 \IN campos(r2)) \IMPLIES_L valor(c1, r1) = valor(c1, \P{res}) \AND valor (c2, r2) = valor(c2, \P{res}) )
     *
     * \complexity{\O((#(campos(r1)) + #(campos(r2))) + (S * (copy(campo) + copy(dato))) )}
     */
    Registro(const Registro& r1, const Registro& r2);

    /**
     * @brief Devuelve el dato asociado a un campo.
     *
     * Devuelve el dato por referencia no modificable.
     *
     * \pre campo \in campos(\P{this})
     * \post \P{res} = valor(campo, \P{this})
     * 
     * \complexity{\O(long(campos(\P{this})) * cmp(campo))}
     */
    const Dato& dato(const string& campo) const;

    /**
     * @brief Devuelve los campos definidos en un registro
     *
     * El conjunto de campos se devuelve por referencia
     *
     * \pre true
     * \post \P{res} = campos(\P{this})
     *
     * \complexity{\O(1)}
     */
    const linear_set<string>& campos() const;

private:
	  ///////////////////////////////////////////////////////////////////////////////////////////////////
    /** \name Representación
     * rep: registro \TO bool\n
     * rep(d) \EQUIV 
     *  * _campos = claves(_datos)
     *
     * abs: registro \TO Registro\n
     * abs(r) \EQUIV r' \|
     *  * campos(r') = _campos \LAND
     *  * \FORALL (c : string) c \in _campos \IMPLIES valor(c, r') = valor(c,
     *    _datos) 
     */
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    /** @{ */
    linear_set<string> _campos;
    string_map<Dato> _datos;
    //string_map<Dato> _datos;
    /** @} */

    friend ostream &operator<<(ostream &, const Registro &);
};


bool operator==(const Registro&, const Registro&);
ostream &operator<<(ostream &, const Registro &);


#endif //_REGISTRO_H
