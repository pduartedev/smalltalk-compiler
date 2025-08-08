#include "Funcao.hpp"
#include "ComandoAtribuicao.hpp"
#include "Expressao.hpp"
#include "../debug-util.hpp"
#include <iostream>

Funcao::Funcao() {
}

Funcao* Funcao::extrai_funcao(No_arv_parse *no) {
  // Adaptado para a gramática completa do Smalltalk
  // Primeiro tenta encontrar Method_Definition na árvore
  Funcao* metodo = extrai_funcao_recursivo(no);
  if (metodo != nullptr) {
    // Incluir variáveis de instância da classe no método
    vector<Variavel*> variaveis_instancia = extrai_variaveis_instancia(no);
    
    // Adicionar variáveis de instância antes das variáveis locais/temporárias
    vector<Variavel*> todas_variaveis = variaveis_instancia;
    todas_variaveis.insert(todas_variaveis.end(), metodo->variaveis.begin(), metodo->variaveis.end());
    metodo->variaveis = todas_variaveis;
    
    return metodo;
  }
  
  // Debug: verificar qual é o símbolo do nó raiz
  if (no != nullptr) {
    cerr << "DEBUG: Nó raiz da árvore de parse: " << no->simb << endl;
  }
  
  // Se não encontrou método, verifica se há statements no nível superior
  // para criar uma "função implícita" (script principal)
  if (no != nullptr) {
    if (no->simb == "Item_List") {
      Funcao* funcao_implicita = criar_funcao_implicita(no);
      if (funcao_implicita != nullptr) {
        return funcao_implicita;
      }
    }
    // Verificar se o nó raiz é diretamente um Statement
    else if (no->simb == "Statement") {
      Funcao* funcao_implicita = criar_funcao_statement_simples(no);
      if (funcao_implicita != nullptr) {
        return funcao_implicita;
      }
    }
    // Verificar se o nó raiz é TOKEN_period - extrair statement do primeiro filho
    else if (no->simb == "TOKEN_period" && no->filhos.size() > 0) {
      // O primeiro filho deve ser o statement
      if (no->filhos[0]->simb == "Statement") {
        cerr << "DEBUG: Encontrado Statement como filho do TOKEN_period" << endl;
        Funcao* funcao_implicita = criar_funcao_statement_simples(no->filhos[0]);
        if (funcao_implicita != nullptr) {
          return funcao_implicita;
        }
      }
    }
    // Caso especial: buscar recursivamente por qualquer Statement na árvore
    else {
      cerr << "DEBUG: Buscando Statement recursivamente na árvore" << endl;
      // Funcao* funcao_statement = buscar_e_extrair_statement_recursivo(no);
      // if (funcao_statement != nullptr) {
      //   return funcao_statement;
      // }
    }
  }
  
  return nullptr;
}

Funcao* Funcao::extrai_funcao_recursivo(No_arv_parse *no) {
  if (no == nullptr) return nullptr;
  
  // Usando gramática completa - Se encontrou um Method_Definition
  // Method_Definition -> Message_Pattern TOKEN_left_bracket Temporaries_Opt Statement_Opt TOKEN_right_bracket
  if (no->simb == "Method_Definition") {
    Funcao* res = new Funcao();
    
    if (no->filhos.size() >= 4) {
      // Extrair padrão da mensagem (nome e parâmetros) - estrutura completa da gramática
      extrai_message_pattern(no->filhos[0], res);
      
      // Ignorar TOKEN_left_bracket (no->filhos[1])
      
      // Extrair variáveis temporárias se existirem
      // Temporaries_Opt -> Temporaries | ε
      if (no->filhos[2]->simb == "Temporaries_Opt") {
        res->variaveis = extrai_temporaries(no->filhos[2]);
      }
      
      // Extrair comandos se existirem  
      // Statement_Opt -> Statements | ε
      if (no->filhos[3]->simb == "Statement_Opt") {
        res->comandos = extrai_statements(no->filhos[3]);
      }
      
      // Ignorar TOKEN_right_bracket (no->filhos[4])
    }
    
    return res;
  }
  
  // Buscar recursivamente nos filhos - percorre toda a árvore da gramática completa
  for (auto filho : no->filhos) {
    Funcao* resultado = extrai_funcao_recursivo(filho);
    if (resultado != nullptr) {
      return resultado;
    }
  }
  
  return nullptr;
}

