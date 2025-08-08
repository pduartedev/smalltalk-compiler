#include "Comando.hpp"
#include "ComandoAtribuicao.hpp"
#include "ComandoLista.hpp"
#include "ComandoIf.hpp"
#include "ComandoWhile.hpp"
#include "Funcao.hpp"
#include "ID.hpp"
#include "Variavel.hpp"
#include <iostream>
#include "../debug-util.hpp"

vector<Comando*> Comando::extrai_lista_comandos(No_arv_parse* no) {
  // LC. 8) LC ->  9) LC -> C LC
  if (no->regra == 8) return vector<Comando*>();
  vector<Comando*> res;
  res.push_back(extrai_comando(no->filhos[0]));
  vector<Comando*> restante = extrai_lista_comandos(no->filhos[1]);
  res.insert(res.end(), restante.begin(), restante.end());
  return res;
}

Comando* Comando::extrai_comando(No_arv_parse* no) {
  if (no == nullptr) return nullptr;
  
  // Statement da gramática completa:
  // Statement -> Return_Statement | Expression
  if (no->simb == "Statement") {
    No_arv_parse* conteudo = no->filhos[0];
    
    // Return_Statement -> TOKEN_return_operator Expression (gramática completa)
    if (conteudo->simb == "Return_Statement") {
      // Por simplicidade, tratamos return como uma atribuição especial
      ComandoAtribuicao* cmd = new ComandoAtribuicao();
      cmd->esquerda = new ID();
      cmd->esquerda->nome = "$return"; // Nome especial para return
      cmd->direita = Expressao::extrai_expressao(conteudo->filhos[1]);
      return cmd;
    }
    // Expression (gramática completa)
    else if (conteudo->simb == "Expression") {
      return extrai_comando_de_expression(conteudo);
    }
  }
  
  // Expression -> Assignment | Basic_Expression (gramática completa)
  if (no->simb == "Expression") {
    return extrai_comando_de_expression(no);
  }
  
  // Assignment -> Assignment_Target TOKEN_assignment_operator Expression (gramática completa)
  if (no->simb == "Assignment") {
    ComandoAtribuicao* cmd = new ComandoAtribuicao();
    // Assignment_Target -> TOKEN_identifier (gramática completa)
    cmd->esquerda = ID::extrai_ID(no->filhos[0]->filhos[0]);
    cmd->direita = Expressao::extrai_expressao(no->filhos[2]);
    return cmd;
  }
  
  // Se não conseguir extrair, retorna nullptr
  return nullptr;
}

Comando* Comando::extrai_comando_de_expression(No_arv_parse* no) {
  if (no == nullptr) return nullptr;
  
  // Expression -> Assignment | Basic_Expression (gramática completa)
  for (auto filho : no->filhos) {
    if (filho->simb == "Assignment") {
      // Assignment -> Assignment_Target TOKEN_assignment_operator Expression (gramática completa)
      ComandoAtribuicao* cmd = new ComandoAtribuicao();
      if (filho->filhos.size() >= 3) {
        cmd->esquerda = ID::extrai_ID(filho->filhos[0]); // Assignment_Target
        cmd->direita = Expressao::extrai_expressao(filho->filhos[2]); // Expression
      }
      return cmd;
    }
    
    // Basic_Expression - verificar se é uma estrutura WHILE antes de tratar como comando de expressão
    if (filho->simb == "Basic_Expression") {
      // Primeiro, tentar extrair um comando WHILE
      ComandoWhile* while_cmd = extrai_comando_while(filho);
      if (while_cmd != nullptr) {
        return while_cmd;
      }
      
      // Se não for WHILE, criar um comando de atribuição temporário para expressões simples
      ComandoAtribuicao* cmd = new ComandoAtribuicao();
      cmd->esquerda = new ID();
      cmd->esquerda->nome = "__temp__"; // Variável temporária para resultado da expressão
      cmd->direita = Expressao::extrai_expressao(filho);
      return cmd;
    }
  }
  
  // Se não conseguir extrair nenhum comando, retorna nullptr
  return nullptr;
}

