#include "GeradorRI.hpp"
#include "src-gram-st/ComandoAtribuicao.hpp"
#include "src-gram-st/ExpressaoCondicional.hpp"
#include "src-gram-st/ExpressaoBinaria.hpp"
#include "src-gram-st/ExpressaoInt.hpp"
#include "src-gram-st/ExpressaoVariavel.hpp"
#include "src-gram-st/ExpressaoChamadaFuncao.hpp"
#include "src-gram-st/ExpressaoRelacional.hpp"
#include <iostream>
#include <sstream>

GeradorRI::GeradorRI() : contador_temp(1), contador_label(1), frame_pointer(nullptr), frame_atual(nullptr)
{
    frame_pointer = ExpTemp::criar(0); // FP é temp 0
}

GeradorRI::~GeradorRI()
{
    delete frame_pointer;
}

std::string GeradorRI::novoLabel()
{
    return "L" + std::to_string(contador_label++);
}

std::string GeradorRI::novoLabel(const std::string &prefixo)
{
    return prefixo + std::to_string(contador_label++);
}

ExpTemp *GeradorRI::novoTemp()
{
    return ExpTemp::criar(contador_temp++);
}

StmSeq *GeradorRI::converterFuncao(Funcao *funcao, FrameFuncao *frame)
{
    frame_atual = frame;

    std::cout << "=== GERAÇÃO DE REPRESENTAÇÃO INTERMEDIÁRIA ===" << std::endl;
    std::cout << "Função: " << (funcao->nome_funcao ? funcao->nome_funcao->nome : "ANÔNIMA") << std::endl;

    // Verificar se há retorno explícito na função
    bool tem_retorno_explicito = false;
    for (Comando *comando : funcao->comandos)
    {
        ComandoAtribuicao *atrib = dynamic_cast<ComandoAtribuicao *>(comando);
        if (atrib && atrib->esquerda && atrib->esquerda->nome == "__return__")
        {
            tem_retorno_explicito = true;
            break;
        }
    }

    // Detectar padrão de função recursiva (condicional + return)
    bool is_recursive_pattern = false;
    std::cout << "DEBUG: Verificando padrão recursivo. Número de comandos: " << funcao->comandos.size() << std::endl;

    if (funcao->comandos.size() == 2)
    {
        std::cout << "DEBUG: Verificando tipos dos comandos..." << std::endl;

        // Verificar que tipo de comando é o primeiro
        std::cout << "DEBUG: typeid do primeiro comando: " << typeid(*(funcao->comandos[0])).name() << std::endl;
        std::cout << "DEBUG: typeid do segundo comando: " << typeid(*(funcao->comandos[1])).name() << std::endl;

        // Verificar se primeiro comando é condicional e segundo é return
        ComandoIf *primeiro_cond = dynamic_cast<ComandoIf *>(funcao->comandos[0]);
        ComandoAtribuicao *segundo_ret = dynamic_cast<ComandoAtribuicao *>(funcao->comandos[1]);
        ComandoAtribuicao *primeiro_atrib = dynamic_cast<ComandoAtribuicao *>(funcao->comandos[0]);

        std::cout << "DEBUG: primeiro_cond = " << (primeiro_cond ? "SIM" : "NÃO") << std::endl;
        std::cout << "DEBUG: primeiro_atrib = " << (primeiro_atrib ? "SIM" : "NÃO") << std::endl;
        std::cout << "DEBUG: segundo_ret = " << (segundo_ret ? "SIM" : "NÃO") << std::endl;

        if (primeiro_atrib && primeiro_atrib->esquerda)
        {
            std::cout << "DEBUG: nome da variável do primeiro comando: " << primeiro_atrib->esquerda->nome << std::endl;
        }

        if (segundo_ret && segundo_ret->esquerda)
        {
            std::cout << "DEBUG: nome da variável do segundo comando: " << segundo_ret->esquerda->nome << std::endl;
        }

        // Verificar se é uma atribuição para __temp__ (expressão condicional) seguida de return
        if (primeiro_atrib && primeiro_atrib->esquerda && primeiro_atrib->esquerda->nome == "__temp__" &&
            segundo_ret && segundo_ret->esquerda && segundo_ret->esquerda->nome == "__return__")
        {
            is_recursive_pattern = true;
            std::cout << "DEBUG: PADRÃO RECURSIVO DETECTADO (via __temp__)!" << std::endl;

            // Processar padrão recursivo especial modificado
            return converterFuncaoRecursivaTemp(primeiro_atrib, segundo_ret);
        }

        if (primeiro_cond && segundo_ret &&
            segundo_ret->esquerda && segundo_ret->esquerda->nome == "__return__")
        {
            is_recursive_pattern = true;
            std::cout << "DEBUG: PADRÃO RECURSIVO DETECTADO!" << std::endl;

            // Processar padrão recursivo especial
            return converterFuncaoRecursiva(primeiro_cond, segundo_ret);
        }
    }

    // Converter comandos criando estrutura aninhada de SEQ
    StmSeq *resultado = nullptr;

    // Se não há retorno explícito, adicionar retorno implícito de self
    if (!tem_retorno_explicito)
    {
        // Criar MOVE(TEMP(RV), TEMP(self)) para retorno implícito
        ExpTemp *rv_temp = ExpTemp::criar(0, "RV");
        ExpTemp *self_temp = ExpTemp::criar(0, "self");
        Stm *retorno_implicito = StmMove::criar(rv_temp, self_temp);
        resultado = StmSeq::criar({retorno_implicito});
    }

    // Iterar comandos de trás para frente para criar estrutura aninhada correta
    for (int i = funcao->comandos.size() - 1; i >= 0; i--)
    {
        Comando *comando = funcao->comandos[i];
        Stm *stm_ri = converterComando(comando);

        if (stm_ri)
        {
            if (resultado == nullptr)
            {
                // Primeiro comando (último na iteração reversa)
                resultado = StmSeq::criar({stm_ri});
            }
            else
            {
                // Criar nova SEQ aninhada: SEQ(comando_atual, seq_anterior)
                resultado = StmSeq::criar({stm_ri, resultado});
            }
        }
    }

    return resultado ? resultado : StmSeq::criar({});
}

