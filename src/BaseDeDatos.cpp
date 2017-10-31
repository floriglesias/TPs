#include "BaseDeDatos.h"
#include <list>
#include <tuple>
#include <algorithm>


BaseDeDatos::BaseDeDatos(){};

void BaseDeDatos::crearTabla(const string &nombre, 
                             const linear_set<string> &claves,
                             const vector<string> &campos,
                             const vector<Dato> &tipos) {
  _nombres_tablas.fast_insert(nombre);
  _tablas.fast_insert(std::pair<string, Tabla>(nombre, Tabla(claves, campos, tipos)));
}

bool BaseDeDatos::tieneIndice(const string &nombre, const string &campo){
    //region O(1) * O(1) = O(1)
    return (_indices.count(nombre) == 1) and (_indices.at(nombre).count(campo)==1);
    //endregion
}

//DONE Complejidad pedida: O(m*[L + logm])
void BaseDeDatos::crearIndice(const string &nombre, const string &campo){
    //region O(m)*(O(L)*(O(L)+O(copyConjReg))) + O(copyIndice) + O(string_map<Indice>) = ... //TODO!!!
  //region O(1)
  if(tieneIndice(nombre,campo)) return;
  const Tabla &t1  = _tablas.at(nombre);
  Indice ind(campo);
  //endregion
  //region O(m)*O(actualizarIndice) = O(m)*(O(L)*(O(L)+O(copyConjReg))) //TODO mejorar
  if(t1.registros().size() != 0){    //O(1)
    for(Registro r : t1.registros()){
      ind.actualizarIndice(r);
    }
  }
  //endregion
  //region O(copyIndice) + O(copy string_map<Indice>)    //TODO ¡mejorar con urgencia!
  if(_indices.count(nombre)==0){
      //region O(1) + O(std::pair<string, Indice>) + O(std::pair<string, string_map<Indice> >)
    string_map<Indice> s;
    s.fast_insert(std::pair<string, Indice>(campo,ind));
    _indices.fast_insert(std::pair<string, string_map<Indice> >(nombre, s));
      //endregion
  } else {
      //region O(1) + O(1) + O(std::pair<string, Indice >)
    _indices[nombre].fast_insert(std::pair<string, Indice >(campo,ind));
      //endregion
  }
  //endregion
    //endregion
}

//Piden que sea O(copyReg + C* [L + log(m)])
void BaseDeDatos::agregarRegistro(const Registro &r, const string &nombre) {
//region O(copyReg + C*[L(L+copyConjReg)]
    //region O(1) + O(copyReg)
  Tabla &t = _tablas.at(nombre);
  t.agregarRegistro(r);
    //endregion
    //region O(C)*(O(1)+O(actualizarIndice)) = O(C*[L(L +copyConjReg)])
  for(auto campo : t.campos()){
    if(tieneIndice(nombre, campo)){
      _indices[nombre][campo].actualizarIndice(r);
    }
  }
    //endregion
//endregion
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

//TODO: revisar cuentas de complejidad para este metodo...
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

//TODO: en comentarios aclaro la idea de como hacerla O(cr).
bool BaseDeDatos::criterioValido(const Criterio &c,
                                 const string &nombre) const {
  const Tabla &t = _tablas.at(nombre);  //O(1) string_map
  for (auto restriccion : c) {  //O(cr)*, y lo de adentro tiene que quedar O(1)
    //TODO: verificar esto:
      // esta queda O(1) haciendo finds con los strings de campo sobre el trie que corresponda...
    if (not t.campos().count(restriccion.campo())) {
      return false;
    }
    //la complejidad de esta parte viene del acceso dado por tipoCampo.
    //se vuelve O(1) al cambiar string_map por linear_map
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


typename BaseDeDatos::join_iterator BaseDeDatos::join_end() {
    //region O = O(1)
    return join_iterator();
    //endregion
};


typename BaseDeDatos::join_iterator BaseDeDatos::join(const string &tabla1, const string & tabla2, const string &campo){
  Tabla & _tabla1 = _tablas.at(tabla1);
  Tabla & _tabla2 = _tablas.at(tabla2);
  /**
   * Caso 1, del lado izquierdo no tenemos registros
   * me voy con el end()
   */
  //region O = O(1) + O(1) = O(1)
  if(_tabla1.registros().size() == 0){
    //region O = O(1)
    return join_iterator();
    //endregion
  }
  //endregion
  /**
   * Caso 2, del lado derecho no tenemos registros
   * me voy con el end()
   */
  //region O = O(1) + O(1) = O(1)
  if(_tabla2.registros().size() == 0){
    //region O = O(1)
    return join_iterator();
    //endregion
  }
  //endregion
  /**
   * Caso 3, al menos uno de los dos tiene que tener indice, asi que se puede utilizar el join
   */
  //region O = O(1) + O(m * L) = O(m * L)
  if(tieneIndice(tabla1, campo)){
      //region O = O(m * L)
      return join(_tabla2.registros(), _indices[tabla1][campo], campo);
      //endregion
  } else {
      //region O = O(m * L)
      return join(_tabla1.registros(), _indices[tabla2][campo], campo);
      //endregion
  }
  //endregion
}

typename BaseDeDatos::join_iterator BaseDeDatos::join(const linear_set<Registro> registros, Indice &indice, const string &campo) {
//region O = O(1) + O(m * L) + O(1) = O(m * L)
  //region O = O(1)
  table *inner_join = new std::list< Relationship >();
  //endregion
  //region O = m * O(L) = O(m * L)
  std::for_each(registros.begin(), registros.end(), [&](const Registro& registro){
      //region O = O(L)
      linear_set<Registro> &join = indice.registrosAsociados(registro.dato(campo));
      //endregion
      //region O = O(1) + O(1) = O(1)
      if(!join.empty()){
          //region O = O(1)
          //region O = O(1) + O(1) + O(1) + O(1) = O(1)
          Relationship relationship(registro, join.begin(), join.end());
          //endregion
          //region O = O(1)
          inner_join->push_back(relationship);
          //endregion
          //endregion
      }
      //endregion
  });
  //endregion
  //region O = O(1)
  return join_iterator(inner_join);
  //endregion
//endregion
}

const BaseDeDatos::Indice &BaseDeDatos::dameIndice(const string &tabla, const string &campo) const {
    //region O(1)
  return _indices.at(tabla).at(campo);
    //endregion O(1)
}