ComandoWhile* Comando::extrai_comando_while(No_arv_parse* no) {
  if (no == nullptr) return nullptr;
  
  // Basic_Expression -> Primary Message_Sequence
  if (no->simb == "Basic_Expression" && no->filhos.size() >= 2) {
    No_arv_parse* primary = no->filhos[0];
    No_arv_parse* message_sequence = no->filhos[1];
    
    // Verificar se o Primary é um Block_Constructor (condição do while)
    if (primary->simb == "Primary" && primary->filhos.size() >= 1) {
      No_arv_parse* block_constructor = primary->filhos[0];
      if (block_constructor->simb == "Block_Constructor") {
        
        // Verificar se Message_Sequence contém whileTrue:
        if (message_sequence->simb == "Message_Sequence" && message_sequence->filhos.size() >= 1) {
          No_arv_parse* message_chain = message_sequence->filhos[0];
          
          if (message_chain->simb == "Message_Chain" && message_chain->filhos.size() >= 1) {
            No_arv_parse* keyword_message = message_chain->filhos[0];
            
            if (keyword_message->simb == "Keyword_Message" && keyword_message->filhos.size() >= 1) {
              No_arv_parse* keyword_element = keyword_message->filhos[0];
              
              if (keyword_element->simb == "Keyword_Message_Element" && keyword_element->filhos.size() >= 2) {
                string keyword = keyword_element->filhos[0]->dado_extra;
                
                if (keyword == "whileTrue:" || keyword == "whileFalse:") {
                  // Extrair a condição do primeiro bloco
                  Expressao* condicao = extrair_expressao_do_bloco(block_constructor);
                  
                  // Extrair os comandos do segundo bloco (corpo do loop)
                  vector<Comando*> comandos_loop = extrair_comandos_do_bloco_while(keyword_element->filhos[1]);
                  
                  // Criar o comando while
                  ComandoWhile* cmd_while = new ComandoWhile();
                  cmd_while->condicao = condicao;
                  cmd_while->comandos_loop = comandos_loop;
                  
                  return cmd_while;
                }
              }
            }
          }
        }
      }
    }
  }
  
  return nullptr;
}

Expressao* Comando::extrair_expressao_do_bloco(No_arv_parse* block_constructor) {
  if (block_constructor == nullptr) return nullptr;
  
  // Block_Constructor -> TOKEN_left_bracket Block_Body_Content TOKEN_right_bracket
  if (block_constructor->simb == "Block_Constructor" && block_constructor->filhos.size() >= 2) {
    No_arv_parse* block_body = block_constructor->filhos[1];
    
    if (block_body->simb == "Block_Body_Content" && block_body->filhos.size() >= 2) {
      // Block_Body_Content -> Temporaries_Opt Block_Statements_Opt
      No_arv_parse* block_statements = block_body->filhos[1];
      
      if (block_statements->simb == "Block_Statements_Opt" && block_statements->filhos.size() >= 1) {
        No_arv_parse* statements = block_statements->filhos[0];
        
        if (statements->simb == "Block_Statements" && statements->filhos.size() >= 1) {
          No_arv_parse* statement = statements->filhos[0];
          
          if (statement->simb == "Statement" && statement->filhos.size() >= 1) {
            No_arv_parse* expression = statement->filhos[0];
            
            if (expression->simb == "Expression") {
              return Expressao::extrai_expressao(expression);
            }
          }
        }
      }
    }
  }
  
  return nullptr;
}