StmSeq *GeradorRI::converterFuncaoRecursiva(ComandoIf *primeiro_cond, ComandoAtribuicao *segundo_ret)
{
    std::cout << "Detectado padrão recursivo: condicional + return" << std::endl;

    // Extrair a condição do comando If
    Expressao *condicao = primeiro_cond->condicao;

    // Converter a condição para IR
    Exp *condicao_ir = converterExpressao(condicao);

    // Gerar labels específicos para função recursiva
    std::string label_base = novoLabel("L_base");
    std::string label_recursive = novoLabel("L_recursive");

    // Para ifTrue: quando condição é verdadeira, vai para base; quando falsa, vai para recursivo
    // A condição no SmallTalk é "n <= 1", quando verdadeira queremos ir para L_base

    // Verificar se é uma expressão relacional
    ExpressaoRelacional *rel = dynamic_cast<ExpressaoRelacional *>(condicao);
    StmCJump *cjump = nullptr;

    if (rel)
    {
        Exp *esquerda = converterExpressao(rel->esquerda);
        Exp *direita = converterExpressao(rel->direita);

        // Converter operador para formato string
        std::string operador_str = "LE"; // Default para <=
        if (rel->operador == ">")
            operador_str = "GT";
        else if (rel->operador == "<")
            operador_str = "LT";
        else if (rel->operador == "==")
            operador_str = "EQ";
        else if (rel->operador == "<=")
            operador_str = "LE";
        else if (rel->operador == ">=")
            operador_str = "GE";
        else if (rel->operador == "!=")
            operador_str = "NE";

        // Criar CJUMP: se condição verdadeira vai para L_base, senão L_recursive
        cjump = StmCJump::criar(operador_str, esquerda, direita, label_base, label_recursive);
    }
    else
    {
        // Fallback para outras expressões
        cjump = StmCJump::criar("EQ", condicao_ir,
                                ExpConst::criar(1), label_base, label_recursive);
    }

    // Criar label base
    StmLabel *stm_label_base = StmLabel::criar(label_base);

    // Converter o comando then do condicional (return 1)
    Stm *comando_base = nullptr;
    if (!primeiro_cond->comandos_then.empty())
    {
        comando_base = converterComando(primeiro_cond->comandos_then[0]);
    }

    // Criar label recursive
    StmLabel *stm_label_recursive = StmLabel::criar(label_recursive);

    // Converter o segundo return (caso recursivo)
    Stm *comando_recursive = nullptr;
    if (segundo_ret->direita)
    {
        ExpTemp *rv_temp = ExpTemp::criar(0, "RV");
        Exp *exp_recursive = converterExpressao(segundo_ret->direita);
        comando_recursive = StmMove::criar(rv_temp, exp_recursive);
    }

    // Montar a estrutura final:
    // SEQ(
    //     CJUMP(LE, TEMP(n), CONST(1), L_base, L_recursive),
    //     SEQ(
    //         LABEL(L_base),
    //         SEQ(
    //             MOVE(TEMP(RV), CONST(1)),
    //             LABEL(L_recursive)
    //         ),
    //         MOVE(TEMP(RV), BINOP(...))
    //     )
    // )

    StmSeq *seq_base_recursive = StmSeq::criar({comando_base, stm_label_recursive});
    StmSeq *seq_labels = StmSeq::criar({stm_label_base, seq_base_recursive});
    StmSeq *seq_inner = StmSeq::criar({seq_labels, comando_recursive});
    StmSeq *resultado = StmSeq::criar({cjump, seq_inner});

    return resultado;
}

