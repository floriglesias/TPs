#include "Indice.h"
#include "utils.h"

using namespace std;

BaseDeDatos::Indice::Indice(const string &campo): campoIndexado(campo) {}

BaseDeDatos::Indice::Indice(const BaseDeDatos::Indice &ind) {
    (*this) = ind;
}

conjReg &BaseDeDatos::Indice::registrosAsociados(const Dato &dato) {
    //region O(L)*O(L)  //TODO: mejorar
    if(indice.count(dato.valor()) != 0) //O(L)
        //region O(L)
        return indice.at(dato.valor());
        //endregion
    else {
        //region O(1)
        return vacia();
        //endregion
    }
    //endregion
}

void BaseDeDatos::Indice::actualizarIndice(const Registro &registro){
//region O(L) * (O(L) + O(copyConjReg)) //TODO mejorar
    //region O(copyString) + O(dato()) + O(valor()) = O(copyString)
    string strDato = registro.dato(campoIndexado).valor();
    //endregion

    //region O(L)* O(copyString) + O(copyConjReg)
    if(indice.count(strDato)==0){
        //region O(std::pair<string, conjReg>) = O(copyString) + O(copyConjReg)
        conjReg l;
        l.insert(registro);
        indice.fast_insert(std::pair<string, conjReg>(strDato,l));
        //endregion
    }
    else {
        //region O(copyConjReg) + O(std::pair<string, conjReg>) = O(copyString) + O(copyConjReg)
        conjReg l = indice.at(strDato);    // así pude hacerlo andar...
        l.insert(registro);
        indice.insert(std::pair<string, conjReg>(strDato,l));
        //endregion
    }
    //endregion
//endregion
}

bool BaseDeDatos::Indice::operator==(const BaseDeDatos::Indice &otro) const {
    //region O(L) + O(cmpString_map<conjReg>)
    return campoIndexado == otro.campoIndexado and this->indice == otro.indice;
    //endregion
}

BaseDeDatos::Indice& BaseDeDatos::Indice::operator=(const BaseDeDatos::Indice &ind) {
    (*this).indice = ind.indice;
    (*this).campoIndexado = ind.campoIndexado;
}

conjReg& BaseDeDatos::Indice::vacia() {
    return listaVacia;
}




