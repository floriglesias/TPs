#include "string_map.h"
template<class T>
class string_map<T>::iterator
{
public:

    string_map<T>::iterator();

    string_map<T>::iterator(const string_map<T>::iterator& otro);

    string_map<T>::const_iterator& operator = (const string_map<T>::const_iterator& otro);

    bool operator == (const string_map<T>::const_iterator&) const;

    bool HaySiguiente() const;
    bool HayAnterior() const;
    const T& SiguienteClave() const;
    Nodo& SiguienteSignificado();
    const T& AnteriorClave() const;
    Nodo& AnteriorSignificado();
    Elem Siguiente();
    Elem Anterior();
    void Avanzar();
    void Retroceder();
    void EliminarSiguiente();
    void EliminarAnterior();

private:

    typename vector<string>::iterator it_claves_;
    typename vector<T>::iterator it_significados_;

    string_map<T>::iterator()(string_map<T>* t);

    friend typename Dicc<K,S>::Iterador Dicc<K,S>::CrearIt();
    friend class Dicc<K, S>::const_Iterador;
};

class const_Iterador
{
public:

    const_Iterador();

    const_Iterador(const typename Dicc<K,S>::Iterador& otro);

    const_Iterador(const typename Dicc<K, S>::const_Iterador& otro);

    const_Iterador& operator = (const typename Dicc<K, S>::const_Iterador& otro);

    bool operator==(const typename Dicc<K,S>::const_Iterador&) const;

    bool HaySiguiente() const;
    bool HayAnterior() const;
    const K& SiguienteClave() const;
    const S& SiguienteSignificado() const;
    const K& AnteriorClave() const;
    const S& AnteriorSignificado() const;
    const_Elem Siguiente() const;
    const_Elem Anterior() const;
    void Avanzar();
    void Retroceder();

private:

    typename Lista<K>::const_Iterador it_claves_;
    typename Lista<S>::const_Iterador it_significados_;

    const_Iterador(const Dicc<K,S>* d);

    friend typename Dicc<K,S>::const_Iterador Dicc<K,S>::CrearIt() const;
};

struct Elem
{
public:

    const K& clave;
    S& significado;

    Elem(const K& c, S& s) : clave(c), significado(s) {}
    //Para sacar esto de aca, necesitariamos definir rasgos y otras yerbas
    //Lamentablemente, sino C++ no reconoce bien los tipos

    friend std::ostream& operator<<(std::ostream& os, const Dicc<K,S>::Elem& e) {
        return os << e.clave << ":" << e.significado;
    }

private:

    typename Dicc<K,S>::Elem& operator=(const Dicc<K,S>::Elem&);
};

struct const_Elem
{
public:

    const K& clave;
    const S& significado;

    const_Elem(const K& c, const S& s) : clave(c), significado(s) {}

    //Para sacar esto de aca, necesitariamos definir rasgos y otras yerbas
    //Lamentablemente, sino C++ no reconoce bien los tipos
    friend std::ostream& operator << (std::ostream& os, const Dicc<K,S>::const_Elem& e) {
        return os << e.clave << ":" << e.significado;
    }

private:

    typename Dicc<K,S>::const_Elem& operator = (const Dicc<K,S>::const_Elem&);
};

private:

Lista<K> claves_;
Lista<S> significados_;

};

template<class K, class S>
std::ostream& operator << (std::ostream &os, const Dicc<K,S>& d);

template<class K, class S>
bool operator == (const Dicc<K,S>& d1, const Dicc<K,S>& d2);

// Implementacion Dicc

template<class K, class S>
Dicc<K,S>::Dicc()
{}

template<class K, class S>
Dicc<K,S>::Dicc(const Dicc<K,S>& otro)
        : claves_(otro.claves_), significados_(otro.significados_)
{}

template<class K, class S>
Dicc<K,S>& Dicc<K,S>::operator = (const Dicc<K,S>& otro)
{
    claves_ = otro.claves_;
    significados_ = otro.significados_;

    return *this;
}

template<class K, class S>
typename Dicc<K,S>::Iterador Dicc<K,S>::Definir(const K& clave, const S& significado)
{
    Iterador it = Buscar(clave);

    if(it.HaySiguiente()) {
        it.SiguienteSignificado() = significado;
    } else {
        it = DefinirRapido(clave, significado);
    }

    return it;
}

template<class K, class S>
typename Dicc<K,S>::Iterador Dicc<K,S>::DefinirRapido(const K& clave, const S& significado)
{
#ifdef DEBUG
    assert( not Definido(clave) );
#endif

    claves_.AgregarAdelante(clave);
    significados_.AgregarAdelante(significado);

    return CrearIt();
}

template<class K, class S>
bool Dicc<K,S>::Definido(const K& clave) const
{
    return Buscar(clave).HaySiguiente();
}