vector<Funcao*> Funcao::extrai_todas_funcoes(No_arv_parse *no) {
  vector<Funcao*> funcoes;
  extrai_todas_funcoes_recursivo(no, funcoes);
  
  // Incluir variáveis de instância da classe em cada método
  vector<Variavel*> variaveis_instancia = extrai_variaveis_instancia(no);
  
  for (Funcao* funcao : funcoes) {
    // Adicionar variáveis de instância antes das variáveis locais/temporárias
    vector<Variavel*> todas_variaveis = variaveis_instancia;
    todas_variaveis.insert(todas_variaveis.end(), funcao->variaveis.begin(), funcao->variaveis.end());
    funcao->variaveis = todas_variaveis;
  }
  
  return funcoes;
}

void Funcao::extrai_todas_funcoes_recursivo(No_arv_parse *no, vector<Funcao*>& funcoes) {
  if (no == nullptr) return;
  
  // Se encontrou um Method_Definition, extrair a função
  if (no->simb == "Method_Definition") {
    Funcao* res = new Funcao();
    
    if (no->filhos.size() >= 4) {
      // Extrair padrão da mensagem (nome e parâmetros)
      extrai_message_pattern(no->filhos[0], res);
      
      // Extrair variáveis temporárias se existirem
      if (no->filhos[2]->simb == "Temporaries_Opt") {
        res->variaveis = extrai_temporaries(no->filhos[2]);
      }
      
      // Extrair comandos se existirem  
      if (no->filhos[3]->simb == "Statement_Opt") {
        res->comandos = extrai_statements(no->filhos[3]);
      }
    }
    
    funcoes.push_back(res);
  }
  
  // Continuar buscando recursivamente nos filhos
  for (auto filho : no->filhos) {
    extrai_todas_funcoes_recursivo(filho, funcoes);
  }
}

void debug_variaveis(const vector<Variavel*> &vars, int tab) {
  for (int iv = 0; iv < vars.size(); ++iv) {
    tab3(tab);
    vars[iv]->debug_com_tab(tab+1);
  }
}

void debug_comandos(const vector<Comando*> &coms, int tab) {
  for (int ic = 0; ic < coms.size(); ++ic) {
    coms[ic]->debug_com_tab(tab+1);
  }  
}

void Funcao::debug() {
  if (tipo_retorno == NULL) cerr<< "TR NULL"<< endl;
  if (nome_funcao == NULL) cerr<< "NF NULL"<< endl;
  cerr << "Funcao:[retorno=" << tipo_retorno->nome << "][nome=" << nome_funcao->nome << "]" << endl;
  cerr << "      (Param:(";
  fflush(stderr);
  for (int i_par = 0; i_par < parametros.size(); ++i_par) {
    cerr << (parametros[i_par])->tipo->nome << " " <<
      (parametros[i_par])->nome->nome << ", ";
  }
  cerr << ") { " << endl;
  debug_variaveis(variaveis, 1);
  debug_comandos(comandos, 1);
  cerr << "}" <<  endl;
}

