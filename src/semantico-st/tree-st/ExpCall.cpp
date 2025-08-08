#include "ExpCall.hpp"
#include "ExpList.hpp"

ExpCall::ExpCall(Exp *funcao, ExpList *argumentos)
    : funcao(funcao), argumentos(argumentos) {}

ExpCall::~ExpCall()
{
    delete funcao;
    delete argumentos;
}

std::string ExpCall::toString() const
{
    std::string resultado = "CALL(\n            " + funcao->toString();
    if (argumentos != nullptr)
    {
        resultado += ",\n            " + argumentos->toString();
    }
    resultado += "\n        )";
    return resultado;
}

ExpCall *ExpCall::criar(Exp *funcao, ExpList *argumentos)
{
    return new ExpCall(funcao, argumentos);
}

ExpList *ExpCall::criarExpList(const std::vector<Exp *> &args)
{
    if (args.empty())
    {
        return nullptr;
    }

    ExpList *result = nullptr;
    for (int i = args.size() - 1; i >= 0; i--)
    {
        result = new ExpList(args[i], result);
    }
    return result;
}
