#ifndef join_iterator_h
#define join_iterator_h

#include "Tabla.h"
//No estoy seguro si habria que usar este iterator o el const_iterador_registros
#include <iterator>

public:

  //Constructor por copia
  join_iterator(join_iterator& it);

  //Operador para desreferenciar
  const Registro& operator*() const;

  //Operador flechita
  const Registro* operator->() const;

  //Operador avanzar
  //Este operador tiene que poder formar el registro que corresponda cuando se lo invoque
  //Preguntar esto
  join_ietrator& operator++();

  bool operator==(const join_iterator& o_it) const;

  bool operator!=(const join_iterator& o_it) const;


private:
  //friend class Tabla??

  //Es necesario tener un iterador que nos indique en que parte de las respectivas tablas estamos parados
  //Por ejemplo, al utilizar la funcion join, se devolveria un join_iterator apuntando al primero de los 
  //registros que se forman al combinar las 2 tablas, y al ir avanzando el iterador se devuelven los siguientes registros

  Tabla::const_iterator iteradorTabla1;

  Tabla::const_iterator iteradorTabla2;
  
  //el registro formado al combinar las 2 tablas
  Registro joinRegistro;


















#endif