void Funcao::extrai_message_pattern(No_arv_parse *no, Funcao* func) {
  if (no == nullptr || func == nullptr) return;
  
  // Message_Pattern da gramática completa:
  // Message_Pattern -> Unary_Pattern | Binary_Pattern | Keyword_Pattern
  
  if (no->simb == "Message_Pattern") {
    // Processar o primeiro filho que contém o padrão real
    if (no->filhos.size() > 0) {
      extrai_message_pattern(no->filhos[0], func);
    }
    return;
  }
  
  if (no->simb == "Unary_Pattern") {
    // Unary_Pattern -> TOKEN_identifier (gramática completa)
    if (no->filhos.size() > 0) {
      func->nome_funcao = ID::extrai_ID(no->filhos[0]);
      // Tipo de retorno padrão para análise semântica
      func->tipo_retorno = new ID();
      func->tipo_retorno->nome = "Object";
    }
  } 
  else if (no->simb == "Binary_Pattern") {
    // Binary_Pattern -> TOKEN_binary_selector TOKEN_identifier (gramática completa)
    if (no->filhos.size() >= 2) {
      func->nome_funcao = ID::extrai_ID(no->filhos[0]);
      
      // Criar parâmetro para análise semântica
      Variavel* param = new Variavel();
      param->nome = ID::extrai_ID(no->filhos[1]);
      param->tipo = new ID();
      param->tipo->nome = "Object"; // Tipo padrão
      func->parametros.push_back(param);
      
      // Tipo de retorno padrão
      func->tipo_retorno = new ID();
      func->tipo_retorno->nome = "Object";
    }
  }
  else if (no->simb == "Keyword_Pattern") {
    // Keyword_Pattern -> Keyword_Pattern_Element Keyword_Pattern_Element_List (gramática completa)
    // Extrair nome da função e parâmetros dos keywords
    string nome_completo = "";
    Funcao::extrai_keyword_pattern_elementos(no, func, nome_completo);
    
    if (!nome_completo.empty()) {
      func->nome_funcao = new ID();
      func->nome_funcao->nome = nome_completo;
    }
    
    // Tipo de retorno padrão
    func->tipo_retorno = new ID();
    func->tipo_retorno->nome = "Object";
  }
}

void Funcao::extrai_keyword_pattern_elementos(No_arv_parse *no, Funcao* func, string& nome_completo) {
  if (no == nullptr || func == nullptr) return;
  
  // Keyword_Pattern -> Keyword_Pattern_Element Keyword_Pattern_Element_List (gramática completa)
  if (no->simb == "Keyword_Pattern") {
    if (no->filhos.size() >= 1) {
      Funcao::extrai_keyword_pattern_elementos(no->filhos[0], func, nome_completo); // Primeiro elemento
      if (no->filhos.size() >= 2) {
        Funcao::extrai_keyword_pattern_elementos(no->filhos[1], func, nome_completo); // Lista de elementos
      }
    }
  }
  else if (no->simb == "Keyword_Pattern_Element") {
    // Keyword_Pattern_Element -> TOKEN_keyword TOKEN_identifier (gramática completa)
    if (no->filhos.size() >= 2) {
      // Adicionar keyword ao nome da função
      nome_completo += no->filhos[0]->dado_extra;
      
      // Criar parâmetro para análise semântica
      Variavel* param = new Variavel();
      param->nome = ID::extrai_ID(no->filhos[1]);
      param->tipo = new ID();
      param->tipo->nome = "Object"; // Tipo padrão
      param->tipo_semantico = param->inferir_tipo_semantico();
      func->parametros.push_back(param);
    }
  }
  else if (no->simb == "Keyword_Pattern_Element_List") {
    // Keyword_Pattern_Element_List -> Keyword_Pattern_Element Keyword_Pattern_Element_List | ε (gramática completa)
    for (auto filho : no->filhos) {
      Funcao::extrai_keyword_pattern_elementos(filho, func, nome_completo);
    }
  }
}

