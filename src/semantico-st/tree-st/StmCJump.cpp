#include "StmCJump.hpp"

StmCJump::StmCJump(Exp* expressao_booleana, const std::string& label_verdadeiro, const std::string& label_falso)
    : expressao_booleana(expressao_booleana), label_verdadeiro(label_verdadeiro), label_falso(label_falso) {}

StmCJump::~StmCJump() {
    delete expressao_booleana;
}

std::string StmCJump::toString() const {
    return "CJUMP(" + expressao_booleana->toString() + ", " + 
           label_verdadeiro + ", " + label_falso + ")";
}

StmCJump* StmCJump::criar(Exp* expressao_booleana, const std::string& label_verdadeiro, const std::string& label_falso) {
    return new StmCJump(expressao_booleana, label_verdadeiro, label_falso);
}
