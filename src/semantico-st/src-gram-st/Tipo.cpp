#include "Tipo.hpp"

bool Tipo::compativel_com(Tipo* outro) {
    if (this->tipo == outro->tipo) return true;
    
    // Em Smalltalk, Object é compatível com qualquer tipo (polimorfismo)
    if (this->tipo == TipoSmallTalk::OBJECT || outro->tipo == TipoSmallTalk::OBJECT) {
        return true;
    }
    
    // Integer é compatível com Float para operações aritméticas
    if ((this->tipo == TipoSmallTalk::INTEGER && outro->tipo == TipoSmallTalk::FLOAT) ||
        (this->tipo == TipoSmallTalk::FLOAT && outro->tipo == TipoSmallTalk::INTEGER)) {
        return true;
    }
    
    return false;
}

bool Tipo::pode_fazer_operacao_aritmetica() {
    return (tipo == TipoSmallTalk::INTEGER || tipo == TipoSmallTalk::FLOAT || tipo == TipoSmallTalk::OBJECT);
}

bool Tipo::pode_fazer_operacao_logica() {
    return (tipo == TipoSmallTalk::BOOLEAN || tipo == TipoSmallTalk::OBJECT);
}

string Tipo::to_string() {
    return nome;
}