template<class K, class S>
const S& Dicc<K,S>::Significado(const K& clave)const
{
#ifdef DEBUG
    assert( Definido(clave) );
#endif

    return Buscar(clave).SiguienteSignificado();
}

template<class K, class S>
S& Dicc<K,S>::Significado(const K& clave)
{
#ifdef DEBUG
    assert( Definido(clave) );
#endif

    return Buscar(clave).SiguienteSignificado();
}

template<class K, class S>
void Dicc<K,S>::Borrar(const K& clave)
{
#ifdef DEBUG
    assert( Definido(clave) );
#endif

    Buscar(clave).EliminarSiguiente();
}

template<class K, class S>
Nat Dicc<K,S>::CantClaves() const
{
    return claves_.Longitud();
}

template<class K, class S>
typename Dicc<K,S>::Iterador Dicc<K,S>::CrearIt()
{
    return Iterador(this);
}

template<class K, class S>
typename Dicc<K,S>::const_Iterador Dicc<K,S>::CrearIt() const
{
    return const_Iterador(this);
}

// Implementacion Iterador

template<class K, class S>
Dicc<K,S>::Iterador::Iterador()
{}

template<class K, class S>
Dicc<K,S>::Iterador::Iterador(const typename string_map<T>::const_iterator& otro)
        : it_claves_(otro.it_claves_), it_significados_(otro.it_significados_)
{}

template<class K, class S>
typename Dicc<K,S>::Iterador& Dicc<K,S>::Iterador::operator = (const typename string_map<T>::const_iterator& otro)
{
    it_claves_ = otro.it_claves_;
    it_significados_ = otro.it_significados_;

    return *this;
}

template<class K, class S>
bool Dicc<K,S>::Iterador::HaySiguiente() const
{
    return it_claves_.HaySiguiente();
}

template<class K, class S>
bool Dicc<K,S>::Iterador::HayAnterior() const
{
    return it_claves_.HayAnterior();
}

template<class K, class S>
const K& Dicc<K,S>::Iterador::SiguienteClave() const
{
#ifdef DEBUG
    assert(HaySiguiente());
#endif

    return it_claves_.Siguiente();
}

template<class K, class S>
S& Dicc<K,S>::Iterador::SiguienteSignificado()
{
#ifdef DEBUG
    assert( HaySiguiente() );
#endif

    return it_significados_.Siguiente();
}

template<class K, class S>
typename Dicc<K,S>::Elem Dicc<K,S>::Iterador::Siguiente()
{
#ifdef DEBUG
    assert( HaySiguiente() );
#endif

    return Elem(SiguienteClave(), SiguienteSignificado());
}

template<class K, class S>
const K& Dicc<K,S>::Iterador::AnteriorClave() const
{
#ifdef DEBUG
    assert( HayAnterior() );
#endif

    return it_claves_.Anterior();
}

template<class K, class S>
S& Dicc<K,S>::Iterador::AnteriorSignificado()
{
#ifdef DEBUG
    assert(HayAnterior());
#endif

    return it_significados_.Anterior();
}

template<class K, class S>
typename Dicc<K,S>::Elem Dicc<K,S>::Iterador::Anterior()
{
#ifdef DEBUG
    assert(HayAnterior());
#endif

    return Elem(AnteriorClave(), AnteriorSignificado());
}

template<class K, class S>
void Dicc<K,S>::Iterador::Avanzar()
{
#ifdef DEBUG
    assert(HaySiguiente());
#endif

    it_claves_.Avanzar();
    it_significados_.Avanzar();
}

template<class K, class S>
void Dicc<K,S>::Iterador::Retroceder()
{
#ifdef DEBUG
    assert(HayAnterior());
#endif

    it_claves_.Retroceder();
    it_significados_.Retroceder();
}

template<class K, class S>
Dicc<K,S>::Iterador::Iterador(Dicc<K,S>* d)
        : it_claves_(d->claves_.CrearIt()), it_significados_(d->significados_.CrearIt())
{}

template<class K, class S>
void Dicc<K,S>::Iterador::EliminarSiguiente()
{
#ifdef DEBUG
    assert( HaySiguiente() );
#endif

    it_claves_.EliminarSiguiente();
    it_significados_.EliminarSiguiente();
}

template<class K, class S>
void Dicc<K,S>::Iterador::EliminarAnterior()
{
#ifdef DEBUG
    assert( HayAnterior() );
#endif

    it_claves_.EliminarAnterior();
    it_significados_.EliminarAnterior();
}

// Implementacion const_Iterador

template<class K, class S>
Dicc<K,S>::const_Iterador::const_Iterador()
{}

template<class K, class S>
Dicc<K,S>::const_Iterador::const_Iterador(const typename Dicc<K,S>::Iterador& otro)
        : it_claves_(otro.it_claves_), it_significados_(otro.it_significados_)
{}

