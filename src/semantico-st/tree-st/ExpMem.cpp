#include "ExpMem.hpp"

ExpMem::ExpMem(Exp* endereco) : endereco(endereco) {}

ExpMem::~ExpMem() {
    delete endereco;
}

std::string ExpMem::toString() const {
    return "MEM(" + endereco->toString() + ")";
}

ExpMem* ExpMem::criar(Exp* endereco) {
    return new ExpMem(endereco);
}