vector<Comando*> Comando::extrair_comandos_do_bloco_while(No_arv_parse* no) {
  vector<Comando*> comandos;
  
  if (no == nullptr) return comandos;
  
  // Keyword_Argument pode ser apenas Primary (Block_Constructor)
  if (no->simb == "Keyword_Argument" && no->filhos.size() >= 1) {
    No_arv_parse* primary = no->filhos[0];
    
    if (primary->simb == "Primary" && primary->filhos.size() >= 1) {
      No_arv_parse* block_constructor = primary->filhos[0];
      
      if (block_constructor->simb == "Block_Constructor" && block_constructor->filhos.size() >= 2) {
        No_arv_parse* block_body = block_constructor->filhos[1];
        
        if (block_body->simb == "Block_Body_Content" && block_body->filhos.size() >= 2) {
          // Block_Body_Content -> Temporaries_Opt Block_Statements_Opt
          No_arv_parse* block_statements = block_body->filhos[1];
          
          if (block_statements->simb == "Block_Statements_Opt" && block_statements->filhos.size() >= 1) {
            No_arv_parse* statements = block_statements->filhos[0];
            
            comandos = Comando::extrai_block_statements(statements);
          }
        }
      }
    }
  }
  
  return comandos;
}

void Comando::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "Comando generico"<< endl;
}

bool Comando::verificar_tipos_semanticos(vector<Variavel*>& variaveis, const vector<Variavel*>& parametros) {
    // Implementação padrão - retorna true
    return true;
}

void Comando::extrai_block_content(No_arv_parse* no, ComandoLista* cmd) {
  if (no == nullptr || cmd == nullptr) return;
  
  if (no->simb == "Block_Body_Content") {
    // Block_Body_Content -> Temporaries_Opt Block_Statements_Opt
    if (no->filhos.size() >= 2) {
      // Extrair variáveis temporárias
      if (no->filhos[0]->simb == "Temporaries_Opt") {
        cmd->lista_variaveis = Funcao::extrai_temporaries(no->filhos[0]);
      }
      
      // Extrair comandos
      if (no->filhos[1]->simb == "Block_Statements_Opt") {
        cmd->lista_comandos = Comando::extrai_block_statements(no->filhos[1]);
      }
    }
  }
}

vector<Comando*> Comando::extrai_block_statements(No_arv_parse* no) {
  vector<Comando*> comandos;
  
  if (no == nullptr) {
    cerr << "DEBUG: extrai_block_statements - no é nullptr" << endl;
    return comandos;
  }
  
  cerr << "DEBUG: extrai_block_statements - processando " << no->simb << endl;
  
  if (no->simb == "Block_Statements") {
    cerr << "DEBUG: Block_Statements encontrado com " << no->filhos.size() << " filhos" << endl;
    // Block_Statements -> Statement Block_Statement_List
    if (no->filhos.size() >= 1) {
      cerr << "DEBUG: Processando Statement filho[0]: " << no->filhos[0]->simb << endl;
      Comando* cmd = Funcao::extrai_statement(no->filhos[0]);
      if (cmd != nullptr) {
        cerr << "DEBUG: Comando extraído com sucesso!" << endl;
        comandos.push_back(cmd);
      } else {
        cerr << "DEBUG: Funcao::extrai_statement retornou nullptr" << endl;
      }
      
      if (no->filhos.size() >= 2) {
        cerr << "DEBUG: Processando Block_Statement_List filho[1]: " << no->filhos[1]->simb << endl;
        vector<Comando*> resto = Comando::extrai_block_statement_list(no->filhos[1]);
        cerr << "DEBUG: extrai_block_statement_list retornou " << resto.size() << " comandos" << endl;
        comandos.insert(comandos.end(), resto.begin(), resto.end());
      }
    }
  } else {
    cerr << "DEBUG: Nó não é Block_Statements, é: " << no->simb << endl;
  }
  
  return comandos;
}

vector<Comando*> Comando::extrai_block_statement_list(No_arv_parse* no) {
  vector<Comando*> comandos;
  
  if (no == nullptr) return comandos;
  
  if (no->simb == "Block_Statement_List") {
    for (auto filho : no->filhos) {
      if (filho->simb == "Statement") {
        Comando* cmd = Funcao::extrai_statement(filho);
        if (cmd != nullptr) {
          comandos.push_back(cmd);
        }
      } else if (filho->simb == "Block_Statement_List") {
        vector<Comando*> resto = Comando::extrai_block_statement_list(filho);
        comandos.insert(comandos.end(), resto.begin(), resto.end());
      }
    }
  }
  
  return comandos;
}