vector<Variavel*> Funcao::extrai_temporaries(No_arv_parse *no) {
  vector<Variavel*> variaveis;
  
  if (no == nullptr) return variaveis;
  
  // Temporaries_Opt -> Temporaries | ε (gramática completa)
  if (no->simb == "Temporaries_Opt") {
    if (no->filhos.size() > 0) {
      return extrai_temporaries(no->filhos[0]);
    }
    return variaveis;
  }
  
  // Temporaries -> TOKEN_pipe TOKEN_identifier Variable_List TOKEN_pipe (gramática completa)
  if (no->simb == "Temporaries") {
    if (no->filhos.size() >= 3) {
      // Primeira variável (TOKEN_identifier)
      if (no->filhos[1]->simb == "TOKEN_identifier") {
        Variavel* var = new Variavel();
        var->nome = ID::extrai_ID(no->filhos[1]);
        var->tipo = new ID();
        var->tipo->nome = "Object";
        var->tipo_semantico = var->inferir_tipo_semantico();
        variaveis.push_back(var);
      }
      
      // Outras variáveis da lista (Variable_List)
      vector<Variavel*> outras = Funcao::extrai_variable_list(no->filhos[2]);
      variaveis.insert(variaveis.end(), outras.begin(), outras.end());
    }
    return variaveis;
  }
  
  // Buscar recursivamente nos filhos
  for (auto filho : no->filhos) {
    vector<Variavel*> filhas = Funcao::extrai_temporaries(filho);
    variaveis.insert(variaveis.end(), filhas.begin(), filhas.end());
  }
  
  return variaveis;
}

vector<Variavel*> Funcao::extrai_variable_list(No_arv_parse *no) {
  vector<Variavel*> variaveis;
  
  if (no == nullptr) return variaveis;
  
  // Variable_List -> TOKEN_identifier Variable_List | ε (gramática completa)
  if (no->simb == "Variable_List") {
    if (no->filhos.size() >= 2) {
      // TOKEN_identifier Variable_List
      if (no->filhos[0]->simb == "TOKEN_identifier") {
        Variavel* var = new Variavel();
        var->nome = ID::extrai_ID(no->filhos[0]);
        var->tipo = new ID();
        var->tipo->nome = "Object";
        var->tipo_semantico = var->inferir_tipo_semantico();
        variaveis.push_back(var);
      }
      
      // Processar resto da lista recursivamente
      vector<Variavel*> resto = extrai_variable_list(no->filhos[1]);
      variaveis.insert(variaveis.end(), resto.begin(), resto.end());
    }
    // Se não há filhos, é a produção vazia (ε)
  }
  
  return variaveis;
}

vector<Comando*> Funcao::extrai_statements(No_arv_parse *no) {
  vector<Comando*> comandos;
  
  if (no == nullptr) return comandos;
  
  // Statement_Opt -> Statements | ε (gramática completa)
  if (no->simb == "Statement_Opt") {
    if (no->filhos.size() > 0) {
      return extrai_statements(no->filhos[0]);
    }
    return comandos;
  }
  
  // Statements -> Statement Statement_List (gramática completa)
  if (no->simb == "Statements") {
    if (no->filhos.size() >= 1) {
      Comando* cmd = Funcao::extrai_statement(no->filhos[0]);
      if (cmd != nullptr) {
        comandos.push_back(cmd);
      }
      
      if (no->filhos.size() >= 2) {
        vector<Comando*> resto = Funcao::extrai_statement_list(no->filhos[1]);
        comandos.insert(comandos.end(), resto.begin(), resto.end());
      }
    }
    return comandos;
  }
  
  // Buscar recursivamente nos filhos
  for (auto filho : no->filhos) {
    vector<Comando*> filhos_cmd = Funcao::extrai_statements(filho);
    comandos.insert(comandos.end(), filhos_cmd.begin(), filhos_cmd.end());
  }
  
  return comandos;
}