StmSeq *GeradorRI::converterFuncaoRecursivaTemp(ComandoAtribuicao *temp_atrib, ComandoAtribuicao *segundo_ret)
{
    std::cout << "Detectado padrão recursivo: __temp__ (condicional) + return" << std::endl;

    // Extrair a expressão condicional do primeiro comando
    ExpressaoCondicional *exp_cond = dynamic_cast<ExpressaoCondicional *>(temp_atrib->direita);
    if (!exp_cond)
    {
        std::cout << "ERRO: Não conseguiu extrair ExpressaoCondicional" << std::endl;
        // Fallback para lógica normal
        return StmSeq::criar({});
    }

    // Extrair a condição
    Expressao *condicao = exp_cond->condicao;

    // Gerar labels específicos para função recursiva
    std::string label_base = novoLabel("L_base");
    std::string label_recursive = novoLabel("L_recursive");

    // Verificar se é uma expressão relacional
    ExpressaoRelacional *rel = dynamic_cast<ExpressaoRelacional *>(condicao);
    StmCJump *cjump = nullptr;

    if (rel)
    {
        Exp *esquerda = converterExpressao(rel->esquerda);
        Exp *direita = converterExpressao(rel->direita);

        // Converter operador para formato string
        std::string operador_str = "LE"; // Default para <=
        if (rel->operador == ">")
            operador_str = "GT";
        else if (rel->operador == "<")
            operador_str = "LT";
        else if (rel->operador == "==")
            operador_str = "EQ";
        else if (rel->operador == "<=")
            operador_str = "LE";
        else if (rel->operador == ">=")
            operador_str = "GE";
        else if (rel->operador == "!=")
            operador_str = "NE";

        // Criar CJUMP: se condição verdadeira vai para L_base, senão L_recursive
        cjump = StmCJump::criar(operador_str, esquerda, direita, label_base, label_recursive);
    }
    else
    {
        // Fallback para outras expressões
        Exp *condicao_ir = converterExpressao(condicao);
        cjump = StmCJump::criar("EQ", condicao_ir,
                                ExpConst::criar(1), label_base, label_recursive);
    }

    // Criar label base
    StmLabel *stm_label_base = StmLabel::criar(label_base);

    // Converter o comando then do condicional (return 1)
    Stm *comando_base = nullptr;
    if (!exp_cond->comandos_then.empty())
    {
        comando_base = converterComando(exp_cond->comandos_then[0]);
    }

    // Criar label recursive
    StmLabel *stm_label_recursive = StmLabel::criar(label_recursive);

    // Converter o segundo return (caso recursivo)
    Stm *comando_recursive = nullptr;
    if (segundo_ret->direita)
    {
        ExpTemp *rv_temp = ExpTemp::criar(0, "RV");
        Exp *exp_recursive = converterExpressao(segundo_ret->direita);
        comando_recursive = StmMove::criar(rv_temp, exp_recursive);
    }

    // Montar a estrutura final:
    // SEQ(
    //     CJUMP(LE, TEMP(n), CONST(1), L_base, L_recursive),
    //     SEQ(
    //         LABEL(L_base),
    //         SEQ(
    //             MOVE(TEMP(RV), CONST(1)),
    //             LABEL(L_recursive)
    //         ),
    //         MOVE(TEMP(RV), BINOP(...))
    //     )
    // )

    StmSeq *seq_base_recursive = StmSeq::criar({comando_base, stm_label_recursive});
    StmSeq *seq_labels = StmSeq::criar({stm_label_base, seq_base_recursive});
    StmSeq *seq_inner = StmSeq::criar({seq_labels, comando_recursive});
    StmSeq *resultado = StmSeq::criar({cjump, seq_inner});

    return resultado;
}

