#include "StmSeq.hpp"

StmSeq::StmSeq(const std::vector<Stm *> &statements) : statements(statements) {}

StmSeq::~StmSeq()
{
    for (Stm *stm : statements)
    {
        delete stm;
    }
}

std::string StmSeq::toString() const
{
    return toString(0);
}

std::string StmSeq::toString(int indentLevel) const
{
    std::string indent(indentLevel * 2, ' ');
    std::string resultado = "SEQ(\n";

    for (size_t i = 0; i < statements.size(); ++i)
    {
        // Verificar se é uma StmSeq aninhada para aumentar indentação
        StmSeq *nestedSeq = dynamic_cast<StmSeq *>(statements[i]);
        if (nestedSeq)
        {
            resultado += indent + "  " + nestedSeq->toString(indentLevel + 1);
        }
        else
        {
            resultado += indent + "  " + statements[i]->toString();
        }

        if (i < statements.size() - 1)
        {
            resultado += ",\n";
        }
        else
        {
            resultado += "\n";
        }
    }
    resultado += indent + ")";
    return resultado;
}

void StmSeq::adicionar(Stm *stm)
{
    statements.push_back(stm);
}

StmSeq *StmSeq::criar(const std::vector<Stm *> &statements)
{
    return new StmSeq(statements);
}
