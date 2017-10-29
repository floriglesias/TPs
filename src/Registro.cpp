#include <iostream>
#include "Registro.h"
#include "utils.h"

using namespace std;

Registro::Registro(const vector<string>& campos, const vector<Dato>& datos) :
//TODO (Flor) : tiene que modificar esta implementación, pues _campos no existe más y _datos no tiene definida la operación fast_insert
    _campos(campos.begin(), campos.end()) {
        for (int i = 0; i < campos.size(); i++) {
            _datos.fast_insert(make_pair(campos[i], datos[i]));
        }
};


const Dato& Registro::dato(const string& campo) const {
    return _datos.at(campo);
}

const linear_set<string>& Registro::campos() const {
    //TODO (Flor) : tiene que modificar esta implementación pues _campos no existe más
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

ostream &operator<<(ostream &os, const Registro &r) {
    //TODO (Flor) : el string_map no tiene el operador <<. Por lo cual, no pueden implementar está operación de esta manera.
  os << r._datos;
  return os;
}
