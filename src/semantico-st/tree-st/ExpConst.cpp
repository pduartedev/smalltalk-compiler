#include "ExpConst.hpp"

ExpConst::ExpConst(int valor) : valor(valor) {}

std::string ExpConst::toString() const {
    return std::to_string(valor);
}

ExpConst* ExpConst::criar(int valor) {
    return new ExpConst(valor);
}