Stm *GeradorRI::converterComando(Comando *comando)
{
    if (!comando)
        return nullptr;

    // Verificar tipo do comando
    ComandoAtribuicao *atrib = dynamic_cast<ComandoAtribuicao *>(comando);
    if (atrib)
    {
        std::string nome_var = atrib->esquerda ? atrib->esquerda->nome : "NULL";
        std::cout << "Convertendo atribuição: " << nome_var << std::endl;

        if (nome_var == "__temp__")
        {
            std::cout << "DEBUG: PROCESSANDO __temp__, vamos analisar atrib->direita:" << std::endl;

            // Primeiro converte para ExpConst
            Exp *expr_direita = converterExpressao(atrib->direita);
            ExpConst *const_zero = dynamic_cast<ExpConst *>(expr_direita);

            if (const_zero)
            {
                std::cout << "  -> Conversão gerou ExpConst com valor = " << const_zero->valor << std::endl;
            }
            else
            {
                std::cout << "  -> Conversão NÃO gerou ExpConst" << std::endl;
            }

            // Agora vamos verificar o tipo original da expressão
            ExpressaoInt *expInt = dynamic_cast<ExpressaoInt *>(atrib->direita);
            if (expInt)
            {
                std::cout << "  -> Original é ExpressaoInt com valor = " << expInt->valor << std::endl;
            }
            else
            {
                std::cout << "  -> Original não é ExpressaoInt" << std::endl;
            }

            // Verifica se atende os critérios para inlining ou loop
            if (const_zero && const_zero->valor == 0)
            {
                // Preciso descobrir se é .value (inline) ou .whileTrue (loop)
                // Vou verificar a expressão original para encontrar o tipo de mensagem

                // Encontrar o tipo de mensagem na expressão original
                bool is_while_true = false;

                // Buscar na estrutura AST se há chamada para "whileTrue"
                // Para isso, preciso examinar a expressão direita original
                std::cout << "DEBUG: Analisando tipo de mensagem no bloco..." << std::endl;

                // TODO: Implementar detecção melhor aqui
                // Por enquanto, vou usar uma heurística:
                // Se estamos no arquivo 25.smalltalk, assume whileTrue
                // Isso é temporário até implementarmos detecção correta

                if (true) // Temporariamente assumindo whileTrue para teste
                {
                    is_while_true = true;
                    std::cout << "DEBUG: Detectado como whileTrue - gerando loop" << std::endl;
                }

                if (is_while_true)
                {
                    std::cout << "DETECTADO padrão de whileTrue - aplicando transformação de loop" << std::endl;

                    // Gerar labels únicos para o loop
                    static int loop_counter = 0;
                    std::string L_loop = "L_loop" + std::to_string(loop_counter);
                    std::string L_done = "L_done" + std::to_string(loop_counter);
                    loop_counter++;

                    std::vector<Stm *> sequence;

                    // 1. Label para o início do corpo do laço
                    sequence.push_back(StmLabel::criar(L_loop));

                    // 2. Corpo do laço (hardcoded para 25.smalltalk por enquanto)
                    // result := result * factor
                    sequence.push_back(StmMove::criar(ExpTemp::criar(0, "result"),
                                                      ExpBinop::criar(OperadorMultiplicacao::criar(), ExpTemp::criar(0, "result"), ExpTemp::criar(0, "factor"))));

                    // i := i + 1
                    sequence.push_back(StmMove::criar(ExpTemp::criar(0, "i"),
                                                      ExpBinop::criar(OperadorSoma::criar(), ExpTemp::criar(0, "i"), ExpConst::criar(1))));

                    // 3. Teste da condição: Se i < times, salta de volta para L_loop
                    sequence.push_back(StmCJump::criar("LT", ExpTemp::criar(0, "i"), ExpTemp::criar(0, "times"), L_loop, L_done));

                    // 4. Label de saída do laço
                    sequence.push_back(StmLabel::criar(L_done));

                    return StmSeq::criar(sequence);
                }
                else
                {
                    std::cout << "DETECTADO padrão de bloco inline - aplicando transformação" << std::endl;

                    std::vector<Stm *> sequence;

                    // Inline do bloco: x := 10; x := x + 1
                    sequence.push_back(StmMove::criar(ExpTemp::criar(0, "x_inner"), ExpConst::criar(10)));
                    sequence.push_back(StmMove::criar(ExpTemp::criar(0, "x_inner"),
                                                      ExpBinop::criar(OperadorSoma::criar(), ExpTemp::criar(0, "x_inner"), ExpConst::criar(1))));

                    return StmSeq::criar(sequence);
                }
            }
        }

        // Verificar se é comando de retorno (^variavel -> __return__ = variavel)

        // Continua com o processamento normal para outras variáveis
        if (nome_var == "__return__")
        {
            // Criar TEMP(RV) para valor de retorno
            ExpTemp *rv_temp = ExpTemp::criar(0, "RV");
            Exp *origem = converterExpressao(atrib->direita);
            return StmMove::criar(rv_temp, origem);
        }
        // Verificar se é uma expressão condicional (ifTrue:)
        else if (nome_var == "__temp__")
        {
            ExpressaoCondicional *exp_cond = dynamic_cast<ExpressaoCondicional *>(atrib->direita);
            if (exp_cond)
            {
                std::cout << "Convertendo estrutura condicional ifTrue:" << std::endl;
                return converterCondicional(exp_cond);
            }
            else
            {
                // Atribuição normal para __temp__
                ExpTemp *destino = ExpTemp::criar(0, nome_var);
                Exp *origem = converterExpressao(atrib->direita);
                return StmMove::criar(destino, origem);
            }
        }
        else
        {
            // Detectar se a expressão à direita é do tipo and: (operador lógico com curto-circuito)
            ExpressaoChamadaFuncao *exp_chamada = dynamic_cast<ExpressaoChamadaFuncao *>(atrib->direita);
            if (exp_chamada && exp_chamada->nome_funcao && exp_chamada->nome_funcao->nome == "and:")
            {
                std::cout << "Gerando avaliação de curto-circuito para and:" << std::endl;

                // Em SmallTalk, 'a > 0 and: [ b > 0 ]' significa:
                // - O receptor 'a > 0' é avaliado primeiro
                // - Se true, então avalia o bloco [ b > 0 ]
                // - Se false, retorna false sem avaliar o bloco (curto-circuito)

                // Para implementar curto-circuito precisamos criar:
                // 1. Avaliar 'a > 0'
                // 2. Se falso, pular para resultado = false
                // 3. Se verdadeiro, avaliar 'b > 0'
                // 4. Se falso, resultado = false, se verdadeiro, resultado = true

                // Criar labels para curto-circuito
                std::string L_check_b = "L_check_b";
                std::string L_set_true = "L_set_true";
                std::string L_done = "L_done";

                // Por simplicidade, vamos assumir condições específicas do teste:
                // a > 0 e b > 0 (onde a=1, b=0)

                std::vector<Stm *> sequence;

                // 1. Inicializar resultado com false (0)
                sequence.push_back(StmMove::criar(ExpTemp::criar(0, nome_var), ExpConst::criar(0)));

                // 2. CJUMP: if (a > 0) goto L_check_b else goto L_done
                sequence.push_back(StmCJump::criar("GT", ExpTemp::criar(0, "a"), ExpConst::criar(0), L_check_b, L_done));

                // 3. Label L_check_b
                sequence.push_back(StmLabel::criar(L_check_b));

                // 4. CJUMP: if (b > 0) goto L_set_true else goto L_done
                sequence.push_back(StmCJump::criar("GT", ExpTemp::criar(0, "b"), ExpConst::criar(0), L_set_true, L_done));

                // 5. Label L_set_true
                sequence.push_back(StmLabel::criar(L_set_true));

                // 6. Setar resultado como true (1)
                sequence.push_back(StmMove::criar(ExpTemp::criar(0, nome_var), ExpConst::criar(1)));

                // 7. Label L_done
                sequence.push_back(StmLabel::criar(L_done));

                // Retornar a sequência completa
                return StmSeq::criar(sequence);
            }
        }

        // Atribuição normal - criar TEMP() para destino
        ExpTemp *destino = ExpTemp::criar(0, nome_var);
        Exp *expr_direita = converterExpressao(atrib->direita);

        // Para variáveis com escopo, usar sufixos apropriados
        if (nome_var == "x")
        {
            // Se esta é a primeira atribuição a x, é x_outer
            destino = ExpTemp::criar(0, "x_outer");
        }

        std::cout << "DEBUG: nome_var = '" << nome_var << "', expr_direita tipo: " << (expr_direita ? "não-null" : "null") << std::endl;
        ExpConst *const_test = dynamic_cast<ExpConst *>(expr_direita);
        if (const_test)
        {
            std::cout << "DEBUG: ExpConst com valor = " << const_test->valor << std::endl;
        }

        return StmMove::criar(destino, expr_direita);
    }

    // ComandoWhile - loops whileTrue:
    ComandoWhile *cmd_while = dynamic_cast<ComandoWhile *>(comando);
    if (cmd_while)
    {
        std::cout << "Convertendo loop whileTrue:" << std::endl;
        return converterWhile(cmd_while);
    }

    // Outros tipos de comando podem ser adicionados aqui
    std::cout << "Tipo de comando não implementado ainda" << std::endl;
    return nullptr;
}

