#include "ExpCall.hpp"

ExpCall::ExpCall(Exp* funcao, const std::vector<Exp*>& argumentos) 
    : funcao(funcao), argumentos(argumentos) {}

ExpCall::~ExpCall() {
    delete funcao;
    for (Exp* arg : argumentos) {
        delete arg;
    }
}

std::string ExpCall::toString() const {
    std::string resultado = "CALL(" + funcao->toString();
    for (Exp* arg : argumentos) {
        resultado += ", " + arg->toString();
    }
    resultado += ")";
    return resultado;
}

ExpCall* ExpCall::criar(Exp* funcao, const std::vector<Exp*>& argumentos) {
    return new ExpCall(funcao, argumentos);
}
