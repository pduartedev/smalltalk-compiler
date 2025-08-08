#include "StmSeq.hpp"

StmSeq::StmSeq(const std::vector<Stm*>& statements) : statements(statements) {}

StmSeq::~StmSeq() {
    for (Stm* stm : statements) {
        delete stm;
    }
}

std::string StmSeq::toString() const {
    std::string resultado = "SEQ(\n";
    for (size_t i = 0; i < statements.size(); ++i) {
        resultado += "  " + statements[i]->toString();
        if (i < statements.size() - 1) {
            resultado += ",\n";
        } else {
            resultado += "\n";
        }
    }
    resultado += ")";
    return resultado;
}

void StmSeq::adicionar(Stm* stm) {
    statements.push_back(stm);
}

StmSeq* StmSeq::criar(const std::vector<Stm*>& statements) {
    return new StmSeq(statements);
}