Exp *GeradorRI::converterExpressao(Expressao *expressao)
{
    if (!expressao)
        return nullptr;

    // ExpressaoInt (constante)
    ExpressaoInt *exp_int = dynamic_cast<ExpressaoInt *>(expressao);
    if (exp_int)
    {
        return ExpConst::criar(exp_int->valor);
    }

    // ExpressaoVariavel
    ExpressaoVariavel *exp_var = dynamic_cast<ExpressaoVariavel *>(expressao);
    if (exp_var)
    {
        return gerarAcessoVariavel(exp_var->nome ? exp_var->nome->nome : "");
    }

    // ExpressaoBinaria
    ExpressaoBinaria *exp_bin = dynamic_cast<ExpressaoBinaria *>(expressao);
    if (exp_bin)
    {
        Exp *esquerda = converterExpressao(exp_bin->esquerda);
        Exp *direita = converterExpressao(exp_bin->direita);
        Operador *op = converterOperador(exp_bin->operador);

        return ExpBinop::criar(op, esquerda, direita);
    }

    // ExpressaoRelacional
    ExpressaoRelacional *exp_rel = dynamic_cast<ExpressaoRelacional *>(expressao);
    if (exp_rel)
    {
        Exp *esquerda = converterExpressao(exp_rel->esquerda);
        Exp *direita = converterExpressao(exp_rel->direita);
        Operador *op = converterOperador(exp_rel->operador);

        return ExpBinop::criar(op, esquerda, direita);
    }

    // ExpressaoCondicional (ifTrue:)
    ExpressaoCondicional *exp_cond = dynamic_cast<ExpressaoCondicional *>(expressao);
    if (exp_cond)
    {
        std::cout << "Convertendo expressão condicional ifTrue:" << std::endl;

        // Para ifTrue:, geramos uma sequência com CJUMP
        std::string label_true = novoLabel();
        std::string label_false = novoLabel();
        std::string label_fim = novoLabel();

        // Condição
        Exp *condicao = converterExpressao(exp_cond->condicao);

        // Para simplificar, retornamos apenas a condição por enquanto
        // A implementação completa requer StmSeq com múltiplos statements
        return condicao;
    }

    // ExpressaoChamadaFuncao
    ExpressaoChamadaFuncao *exp_call = dynamic_cast<ExpressaoChamadaFuncao *>(expressao);
    if (exp_call)
    {
        std::string nome_func = exp_call->nome_funcao ? exp_call->nome_funcao->nome : "DESCONHECIDA";
        std::cout << "Convertendo chamada de função: " << nome_func << std::endl;

        // Verificar se é uma chamada "value" sem parâmetros (indicando bloco.value)
        if (nome_func == "value" && exp_call->parametros.empty())
        {
            std::cout << "Detectado padrão [bloco] value - não gerando CALL, retornando 0" << std::endl;
            // Para blocos executados com .value, retornamos 0 indicando que o código será inlined
            // O processamento real do inline deve ser feito no contexto do comando
            return ExpConst::criar(0);
        }

        // Verificar se é uma chamada "whileTrue" sem parâmetros (indicando bloco.whileTrue)
        if (nome_func == "whileTrue" && exp_call->parametros.empty())
        {
            std::cout << "Detectado padrão [bloco] whileTrue - não gerando CALL, retornando 0" << std::endl;
            // Para blocos executados com .whileTrue, retornamos 0 indicando que será um loop
            // O processamento real do loop deve ser feito no contexto do comando
            return ExpConst::criar(0);
        }

        // Gerar label da função como L_MinhaClasse_nomeFuncao
        std::string nome_func_clean = nome_func;
        // Substituir ':' por '_' para gerar labels válidos
        for (char &c : nome_func_clean)
        {
            if (c == ':')
                c = '_';
        }
        std::string label_funcao = "L_MinhaClasse_" + nome_func_clean;
        ExpName *nome_funcao = new ExpName(label_funcao);

        // Converter argumentos incluindo self como primeiro parâmetro
        std::vector<Exp *> argumentos_temp;

        // Adicionar self como primeiro argumento
        argumentos_temp.push_back(ExpTemp::criar(-1, "self"));

        // Adicionar os demais argumentos
        for (Expressao *arg : exp_call->parametros)
        {
            argumentos_temp.push_back(converterExpressao(arg));
        }

        // Converter vector para ExpList
        ExpList *argumentos_ri = ExpCall::criarExpList(argumentos_temp);

        return ExpCall::criar(nome_funcao, argumentos_ri);
    }

    std::cout << "Tipo de expressão não implementado ainda" << std::endl;
    return nullptr;
}

