#ifndef _BASEDEDATOS_H
#define _BASEDEDATOS_H

#include "Registro.h"
#include "Restriccion.h"
#include "Tabla.h"
#include <utility>
#include <list>
#include <string>
#include "linear_map.h"
#include "linear_set.h"
#include "utils.h"

using namespace std;
template<typename T>
using strmap = string_map< T >;
/**
 * @brief Una base de datos es un administrador de tablas con funciones de
 * búsqueda.
 *
 * Una base de datos permite administrar tablas identificadas por registro.
 * Permite saber si se puede agegar un registro a una tabla y luego agregarlo.
 * Permite realizar filtros del contenido de tablas mediante criterios de
 * búsqueda. Además mantiene estadísticas del uso de los criterios.
 *
 * **se explica con** TAD BaseDeDatos
 */
class BaseDeDatos {

public:
    using value_type      = Registro;
    /**
     * Estructura para la relacion entre la tabla del lado izquierdo (por convencion, la que no tiene indice en la BBDD)
     * y la tabla del lado derecho (por convencion, la que tiene indice en la BBDD).
     *
     * Por lo tanto, por cada una de las relaciones, entre las dos tablas, guardamos el registro de la tabla izquierda y
     * guardamos el [inicio, fin) del iterador del conjunto asociado a la tabla derecha.
     *
     * Esquematicamente, la relacion se refleja de la siguiente manera (ver el ejemplo que da el trabajo practico)
     *
     * {
     *  registro: {
     *      LU: "321/15",
     *      Nombre: "Amanda",
     *      Nacimiento: 1955
     *  },
     *  from: [{
     *          LU: "123/23",
     *          Materia: "Algo 3"
     *      },
     *      {
     *          LU: "123/23",
     *          Materia: "SisOp"
     *      }
     *  ]
     *  to : null
     * }
     *
     * Tabla Alumnos NO tiene indice y Tabla Materias SI tiene indice
     */
    struct Relationship {
        /**
         * Inicio de la coleccion del registro de la tabla derecha
         */
        typename linear_set<Registro>::iterator from;
        /**
         * Fin de la coleccion del registro de la tabla derecha
         */
        typename linear_set<Registro>::iterator to;
        /**
         * Registro de la tabla izquierda
         */
        Registro registro;
        /**
         * Constructor de la relacion
         */
        Relationship(const Registro &registro, typename linear_set<Registro>::iterator from, typename linear_set<Registro>::iterator to) : registro(registro), from(from), to(to){

        }
    };
    /**
     * Alias para la tabla resultante del join
     */
    using table           = std::list< Relationship >;
    /**
     * Iterador para la tabla resultante del join
     */
    using table_join      = typename table::iterator;

    class join_iterator {
    public :

