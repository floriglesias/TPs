#include "BaseDeDatos.h"
#include "const_iterador_registros.h" // New
#include <list>
#include <tuple>
#include <algorithm>

BaseDeDatos::BaseDeDatos(){};

void BaseDeDatos::crearTabla(const string &nombre, 
                             const linear_set<string> &claves,
                             const vector<string> &campos,
                             const vector<Dato> &tipos) {
  _nombres_tablas.fast_insert(nombre);
  _tablas.fast_insert(make_pair(nombre, Tabla(claves, campos, tipos)));
}

void BaseDeDatos::agregarRegistro(const Registro &r, const string &nombre) {
  Tabla &t = _tablas.at(nombre);
  t.agregarRegistro(r);
}

const linear_set<string> &BaseDeDatos::tablas() const { return _nombres_tablas; }

const Tabla &BaseDeDatos::dameTabla(const string &nombre) const {
  return _tablas.at(nombre);
}

int BaseDeDatos::uso_criterio(const BaseDeDatos::Criterio &criterio) const {
  if (_uso_criterios.count(criterio)) {
    return _uso_criterios.at(criterio);
  } else {
    return 0;
  }
}

bool BaseDeDatos::registroValido(const Registro &r,
                                 const string &nombre) const {
  const Tabla &t = _tablas.at(nombre);

  return (t.campos() == r.campos() and _mismos_tipos(r, t) and
          _no_repite(r, t));
}

bool BaseDeDatos::_mismos_tipos(const Registro &r, const Tabla &t) const {
  for (auto c : t.campos()) {
    if (r.dato(c).esNat() != t.tipoCampo(c).esNat()) {
      return false;
    }
  }
  return true;
}

bool BaseDeDatos::_no_repite(const Registro &r, const Tabla &t) const {
  list<Registro> filtrados(t.registros().begin(), t.registros().end());
  for (auto clave : t.claves()) {
    _filtrar_registros(clave, r.dato(clave), filtrados);
  }
  return filtrados.empty();
}

list<Registro> &
BaseDeDatos::_filtrar_registros(const string &campo, const Dato &valor,
                                list<Registro> &registros) const {
  return _filtrar_registros(campo, valor, registros, true);
}

list<Registro> &BaseDeDatos::_filtrar_registros(const string &campo,
                                                const Dato &valor,
                                                list<Registro> &registros,
                                                bool igualdad) const {
  auto iter = registros.begin();
  while ( iter != registros.end()) {
    auto now = iter;
    iter++;
    if ((not igualdad) xor now->dato(campo) != valor) {
      registros.erase(now);
    }
  }

  return registros;
}

pair<vector<string>, vector<Dato> > BaseDeDatos::_tipos_tabla(const Tabla &t) {
  vector<string> res_campos;
  vector<Dato> res_tipos;
  for (auto c : t.campos()) {
    res_campos.push_back(c);
    res_tipos.push_back(t.tipoCampo(c));
  }
  return make_pair(res_campos, res_tipos);
}

bool BaseDeDatos::criterioValido(const Criterio &c,
                                 const string &nombre) const {
  const Tabla &t = _tablas.at(nombre);
  for (auto restriccion : c) {
    if (not t.campos().count(restriccion.campo())) {
      return false;
    }
    if (t.tipoCampo(restriccion.campo()).esNat() != 
        restriccion.dato().esNat()) {
      return false;
    }
  }
  return true;
}

Tabla BaseDeDatos::busqueda(const BaseDeDatos::Criterio &c,
                            const string &nombre) {
  if (_uso_criterios.count(c)) {
    _uso_criterios.at(c)++;
  } else {
    _uso_criterios.fast_insert(make_pair(c, 1));
  }

  const Tabla &ref = dameTabla(nombre);
  auto campos_datos = _tipos_tabla(ref);
  Tabla res(ref.claves(), campos_datos.first, campos_datos.second);
  list<Registro> regs(ref.registros().begin(), ref.registros().end());
  for (auto restriccion : c) {
    _filtrar_registros(restriccion.campo(), restriccion.dato(),
                       regs, restriccion.igual());
  }
  for (auto r : regs) {
    res.agregarRegistro(r);
  }
  return res;
}

linear_set<BaseDeDatos::Criterio> BaseDeDatos::top_criterios() const {
  linear_set<Criterio> ret;
  int max = 0;
  for (auto crit_count : _uso_criterios) {
    if (crit_count.second >= max) {
      if (crit_count.second > max) {
        ret = linear_set<Criterio>();
        max = crit_count.second;
      }
      ret.fast_insert(crit_count.first);
    }
  }
  return ret;
}