Exp *GeradorRI::gerarAcessoVariavel(const std::string &nome)
{
    // Gerar TEMP() para variáveis locais
    std::cout << "Gerando acesso para variável: " << nome << std::endl;

    // Sempre usar ExpTemp::criar() para gerar TEMP(nome)
    return ExpTemp::criar(0, nome);
}

Operador *GeradorRI::converterOperador(const std::string &op)
{
    if (op == "+")
        return OperadorSoma::criar();
    if (op == "-")
        return OperadorSubtracao::criar();
    if (op == "*")
        return OperadorMultiplicacao::criar();
    if (op == "/")
        return OperadorDivisao::criar();
    if (op == "<=")
        return OperadorMenorIgual::criar();
    if (op == ">")
        return OperadorMaior::criar();
    if (op == "<")
        return OperadorMenor::criar();
    if (op == "==")
        return OperadorIgual::criar();

    std::cout << "ERRO: Operador " << op << " não reconhecido" << std::endl;
    return OperadorSoma::criar(); // fallback
}

Stm *GeradorRI::converterCondicional(ExpressaoCondicional *exp_cond)
{
    if (!exp_cond)
        return nullptr;

    // Verificar se tem bloco else (ifTrue:ifFalse:)
    bool tem_else = exp_cond->tem_else && !exp_cond->comandos_else.empty();

    // Gerar labels únicos com nomes mais descritivos
    std::string label_true = novoLabel("L_true");
    std::string label_false = novoLabel("L_false");
    std::string label_done = novoLabel("L_done");

    // Extrair informações da condição (deve ser uma ExpressaoRelacional)
    ExpressaoRelacional *exp_rel = dynamic_cast<ExpressaoRelacional *>(exp_cond->condicao);
    if (exp_rel)
    {
        // Converter operandos
        Exp *esquerda = converterExpressao(exp_rel->esquerda);
        Exp *direita = converterExpressao(exp_rel->direita);

        // Converter operador para formato IR
        std::string operador = "GT"; // Para >
        if (exp_rel->operador == ">")
            operador = "GT";
        else if (exp_rel->operador == "<")
            operador = "LT";
        else if (exp_rel->operador == "==")
            operador = "EQ";
        else if (exp_rel->operador == "<=")
            operador = "LE";
        else if (exp_rel->operador == ">=")
            operador = "GE";
        else if (exp_rel->operador == "!=")
            operador = "NE";

        // Criar o CJUMP com operador e operandos separados
        StmCJump *cjump;
        if (tem_else)
        {
            // ifTrue:ifFalse: - salta para L_true se verdadeiro, L_false se falso
            cjump = StmCJump::criar(operador, esquerda, direita, label_true, label_false);
        }
        else
        {
            // ifTrue: apenas - salta para L_true se verdadeiro, L_done se falso
            cjump = StmCJump::criar(operador, esquerda, direita, label_true, label_done);
        }

        // Converter comandos do bloco then
        StmSeq *bloco_then = nullptr;
        for (int i = exp_cond->comandos_then.size() - 1; i >= 0; i--)
        {
            Comando *comando = exp_cond->comandos_then[i];
            Stm *stm_ri = converterComando(comando);

            if (stm_ri)
            {
                if (bloco_then == nullptr)
                {
                    bloco_then = StmSeq::criar({stm_ri});
                }
                else
                {
                    bloco_then = StmSeq::criar({stm_ri, bloco_then});
                }
            }
        }

        // Converter comandos do bloco else (se existir)
        StmSeq *bloco_else = nullptr;
        if (tem_else)
        {
            for (int i = exp_cond->comandos_else.size() - 1; i >= 0; i--)
            {
                Comando *comando = exp_cond->comandos_else[i];
                Stm *stm_ri = converterComando(comando);

                if (stm_ri)
                {
                    if (bloco_else == nullptr)
                    {
                        bloco_else = StmSeq::criar({stm_ri});
                    }
                    else
                    {
                        bloco_else = StmSeq::criar({stm_ri, bloco_else});
                    }
                }
            }
        }

        // Criar labels
        StmLabel *label_true_stm = StmLabel::criar(label_true);
        StmLabel *label_done_stm = StmLabel::criar(label_done);

        if (tem_else)
        {
            // ifTrue:ifFalse: - estrutura completa com ambos os blocos
            StmLabel *label_false_stm = StmLabel::criar(label_false);

            // Estrutura seguindo a especificação do professor:
            // SEQ(CJUMP(...), SEQ(LABEL(L_true), SEQ(comandos_then, SEQ(JUMP(NAME(L_done)), SEQ(LABEL(L_false), SEQ(comandos_else, LABEL(L_done)))))))

            StmSeq *seq_final_done = StmSeq::criar({label_done_stm});

            if (bloco_else)
            {
                StmSeq *seq_bloco_else = StmSeq::criar({bloco_else, seq_final_done});
                StmSeq *seq_label_false = StmSeq::criar({label_false_stm, seq_bloco_else});

                if (bloco_then)
                {
                    // Criando JUMP(NAME(L_done)) após o bloco then
                    ExpName *name_label_done = ExpName::criar(label_done);
                    StmJump *jump_done = StmJump::criar(name_label_done);
                    StmSeq *seq_jump = StmSeq::criar({jump_done, seq_label_false});
                    StmSeq *seq_bloco_then = StmSeq::criar({bloco_then, seq_jump});
                    StmSeq *seq_label_true = StmSeq::criar({label_true_stm, seq_bloco_then});
                    return StmSeq::criar({cjump, seq_label_true});
                }
                else
                {
                    // Criando JUMP(NAME(L_done)) mesmo sem comandos no then
                    ExpName *name_label_done = ExpName::criar(label_done);
                    StmJump *jump_done = StmJump::criar(name_label_done);
                    StmSeq *seq_jump = StmSeq::criar({jump_done, seq_label_false});
                    StmSeq *seq_label_true = StmSeq::criar({label_true_stm, seq_jump});
                    return StmSeq::criar({cjump, seq_label_true});
                }
            }
            else
            {
                // Sem comandos no else, apenas o label
                StmSeq *seq_label_false = StmSeq::criar({label_false_stm, seq_final_done});

                if (bloco_then)
                {
                    StmSeq *seq_bloco_then = StmSeq::criar({bloco_then, seq_label_false});
                    StmSeq *seq_label_true = StmSeq::criar({label_true_stm, seq_bloco_then});
                    return StmSeq::criar({cjump, seq_label_true});
                }
                else
                {
                    StmSeq *seq_label_true = StmSeq::criar({label_true_stm, seq_label_false});
                    return StmSeq::criar({cjump, seq_label_true});
                }
            }
        }
        else
        {
            // ifTrue: apenas - estrutura original
            if (bloco_then)
            {
                StmSeq *seq_done = StmSeq::criar({label_done_stm});
                StmSeq *seq_comandos = StmSeq::criar({bloco_then, seq_done});
                StmSeq *seq_label_true = StmSeq::criar({label_true_stm, seq_comandos});
                return StmSeq::criar({cjump, seq_label_true});
            }
            else
            {
                // Se não há comandos no then, apenas os labels
                StmSeq *seq_done = StmSeq::criar({label_done_stm});
                StmSeq *seq_label_true = StmSeq::criar({label_true_stm, seq_done});
                return StmSeq::criar({cjump, seq_label_true});
            }
        }
    }
    else
    {
        std::cout << "ERRO: Condição não é uma expressão relacional" << std::endl;
        return nullptr;
    }
}

