#include "StmExp.hpp"

StmExp::StmExp(Exp* expressao) : expressao(expressao) {}

StmExp::~StmExp() {
    delete expressao;
}

std::string StmExp::toString() const {
    return "EXP(" + expressao->toString() + ")";
}

StmExp* StmExp::criar(Exp* expressao) {
    return new StmExp(expressao);
}