template<class K, class S>
Dicc<K,S>::const_Iterador::const_Iterador(const typename Dicc<K, S>::const_Iterador& otro)
        : it_claves_(otro.it_claves_), it_significados_(otro.it_significados_)
{}

template<class K, class S>
typename Dicc<K,S>::const_Iterador& Dicc<K,S>::const_Iterador::operator=(const typename Dicc<K, S>::const_Iterador& otro)
{
    it_claves_ = otro.it_claves_;
    it_significados_ = otro.it_significados_;

    return *this;
}

template<class K, class S>
bool Dicc<K,S>::const_Iterador::HaySiguiente() const
{
    return it_claves_.HaySiguiente();
}

template<class K, class S>
bool Dicc<K,S>::const_Iterador::HayAnterior() const
{
    return it_claves_.HayAnterior();
}

template<class K, class S>
const K& Dicc<K,S>::const_Iterador::SiguienteClave() const
{
#ifdef DEBUG
    assert(HaySiguiente());
#endif

    return it_claves_.Siguiente();
}

template<class K, class S>
const S& Dicc<K,S>::const_Iterador::SiguienteSignificado() const
{
#ifdef DEBUG
    assert(HaySiguiente());
#endif

    return it_significados_.Siguiente();
}

template<class K, class S>
typename Dicc<K,S>::const_Elem Dicc<K,S>::const_Iterador::Siguiente() const
{
#ifdef DEBUG
    assert( HaySiguiente() );
#endif

    return const_Elem(SiguienteClave(), SiguienteSignificado());
}

template<class K, class S>
const K& Dicc<K,S>::const_Iterador::AnteriorClave() const
{
#ifdef DEBUG
    assert( HayAnterior() );
#endif

    return it_claves_.Anterior();
}

template<class K, class S>
const S& Dicc<K,S>::const_Iterador::AnteriorSignificado() const
{
#ifdef DEBUG
    assert(HayAnterior());
#endif

    return it_significados_.Anterior();
}

template<class K, class S>
typename Dicc<K,S>::const_Elem Dicc<K,S>::const_Iterador::Anterior() const
{
#ifdef DEBUG
    assert( HayAnterior() );
#endif

    return const_Elem(AnteriorClave(), AnteriorSignificado());
}

template<class K, class S>
void Dicc<K,S>::const_Iterador::Avanzar()
{
#ifdef DEBUG
    assert(HaySiguiente());
#endif

    it_claves_.Avanzar();
    it_significados_.Avanzar();
}

template<class K, class S>
void Dicc<K,S>::const_Iterador::Retroceder()
{
#ifdef DEBUG
    assert(HayAnterior());
#endif

    it_claves_.Retroceder();
    it_significados_.Retroceder();
}

template<class K, class S>
Dicc<K,S>::const_Iterador::const_Iterador(const Dicc<K,S>* d)
        : it_claves_(d->claves_.CrearIt()), it_significados_(d->significados_.CrearIt())
{}

template<class K, class S>
bool Dicc<K,S>::const_Iterador::operator == (const typename Dicc<K,S>::const_Iterador& otro) const
{
    return it_claves_ == otro.it_claves_ && it_significados_ == otro.it_significados_;
}

template<class K, class S>
bool Dicc<K,S>::Iterador::operator == (const typename Dicc<K,S>::Iterador& otro) const
{
    return it_claves_ == otro.it_claves_ && it_significados_ == otro.it_significados_;
}

///Funciones auxiliares

template<class K, class S>
typename Dicc<K,S>::Iterador Dicc<K,S>::Buscar(const K& clave)
{
    typename Dicc<K,S>::Iterador it = CrearIt();
    while(it.HaySiguiente() && !(it.SiguienteClave() == clave)){
        it.Avanzar();
    }
    return it;
}

template<class K, class S>
typename Dicc<K,S>::const_Iterador Dicc<K,S>::Buscar(const K& clave) const
{
    typename Dicc<K,S>::const_Iterador it = CrearIt();
    while(it.HaySiguiente() && !(it.SiguienteClave() == clave)){
        it.Avanzar();
    }
    return it;
}

template<class K, class S>
std::ostream& operator << (std::ostream& os, const Dicc<K,S>& d)
{
    return Mostrar(os, d, '{', '}');
}

template<class K, class S>
bool operator == (const Dicc<K,S>& d1, const Dicc<K,S>& d2)
{
    bool retval = d1.CantClaves() == d2.CantClaves();
    typename Dicc<K,S>::const_Iterador it1 = d1.CrearIt();
    while(retval and it1.HaySiguiente()){
        typename Dicc<K,S>::const_Iterador it2 = d2.Buscar(it1.SiguienteClave());
        retval = it2.HaySiguiente() and it1.SiguienteSignificado() == it2.SiguienteSignificado();
        it1.Avanzar();
    }
    return retval;