Comando* Funcao::extrai_statement(No_arv_parse *no) {
  if (no == nullptr) {
    cerr << "DEBUG: Funcao::extrai_statement - no é nullptr" << endl;
    return nullptr;
  }
  
  cerr << "DEBUG: Funcao::extrai_statement - processando " << no->simb << " com " << no->filhos.size() << " filhos" << endl;
  
  if (no->simb == "Statement") {
    // Statement pode ser Return_Statement ou Expression
    if (no->filhos.size() > 0) {
      cerr << "DEBUG: Statement filho[0]: " << no->filhos[0]->simb << endl;
      if (no->filhos[0]->simb == "Return_Statement") {
        cerr << "DEBUG: Processando Return_Statement" << endl;
        // Por simplicidade, tratar return como atribuição para uma variável especial
        return Funcao::extrai_return_statement(no->filhos[0]);
      } else if (no->filhos[0]->simb == "Expression") {
        cerr << "DEBUG: Processando Expression" << endl;
        return Funcao::extrai_expression_command(no->filhos[0]);
      } else {
        cerr << "DEBUG: Statement filho[0] não é Return_Statement nem Expression: " << no->filhos[0]->simb << endl;
      }
    } else {
      cerr << "DEBUG: Statement não tem filhos" << endl;
    }
  } else {
    cerr << "DEBUG: Nó não é Statement: " << no->simb << endl;
  }
  
  return nullptr;
}

Comando* Funcao::extrai_return_statement(No_arv_parse *no) {
  // Return_Statement -> TOKEN_return_operator Expression (gramática completa)
  if (no != nullptr && no->filhos.size() >= 2) {
    ComandoAtribuicao* cmd = new ComandoAtribuicao();
    cmd->esquerda = new ID();
    cmd->esquerda->nome = "__return__"; // Variável especial para return
    cmd->direita = Expressao::extrai_expressao(no->filhos[1]);
    return cmd;
  }
  return nullptr;
}

Comando* Funcao::extrai_expression_command(No_arv_parse *no) {
  // Por simplicidade, assumir que é uma atribuição
  if (no != nullptr && no->simb == "Expression") {
    cerr << "DEBUG: extrai_expression_command - chamando Comando::extrai_comando" << endl;
    Comando* resultado = Comando::extrai_comando(no);
    if (resultado == nullptr) {
      cerr << "DEBUG: Comando::extrai_comando retornou nullptr" << endl;
    } else {
      cerr << "DEBUG: Comando::extrai_comando retornou comando válido" << endl;
    }
    return resultado;
  }
  cerr << "DEBUG: extrai_expression_command - nó não é Expression ou é nullptr" << endl;
  return nullptr;
}

vector<Comando*> Funcao::extrai_statement_list(No_arv_parse *no) {
  vector<Comando*> comandos;
  
  if (no == nullptr) return comandos;
  
  if (no->simb == "Statement_List") {
    // Statement_List pode ser TOKEN_period, TOKEN_period Statement Statement_List, ou vazio
    for (auto filho : no->filhos) {
      if (filho->simb == "Statement") {
        Comando* cmd = Funcao::extrai_statement(filho);
        if (cmd != nullptr) {
          comandos.push_back(cmd);
        }
      } else if (filho->simb == "Statement_List") {
        vector<Comando*> resto = Funcao::extrai_statement_list(filho);
        comandos.insert(comandos.end(), resto.begin(), resto.end());
      }
    }
  }
  
  return comandos;
}

Funcao* Funcao::criar_funcao_implicita(No_arv_parse *no) {
  if (no == nullptr) return nullptr;
  
  // Encontrar statements no nível superior da Item_List
  vector<Comando*> comandos_encontrados;
  
  // Percorrer Item_List recursivamente procurando por Items que são statements
  buscar_statements_nivel_superior(no, comandos_encontrados);
  
  if (comandos_encontrados.empty()) {
    return nullptr;
  }
  
  // Criar função implícita com nome "main"
  Funcao* funcao = new Funcao();
  
  // Criar ID fictício para o nome da função
  ID* nome = new ID();
  nome->nome = "main";
  funcao->nome_funcao = nome;
  
  // Criar ID fictício para tipo de retorno padrão
  ID* tipo = new ID();
  tipo->nome = "INTEGER";
  funcao->tipo_retorno = tipo;
  
  // Sem parâmetros para função implícita
  funcao->parametros.clear();
  
  // Sem variáveis locais (por enquanto)
  funcao->variaveis.clear();
  
  // Usar os comandos encontrados
  funcao->comandos = comandos_encontrados;
  
  return funcao;
}