        using value_type = BaseDeDatos::value_type ;
        using iterator_category = std::input_iterator_tag;
        using reference = value_type &;
        using pointer = value_type *;
        using difference_type = std::ptrdiff_t;
        /**
         * @brief Operador asignacion
         * \pre ninguna
         * \post genera una nueva instancia
         * \complexity{\O(1)}
         */
        join_iterator operator=(const join_iterator &other) {
            //region O = O(1) + O(1) = O(1)
            //region O = O(1)
            inner_join = other.inner_join;
            //endregion
            //region O = O(1) + O(1) = O(1)
            if(inner_join != nullptr){
                //region O = O(1)
                it = other.it;
                //endregion
                //region O = O(1)
                end = other.end;
                //endregion
            }
            //endregion
            //endregion
        }
        /**
         * @brief constructor por copia
         * \pre ninguna
         * \post pisa los datos existentes por el parametro pasado
         * \complexity{\O(1)}
         */
        join_iterator(const join_iterator &other) {
            //region O = O(1)
            (*this) = other;
            //endregion
        }
        /**
         * @brief constructor por defecto
         * \pre ninguna
         * \post ninguna
         * \complexity{\O(1)}
         */
        join_iterator() : inner_join(nullptr) {

        }
        /**
         * @brief destructor por defecto
         * \pre el objeto debe estar vivo
         * \post el objeto es destruido
         */
        ~join_iterator(){
            delete inner_join;
            inner_join = nullptr;
        }
        /**
         * @brief constructor del iterador
         * \pre join NO debe ser nulo
         * \post genera una nueva instancia
         * \complexity{\O(1)}
         */
        join_iterator(table *join) : inner_join(join) {
            //region O = O(1)
            it = join->begin();
            //endregion
            //region O = O(1)
            end = join->end();
            //endregion
        }
        /**
         * @brief Avanza el iterador una posición.
         *
         * \pre El iterador no debe estar en la posición pasando-el-último.
         * \post \P{res} es una referencia a \P{this}. \P{this} apunta a la posición
         * siguiente.
         *
         * \complexity{\O(1)}
         */
        join_iterator &operator++(){
            //region O = O(1)
            Relationship & relationship = (*it);
            //endregion
            //region O = O(1)
            if(relationship.from != relationship.to){
                ++ relationship.from;
                if(relationship.from != relationship.to){
                    return *(new join_iterator(*this));
                } else {
                    if(it != end){
                        ++it;
                        if(it != end){
                            return *(new join_iterator(*this));
                        } else {
                            inner_join = nullptr;
                            return *(new join_iterator);
                        }
                    }
                }
            } else {
                if(it != end){
                    ++it;
                    if(it != end){
                        return *(new join_iterator(*this));
                    } else {
                        inner_join = nullptr;
                        return *(new join_iterator);
                    }
                }
            }
            //endregion
        }
        /**
         * @brief Avanza el iterador una posición.
         *
         * \pre El iterador no debe estar en la posición pasando-el-último.
         * \post \P{res} es una referencia a \P{this}. \P{this} apunta a la posición
         * siguiente.
         *
         * \complexity{\O(1)}
         */
        join_iterator &operator++(int){
            //region O = O(1)
            Relationship & relationship = (*it);
            //endregion
            //region O = O(1)
            if(relationship.from != relationship.to){
                ++ relationship.from;
                if(relationship.from != relationship.to){
                    return *(new join_iterator(*this));
                } else {
                    if(it != end){
                        ++it;
                        if(it != end){
                            return *(new join_iterator(*this));
                        } else {
                            inner_join = nullptr;
                            return *(new join_iterator);
                        }
                    }
                }
            } else {
                if(it != end){
                    ++it;
                    if(it != end){
                        return *(new join_iterator(*this));
                    } else {
                        inner_join = nullptr;
                        return *(new join_iterator);
                    }
                }
            }
            //endregion
        }
        /**
         * @brief Desreferencia el puntero
         *
         * El valor devuelto tiene aliasing dentro de la colección.
         *
         * \pre El iterador no debe estar en la posición pasando-el-último.
         * \post El valor resultado es una referencia al valor apuntado.
         *
         * \complexity{\O(copy(Registro))}
         */
        const value_type &operator*() const{
            //region O = O(1)
            Relationship & relationship = *(it);
            //endregion
            //region O = O(1)
            const Registro &first  = relationship.registro;
            //endregion
            //region O = O(1)
            const Registro &second = *relationship.from;
            //endregion
            //region O = O(copy(Registro))
            return *(new Registro(first, second));
            //endregion
        }
        /**
         * @brief Operador flechita
         *
         * El valor devuelvo tiene aliasing dentro de la colección.
         *
         * \pre El iterador no debe estar en la posición pasando-el-último.
         * \post El valor resultado es un puntero al valor apuntado.
         *
         * \complexity{\O(copy(Registro))}
         */
        const value_type *operator->() const{
            //region O = O(1)
            Relationship & relationship = *(it);
            //endregion
            //region O = O(1)
            Registro first  = relationship.registro;
            //endregion
            //region O = O(1)
            Registro second = *relationship.from;
            //endregion
            //region O = O(copy(Registro))
            return new Registro(first, second);
            //endregion
        }

        /**
         * @brief Comparación entre iteradores
         *
         * \pre ambos iteradores refieren a la misma colección
         * \post true sii los iteradores apuntan al mismo elemento
         *
         * \complexity{\O(1)}
         */
        bool operator==(const join_iterator &other) const{
            return (inner_join == other.inner_join);
        }

