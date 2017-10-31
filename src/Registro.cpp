#include <iostream>
#include "Registro.h"
#include "utils.h"

using namespace std;



Registro::Registro(const vector<string>& campos, const vector<Dato>& datos) : 
    _campos(campos.begin(), campos.end()) {
        for (int i = 0; i < campos.size(); i++) {
            _datos.fast_insert(std::pair<string, Dato>(campos[i], datos[i]));
        }
};

/*
 * DONE(Ivo): "Para las complejidades se debe tener en cuenta que se consideran que los nombres de tablas y los nombres
de campos son de largo acotado.", y tambien se lo consulte bien a March -> son O(1), solo los string datos son O(L).
 */
const Dato& Registro::dato(const string& campo) const {
    //return _datos[campo];
    return _datos.at(campo);
}

const linear_set<string>& Registro::campos() const {
    return _campos;
}

bool operator==(const Registro& r1, const Registro& r2) {
    linear_set<string> c1 = r1.campos();
    linear_set<string> c2 = r2.campos();
    if (not (c1 == c2)) {
        return false;
    }
    linear_set<string> campos = r1.campos();
    for (auto c : campos) {
        if (r1.dato(c) != r2.dato(c)) {
            return false;
        }
    }
    return true;
}

ostream &operator<<(ostream &os, const Registro &registro) {
  for_each(registro.campos().begin(), registro.campos().end(), [&](const string & campo){
      os << " { campo : " << campo << ", dato : " << registro.dato(campo) << "} ";
  });
  return os;
}

//FIXME se debe reemplazar por el algoritmo version final
Registro::Registro(const Registro& r1, const Registro& r2) {
    _campos = r1._campos; // O(#(r1.campos) * copy(campo))
    _datos = r1._datos; // O(#(r1.campos) * (S + copy(campo) + copy(dato)) )

    std::for_each(r2._campos.begin(), r2._campos.end(), [&](const string & campo){ // O(#(campos))
        if(_datos.count(campo) == 0){ // O(S)
            _campos.fast_insert(campo); // O(copy(campo))
            _datos.fast_insert(std::pair<string, Dato>(campo, r2._datos.at(campo))); // O(S + copy(campo) + copy(dato))
        }
    });

    // O(#(campos) * (S + copy(campo) + copy(dato)) )
}
