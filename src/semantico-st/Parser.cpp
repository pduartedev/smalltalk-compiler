#include "Parser.hpp"

Parser::Parser(ifstream &arq_gramatica, ifstream &arq_tabela_lr1) :
  gram(arq_gramatica),
  tabela(arq_tabela_lr1) 
{}

string to_upper(string str) {
  transform(str.begin(), str.end(),str.begin(), ::toupper);
  return str;
}

string meu_getline(istream &input) {
  char s[2001];
  input.getline(s, 2000, '\n');
  string resultado(s);
  return resultado;
}

void le_terminal(const string &linha_entr, string &tipo_terminal, string &texto_terminal) {
  string x = linha_entr.substr(0, linha_entr.find_first_of(' '));
  string y = linha_entr.substr(linha_entr.find_first_of(' ') + 1);
  //  tipo_terminal = to_upper(x); // Se voce estiver usando
  // case-insensitive nao tratado no lexico.  Nesse caso, voce nao
  // pode usar nome de nao terminal maiusculo que coincida com nome de
  // terminal minusculo
  tipo_terminal = x;
  texto_terminal = y;
  cerr << "LENDO: " << tipo_terminal << " " << texto_terminal << endl;
}

void espia_proximo_tipo_terminal(const string &linha_entr, string &tipo_terminal) {
  string x = linha_entr.substr(0, linha_entr.find_first_of(' '));
  tipo_terminal = x;
  cerr << "ESPIOU: " << tipo_terminal << endl;
}

void debug_desempilha(stack<pair<No_arv_parse *,int> > &p) {
  int posicao = 0;
  while(!p.empty()) {
    cerr << posicao << "=posicao na pilha" << endl;
    No_arv_parse::debug_no(p.top().first);
    p.pop();
    posicao++;
  }
}

Arvore_parse Parser::executa_parse(istream &input) {
  vector<string> linhas_entrada;
  while(!input.eof()) {
    string x = meu_getline(input);
    if (x.find_first_not_of(' ') != string::npos) {
      linhas_entrada.push_back(x);
    }
  }
  cerr << "executa_parse(" << endl;
  for (int i = 0; i < linhas_entrada.size(); ++i) {
    cerr << linhas_entrada[i] << endl;
  }
  cerr << ")FIM_executa_parse" << endl;
  int estado_atual = 0;
  string tipo_terminal_lookahead;
  string texto_terminal;
  int pos_entrada = 0 ;
  le_terminal(linhas_entrada[pos_entrada++], tipo_terminal_lookahead, texto_terminal);
  bool terminou = false;
  do {
    cerr << "Estado:" << estado_atual << " Lookahead:" <<  tipo_terminal_lookahead << endl;
    Transicao t = tabela.Tab[estado_atual][tipo_terminal_lookahead];
    cerr << "Transicao:"<<t.impressao() << endl;
    if (t.tipo == 5) {
      cerr << " REGRA LR1 insuficiente. Utilizando regra LR2" << endl;
      string lookahead_2;
      espia_proximo_tipo_terminal(linhas_entrada[pos_entrada], lookahead_2);
      Transicao regra_LR2 = t.mapa_LR2[lookahead_2];
      t = regra_LR2;
      cerr << "Nova LR2 Transicao:"<<t.impressao() << endl;
    }
    switch(t.tipo) {
    case 0: { return Arvore_parse(NULL); break; } //erro
    case 1: { // terminal
        No_arv_parse * ap_no = new No_arv_parse;
        ap_no->simb = tipo_terminal_lookahead;
	ap_no->dado_extra = texto_terminal;
	//cerr << "Empilhando:" << estado_atual << ":";
	//	No_arv_parse::debug_no(pilha.top().first);
        pilha.push(make_pair(ap_no,estado_atual));

        estado_atual = t.prox_estado;
	if (pos_entrada != linhas_entrada.size())  {
	  le_terminal(linhas_entrada[pos_entrada++], tipo_terminal_lookahead, texto_terminal);
	} else {
	  terminou = true;
	}
	break;
    }
    case 2: { //goto
      cerr << "ERRO goto em lookahead."<<endl;
      return Arvore_parse(NULL);
    }
    case 3: //reducao
      {
        Regra r = gram.R[t.reducao];
	//	r.debug();
        No_arv_parse * ap_no = new No_arv_parse;
        ap_no->simb = r.esq;
        ap_no->regra = t.reducao;
        ap_no->filhos.resize(r.dir.size());
        int estado = estado_atual; 
	//	cerr << "BLOCO DE EMPILHAMENTO" << endl;
        for(int i = 0; i < r.dir.size(); ++i) {
	  //	  	  cerr << "DesEmpilha:" <<  pilha.top().second << ":";
	  //	  No_arv_parse::debug_no(pilha.top().first);
          ap_no->filhos[r.dir.size() - i - 1] = pilha.top().first;
          estado = pilha.top().second;
          pilha.pop();
        }
	//	cerr << "Empilhando:" << estado << ":";
	//	No_arv_parse::debug_no(ap_no);
        pilha.push(make_pair(ap_no,estado));
	//	cerr << "FIM BLOCO DE EMPILHAMENTO" << endl;
        Transicao go_to = tabela.Tab[estado][r.esq];
        if (go_to.tipo != 2) {
          cerr << "ausencia de goto apos reducao. Estado="<< estado << ", lookahead="<< r.esq << "]->" << go_to.impressao() << endl;
	  debug_desempilha(pilha);
          return Arvore_parse(NULL);
        }
        estado_atual = go_to.prox_estado;
	break;
      }
    case 4: {
      cerr << "   QUASE FIM PARSE" << endl;
      No_arv_parse * retorno = pilha.top().first;
      debug_desempilha(pilha);
      cerr << "   FIM PARSE" << endl;
      return Arvore_parse(retorno);
      break;
    }
    default: {
      cerr << "Codigo invalido" << endl;
      return Arvore_parse(NULL);
    }
    }
  }while(!terminou);
  cerr << "   ------------------------------------------------------------------" << endl;
  cerr << "   PARSE Terminou por fim de arquivo. Faltou o $ no final da entrada?" << endl;
  cerr << "   ------------------------------------------------------------------" << endl;
  No_arv_parse * retorno = pilha.top().first;
  debug_desempilha(pilha);
  return Arvore_parse(retorno);
}