        /**
         * @brief Comparación entre iteradores
         *
         * \pre ambos iteradores refieren a la misma colección
         * \post true sii los iteradores no apuntan al mismo elemento
         *
         * \complexity{\O(1)}
         */
        bool operator!=(const join_iterator &other) const{
            return (inner_join != other.inner_join);
        }


    private:
        /**
         * Contiene la referencia a la cual estoy iterando. Es necesaria debido a que debemos comparar que estamos
         * iterando la misma estructura y ya que esto es una referencia temporal, debemos liberar la memoria
         */
        table *inner_join;

        table_join it;
        table_join end;
    };
    // Forward declaration
    class Indice;


  /** @brief Criterio de búsqueda para una base de datos */
  typedef linear_set<Restriccion> Criterio;

  /**
   * @brief Inicializa una base de datos sin tablas.
   *
   * \pre true
   * \post \P{this} = nuevaDB
   *
   * \complexity{\O(1)}
   */
  BaseDeDatos();

  /**
   * @brief Crea una nueva tabla en la base de datos.
   *
   * @param nombre Nombre identificador de la tabla
   * @param claves Claves de la tabla a crear
   * @param campos Campos de la tabla a crear
   * @param tipos  Tipos para los campos de la tabla a crear
   *
   * \pre db = \P{this} \LAND
   *      \LNOT (nombre \IN tablas(\P{this})) \LAND 
   *      \LAND \LNOT \EMPTYSET?(claves) \LAND
   *      \FORALL (c: campo) c \IN claves \IMPLICA c \IN campos \LAND 
   *      long(campos) = long(tipos) \LAND sinRepetidos(campos)
   * \post \P{this} = agregarTabla(nuevaTabla(claves, nuevoRegistro(campos, tipos)), db)
   *
   * \complexity{\O(C)}
   */
  void crearTabla(const string &nombre, const linear_set<string> &claves,
                  const vector<string> &campos, const vector<Dato> &tipos);

  /**
   * @brief Crea un índice de la tabla sobre el campo pasados por parámetro
   *
   * @param nombre Nombre de la tabla donde crear el indice
   * @param campo Nombre del campo sobre el cual crear el indice
   *
   * \pre nombre \IN tablas(\P{this}) \LAND campo \IN campos(dameTabla(nombre, \P{this}))
   * \post \P{res} = crearIndice(nombre, campo, \P{this})
   *
   * \complexity{\O(1)}
   */
  void crearIndice(const string &nombre, const string &campo);

  /**
   * @brief Devuelve true si existe un indice para la tabla por el campo pasados por parámetro
   *
   * @param nombre Nombre de la tabla donde crear el indice
   * @param campo Nombre del campo sobre el cual crear el indice
   *
   * \pre true
   * \post \P{res} = tieneIndice?(nombre, campo, \P{this})
   *
   * \complexity{\O(1)}
   */
  bool tieneIndice(const string &nombre, const string &campo);

    //TODO: corregir esta interface en función de las modificaciones hechas.
  /**
   * @brief Agrega un registro a la tabla parámetro
   *
   * @param r Registro a agregar
   * @param nombre Nombre de la tabla donde se agrega el registro
   *
   * \pre db = \P{this} \LAND nombre \IN tablas(\P{this}) \LAND 
   *      puedoInsertar?(r, dameTabla(\P{this}))
   * \post \P{this} = insertarEntrada(r, nombre, db)
   *
   * \complexity{\O(\COPY(Registro))}
   */
  void agregarRegistro(const Registro &r, const string &nombre);

  /**
   * @brief Devuelve el conjunto de tablas existentes en la base.
   *
   * El conjunto de nombres se devuelve por referencia no-modificable.
   *
   * \pre true
   * \post \P{res} = tablas(\P{this})
   *
   * \complexity{\O(1)}
   */
  const linear_set<string> &tablas() const;

  /**
   * @brief Devuelve la tabla asociada al nombre.
   *
   * La tabla se devuelve por referencia no modificable.
   *
   * @param nombre Nombre de la tabla buscada.
   *
   * \pre nombre \IN tablas(\P{this})
   * \post \P{res} = dameTabla(nombre, \P{this})
   *
   * \complexity{O(1)}
   */
  const Tabla &dameTabla(const string &nombre) const;