void BaseDeDatos::crearIndice(const string &nombre, const string &campo) { // Consultar
    Tabla t = dameTabla(nombre);
    if (t.tipoCampo(campo).esNat()) {
        auto it = t.registros_begin();
        while (it != t.registros_end()) {
            if (((_indicesInt.find(nombre).second).find(campo).second).count((*it).dato(campo)) == 1) {
                ((_indicesInt.find(nombre).second).find(campo).second).fast_insert((*it).dato(campo));
            } else {
                linear_set<Tabla::const_iterador_registros> l;
                l.fast_insert(*it);
                pair<Dato, linear_set<Tabla::const_iterador_registros> p = make_pair(campo, *it);
                dicc.insert(p);
            }
        }
    } else {
        auto it = t.registros_begin();
        while (it != t.registros_end()) {
            if (((_indicesString.find(nombre).second).find(campo).second).count((*it).dato(campo)) == 1) {
                ((_indicesString.find(nombre).second).find(campo).second).fast_insert((*it).dato(campo));
            } else {
                linear_set<Tabla::const_iterador_registros> l;
                l.fast_insert(it);
                pair<Dato, linear_set<Tabla::const_iterador_registros> p = make_pair(campo, *it);
                dicc.insert(p);
            }
        }
    }
/*
    string_map < linear_set < Tabla::const_iterador_registros > > s = string_map();
    auto it = t.registros_begin();
    while (it != t.registros_end()) {
        pair<Dato, Registro *> p = make_pair((*it).dato(c), *it->()); // convertir dato en string
        s.insert(p);
        ++it;
    }
*/
}

join_iterator join(const string &tabla1, const string &tabla2, const string &campo) const {
  //FALTA UN IF QUE DIVIDA ENTRE DATOS TIPO NAT Y TIPO STRING
  //COMPLETAR CUANDO SE ACTUALICE LA ESTRUCTURA


  //Encuentro cual de las tablas tiene indice para el campo en cuestion

  //Dado que no existen indices incompletos, puedo estar seguro de que si para el campo en cuestion
  //existe al menos un dato que esté indexado, entonces efectivamente la tabla tiene un indice para dicho campo


  if((indices.find(tabla1).second).find(campo).second.size() > 0) {
    
    //Busco el primer dato del campo en comun de la otra tabla (la que podria no tener indice), me fijo si coincide con el primer dato
    //de la tabla con indice y en caso de que esto suceda creo el correspondiente registro producto de la fusion de las dos tablas
   
    //Lo que esta a la izquierda es (teoricamente) el dato del campo en comun del primer registro de la tabla2
    //Lo que esta a la derecha es el primer valor del indice
    if( (*(tabla2.registros().begin())).dato(campo) == 
    (*((((indices.find(tabla1).second).find(campo).second).find(dato(campo)).first ) {
      
      //Si coinciden, creo el registro fusionado
      Registro regFusionado = *(tabla2.registros().begin());
      for(iterator it = tabla1.campos().begin(); it != tabla1.campos().end(); ++it) {
      regFusionado.campos().fast_insert(*it);
      //Falta agregar los datos de la tabla1...(como se hace?)

      }
      //Esta declaracion seguramente esta mal, pero la idea es que el join_iterator apunte al registro que se acaba de crear
      //Consultar sintaxis
      join_iterator registroUnion = regFusionado;

      return registroUnion;

    }

    else 
      //Seguir con el siguiente dato



  //Como la precondicion establece que al menos una tabla tiene indice para dicho campo, entonces podemos estar seguros
  //de que la otra tabla es la que esta indexada
  else 
     //Igual que el caso anterior pero con las tablas invertidas
     if( (*(tabla1.registros().begin())).dato(campo) == 
    (*((((indices.find(tabla2).second).find(campo).second).find(dato(campo)).first ) {
      
      //Si coinciden, creo el registro fusionado
      Registro regFusionado = *(tabla1.registros().begin());
      for(iterator it = tabla2.campos().begin(); it != tabla2.campos().end(); ++it) {
      regFusionado.campos().fast_insert(*it);
      //Falta agregar los datos de la tabla2...

      }

      join_iterator registroUnion = regFusionado;

      return registroUnion;

    }

    else 
      //Seguir con el siguiente dato

}