Funcao* Funcao::criar_funcao_statement_simples(No_arv_parse *no) {
  if (no == nullptr || no->simb != "Statement") return nullptr;
  
  // Extrair o comando do statement diretamente
  Comando* cmd = Funcao::extrai_statement(no);
  if (cmd == nullptr) {
    return nullptr;
  }
  
  // Criar função implícita com nome "main"
  Funcao* funcao = new Funcao();
  
  // Criar ID fictício para o nome da função
  ID* nome = new ID();
  nome->nome = "main";
  funcao->nome_funcao = nome;
  
  // Criar ID fictício para tipo de retorno padrão
  ID* tipo = new ID();
  tipo->nome = "INTEGER";
  funcao->tipo_retorno = tipo;
  
  // Sem parâmetros para função implícita
  funcao->parametros.clear();
  
  // Sem variáveis locais (por enquanto)
  funcao->variaveis.clear();
  
  // Usar o comando encontrado
  funcao->comandos.clear();
  funcao->comandos.push_back(cmd);
  
  return funcao;
}

void Funcao::buscar_statements_nivel_superior(No_arv_parse *no, vector<Comando*> &comandos) {
  if (no == nullptr) return;
  
  // Item -> Statement TOKEN_period (regra 4)
  if (no->simb == "Item" && no->filhos.size() >= 2) {
    if (no->filhos[0]->simb == "Statement") {
      Comando* cmd = Funcao::extrai_statement(no->filhos[0]);
      if (cmd != nullptr) {
        comandos.push_back(cmd);
      }
    }
  }
  
  // Buscar recursivamente em filhos
  for (auto filho : no->filhos) {
    buscar_statements_nivel_superior(filho, comandos);
  }
}

// Nova função para extrair variáveis de instância da classe
vector<Variavel*> Funcao::extrai_variaveis_instancia(No_arv_parse *no_raiz) {
  vector<Variavel*> variaveis_instancia;
  
  if (no_raiz == nullptr) return variaveis_instancia;
  
  // Buscar Class_Definition -> TOKEN_identifier TOKEN_subclass TOKEN_identifier TOKEN_left_bracket Class_Body TOKEN_right_bracket
  No_arv_parse* class_definition = buscar_no_por_simbolo(no_raiz, "Class_Definition");
  if (class_definition == nullptr) return variaveis_instancia;
  
  // Verificar se tem pelo menos 5 filhos para acessar Class_Body
  if (class_definition->filhos.size() < 5) return variaveis_instancia;
  
  No_arv_parse* class_body = class_definition->filhos[4]; // Class_Body
  if (class_body == nullptr || class_body->simb != "Class_Body") return variaveis_instancia;
  
  // Class_Body -> Temporaries Method_Definitions | Method_Definitions
  // Se o primeiro filho é Temporaries, são as variáveis de instância
  if (class_body->filhos.size() > 0 && class_body->filhos[0]->simb == "Temporaries") {
    variaveis_instancia = extrai_temporaries(class_body->filhos[0]);
  }
  
  return variaveis_instancia;
}

// Função auxiliar para buscar um nó por símbolo na árvore
No_arv_parse* Funcao::buscar_no_por_simbolo(No_arv_parse *no, const string& simbolo) {
  if (no == nullptr) return nullptr;
  
  if (no->simb == simbolo) {
    return no;
  }
  
  // Buscar recursivamente nos filhos
  for (auto filho : no->filhos) {
    No_arv_parse* resultado = buscar_no_por_simbolo(filho, simbolo);
    if (resultado != nullptr) {
      return resultado;
    }
  }
  
  return nullptr;
}