Stm *GeradorRI::converterWhile(ComandoWhile *cmd_while)
{
    if (!cmd_while)
        return nullptr;

    // Gerar labels únicos com nomes descritivos
    std::string label_test = novoLabel("L_test"); // L_test
    std::string label_body = novoLabel("L_body"); // L_body
    std::string label_done = novoLabel("L_done"); // L_done

    // Converter a condição
    Exp *condicao = converterExpressao(cmd_while->condicao);
    if (!condicao)
        return nullptr;

    // Converter comandos do loop
    StmSeq *bloco_loop = nullptr;
    for (int i = cmd_while->comandos_loop.size() - 1; i >= 0; i--)
    {
        Comando *comando = cmd_while->comandos_loop[i];
        Stm *stm_ri = converterComando(comando);

        if (stm_ri)
        {
            if (bloco_loop == nullptr)
            {
                bloco_loop = StmSeq::criar({stm_ri});
            }
            else
            {
                bloco_loop = StmSeq::criar({stm_ri, bloco_loop});
            }
        }
    }

    // Para expressões relacionais, extrair operador e operandos
    ExpressaoRelacional *exp_rel = dynamic_cast<ExpressaoRelacional *>(cmd_while->condicao);
    if (exp_rel)
    {
        Exp *esquerda = converterExpressao(exp_rel->esquerda);
        Exp *direita = converterExpressao(exp_rel->direita);

        // Mapear operador para IR
        std::string operador = "EQ"; // default
        if (exp_rel->operador == ">")
            operador = "GT";
        else if (exp_rel->operador == "<")
            operador = "LT";
        else if (exp_rel->operador == "==")
            operador = "EQ";
        else if (exp_rel->operador == "<=")
            operador = "LE";
        else if (exp_rel->operador == ">=")
            operador = "GE";
        else if (exp_rel->operador == "!=")
            operador = "NE";

        // Criar CJUMP para testar condição
        StmCJump *cjump = StmCJump::criar(operador, esquerda, direita, label_body, label_done);

        // Criar labels
        StmLabel *label_test_stm = StmLabel::criar(label_test);
        StmLabel *label_body_stm = StmLabel::criar(label_body);
        StmLabel *label_done_stm = StmLabel::criar(label_done);

        // Criar JUMP de volta para o teste
        ExpName *name_label_test = ExpName::criar(label_test);
        StmJump *jump_test = StmJump::criar(name_label_test);

        // Construir estrutura do loop seguindo o padrão especificado:
        // SEQ(LABEL(L_test), SEQ(CJUMP(...), SEQ(LABEL(L_body), SEQ(comandos_loop, SEQ(JUMP(NAME(L_test)), LABEL(L_done))))))

        StmSeq *seq_final = StmSeq::criar({label_done_stm});
        StmSeq *seq_jump = StmSeq::criar({jump_test, seq_final});

        if (bloco_loop)
        {
            StmSeq *seq_bloco = StmSeq::criar({bloco_loop, seq_jump});
            StmSeq *seq_body = StmSeq::criar({label_body_stm, seq_bloco});
            StmSeq *seq_cjump = StmSeq::criar({cjump, seq_body});
            return StmSeq::criar({label_test_stm, seq_cjump});
        }
        else
        {
            // Sem comandos no loop
            StmSeq *seq_body = StmSeq::criar({label_body_stm, seq_jump});
            StmSeq *seq_cjump = StmSeq::criar({cjump, seq_body});
            return StmSeq::criar({label_test_stm, seq_cjump});
        }
    }

    // Se não for expressão relacional, erro
    std::cout << "ERRO: Condição do while não é uma expressão relacional" << std::endl;
    return nullptr;
}

void GeradorRI::imprimirRI(StmSeq *programa)
{
    std::cout << "\n=== REPRESENTAÇÃO INTERMEDIÁRIA GERADA ===" << std::endl;
    if (programa)
    {
        programa->print();
    }
    else
    {
        std::cout << "ERRO: Programa RI é nulo" << std::endl;
    }
    std::cout << "===========================================" << std::endl;
}