  /**
   * @brief Devuelve la cantidad de usos que tiene un criterio
   *
   * @param criterio Criterio por el cual se consulta.
   *
   * \pre nombre \IN tablas(\P{this})
   * \post \P{res} = usoCriterio(criterio, \P{this})
   *
   * \complexity{\O(cs * \CMP(Criterio))}
   */
  int uso_criterio(const Criterio &criterio) const;

  /**
   * @brief Evalúa si un registro puede ingresarse en la tabla parámetro.
   *
   * @param r Registro a ingresar en la tabla.
   * @param nombre Nombre de la tabla.
   *
   * \pre nombre \IN tablas(\P{this})
   * \post \P{res} = puedoInsertar?(r, dameTabla(nombre, \P{this}))
   *
   * \complexity{\O(\CMP(linear_set) + C + (c * n * L))}
   */
  bool registroValido(const Registro &r, const string &nombre) const;

  /**
   * @brief Evalúa si un criterio puede aplicarse en la tabla parámetro.
   *
   * @param c Criterio a utilizar.
   * @param nombre Nombre de la tabla.
   *
   * \pre tabla \IN tablas(\P{this})
   * \post \P{res} = criterioValido(c, nombre, \P{this})
   * 
   * \complexity{\O(cr)}
   */
  bool criterioValido(const Criterio &c, const string &nombre) const;

  /**
   * @brief Devuelve el resultado de buscar en una tabla con un criterio.
   *
   * @param c Criterio de búsqueda utilizado.
   * @param nombre Nombre de la tabla.
   *
   * \pre nombre \IN tablas(\P{this}) \LAND criterioValido(c, nombre, \P{this}) 
   * \post \P{res} = buscar(c, nombre, \P{this})
   *
   * \complexity{\O(cs * \CMP(Criterio) + cr * n * (C + L + \COPY(Registro)))}
   */
  Tabla busqueda(const Criterio &c, const string &nombre);

  /**
   * @brief Devuelve los criterios de máximo uso.
   *
   * \pre true
   * \post \FORALL (c : Criterio) [c \IN \P{res} \IFF 
   *       \FORALL (c' : Criterio) usoCriterio(c, db) >= usoCriterio(c', db)]
   *
   * \complexity{\O(cs * \COPY(Criterio))}
   */
  linear_set<Criterio> top_criterios() const;

  /**
   * @brief Devuelve el índice de la tabla por el campo pasados por parámetros
   *
   * \pre tieneIndice?(nombre, campo, \P{this})
   * \post \P{res} = obtener(campo, obtener(nombre, _indices))
   * 
   * \complexity{\O(1)}
   */
    const Indice &dameIndice(const string &tabla,const string &campo) const;

  /**
   * @brief Devuelve un iterador para recorrer los registros que da como resultado hacer el join de las tablas pasadas como parámetro. El iterador apunta a la primer posición
   *
   * \pre tabla1 \IN tablas(\P{this}) \LAND tabla2 \IN tablas(\P{this}) \LAND campo \IN campos(dameTabla(tabla1, \P{this})) \LAND campo \IN campos(dameTabla(tabla2, \P{this})) \LAND_L (tieneIndice?(tabla1, campo, \P{this}) \LOR tieneIndice?(tabla2, campo, \P{this}))
   * \post \P{res} = join(tabla1, tabla2, campo, \P{this})
   * 
   * \complexity{\O(n * L)}
   */
    join_iterator join(const string &tabla1, const string & tabla2, const string &campo);

  /**
   * @brief Devuelve un iterador para recorrer los registros que da como resultado hacer un join. El iterador apunta a la posición pasando-el-último
   *
   * \pre true
   * \post \P{res} apunta a la posición pasando-el-último
   * 
   * \complexity{\O(1)}
   */
    join_iterator join_end();

private:


    join_iterator join(const linear_set<Registro> registros, Indice & indice, const string &campo);
	  ///////////////////////////////////////////////////////////////////////////////////////////////////
    /** \name Representación
     * rep: basededatos \TO bool\n
     * rep(bd) \EQUIV 
     *  * _nombres_tablas = claves(_tablas) \LAND
     *  * \FORALL (c : Criterio) c \IN claves(_uso_criterios) \IMPLIES 
     *     * (
     *       * \EXISTS (n : string) n \IN _nombres_tablas 
     *       * \LAND criterioValido(c, n, db)
     *     * ) \LAND
     *     * obtener(c, _uso_criterios) > 0
     *
     * abs: basededatos \TO BaseDeDatos\n
     * abs(bd) \EQUIV bd' \|
     *  * _nombres_tablas = tablas(bd') \LAND
     *  * (\FORALL nt : string) nt \IN _nombres_tablas \IMPLIES
     *    * obtener(nt, _tablas) = dameTabla(nt, bd') \LAND
     *  * (\FORALL c : criterio) 
     *    * (usoCriterio(c, bd') == 0 \LAND \LNOT def?(c, _uso_criterios)) \LOR
     *    * (usoCriterio(c, db') == obtener(c, _uso_criterios))
     */
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    /** @{ */
    linear_set<string> _nombres_tablas;
    string_map<Tabla> _tablas;
    linear_map<Criterio, int> _uso_criterios;
    //el primer string= nombreTabla, segundo string = nombreCampoAIndexar
    string_map< string_map<Indice> > _indices;
    /** @} */

    /** @{ */
    /**
     * @brief Revisa si los campos del registro y la tabla tienen el mismo tipo.
     *
     * \pre campos(r) == campos(t)
     * \post \P{res} == \FORALL (c : campo) c \IN campos(r) \IMPLIES
     * Nat?(valor(c, r)) == tipoCampo(c, t)
     *
     * \complexity{O(C)}
     */
    bool _mismos_tipos(const Registro &r, const Tabla &t) const;

    /**
     * @brief Revisa si el registro no repite claves en la tabla.
     *
     * \pre compatible(r, t)
     * \post \P{res} = \FORALL (r' : Registro) r \IN registros(t) \IMPLIES
     *  \EXISTS (c : campo) c \IN claves(t) \LAND valor(c, r') != valor(c, r)
     *
     * \complexity{O(c * n * L)}
     */
    bool _no_repite(const Registro &r, const Tabla &t) const;

    /**
     * @brief Filtra la lista de registros parametro según el criterio.
     *
     * El resultado tiene aliasing con el parámetro registros.
     *
     * \pre \FORALL (r : Registro) r \IN registros \IMPLIES campo \IN
     *      campos(r) \LAND tipo?(valor(campo, r)) = tipo?(valor)
     * \post \P{res} = filtrarRegistrosSegunRestriccion(
     *       nueva(campo, valor, igualdad), registros)
     */
    list<Registro> &_filtrar_registros(const string &campo, const Dato &valor,
                                      list<Registro> &registros,
                                      bool igualdad) const;

    /**
     * @brief Filtra la lista de registros parametro según el criterio.
     *
     * El resultado tiene aliasing con el parámetro registros.
     *
     * \pre \FORALL (r : Registro) r \IN registros \IMPLIES campo \IN
     *      campos(r) \LAND tipo?(valor(campo, r)) = tipo?(valor)
     * \post \P{res} = filtrarRegistrosSegunRestriccion(
     *       nueva(campo, valor, true), registros)
     */
    list<Registro> &_filtrar_registros(const string &campo, const Dato &valor,
                                       list<Registro> &registros) const;

    /**
     * @brief Obtiene los campos y tipos de una tabla.
     *
     * \pre true
     * \post (\FORALL (c : Campo) está?(c, \P1(\P{res})) \IFF c \IN campos(t))
     *       \LAND #(campos(t)) = long(\P1(\P{res}))
     *       \LAND \FORALL (i : Nat) 0 \LEQ i < #(campos(t)) \IMPLIES
     *       tipo?(\P2(\P{res})[i]) = tipoCampo(\P1(\P{res})[i], t)
     */
    pair<vector<string>, vector<Dato> > _tipos_tabla(const Tabla &t);
    /** @} */
};

#include "Indice.h"

#endif
