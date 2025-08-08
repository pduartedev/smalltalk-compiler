#include "TabelaLR1.hpp"
#include "matriz-util.hpp"

#include<vector>
#include<string>
#include<iostream>
#include<fstream>   
#include<sstream>
using namespace std;

int meu_stoi(string s) {
  return atoi(s.c_str());
}

void Transicao::inicializa(string tok){
  tipo = reducao = prox_estado = -1;
  mapa_LR2.clear();
  if (tok.size() == 0) {
    tipo = 0; //vazio
  } else if (tok[0] == 's') {
    tipo = 1; //shift
    //      cerr << "convertendo:" << tok.substr(1) << endl;
    prox_estado = meu_stoi(tok.substr(1));
  } else if (isdigit(tok[0])) {
    tipo = 2; // goto
    //      cerr << "convertendo:" << tok << endl;
    prox_estado = meu_stoi(tok);
  } else if (tok[0] == 'r') {// == r
    tipo = 3; //reducao
    //      cerr << "convertendo:" << tok.substr(1) << endl;
    reducao = meu_stoi(tok.substr(1));
  } else if (tok[0] == 'a') {
    tipo = 4; //aceita
  }else if (tok[0] == 'E') { // ESPEC_LR2
    tipo = 5;
    string str = tok;
    str.erase(0, str.find_first_of(':') + 1); // remove tudo ateh o primeiro :
    vector<string> regras_LR2 = vetor_de_uma_linha(str, '|');
    for (int i = 0; i < regras_LR2.size(); ++i) {
      vector<string> regra = vetor_de_uma_linha(regras_LR2[i], '_');
      mapa_LR2[regra[0]] = Transicao(regra[1]);
    }
  } else {
    tipo = 0;
  }
}

Transicao::Transicao(string tok){
  inicializa(tok);
}
Transicao::Transicao(){
  inicializa(string(""));
}

string Transicao::impressao() {
  string T = ((tipo == 0) ? "ZERO" :
	      (tipo == 1) ? "shift:" :
	      (tipo == 2) ? "goto:" :
	      (tipo == 3) ? "reduz:" :
	      (tipo == 4) ? "aceita" :
	      (tipo == 5) ? "especial_LR2" : "INVALIDO");
  string res = "(" + T + ",r=" + to_string(reducao) + ",prox=" + to_string(prox_estado) + ")";
  return res;
}

int maximo_coluna_int_como_string(const vector<vector<string> > M, int col) {
  int maxi = -1;
  for (int lin = 0; lin < M.size(); ++lin) {
    int estado = meu_stoi(M[lin][col]);
    if (maxi < estado) {
      maxi = estado;
    }
  }
  return maxi;
}

Tabela_LR1::Tabela_LR1(ifstream &arq_tabela_lr1) {
  // Formato: "estado simbolo transicao".
  // Ex. 0 NUM s3 (shift para estado 3 quando NUM na entrada no estado zero
  vector<vector<string> > mat = matriz_de_arquivo(arq_tabela_lr1, '\n', ' ');
  int numero_de_estados = 1 + maximo_coluna_int_como_string(mat, 0);
  Transicao t_vazio(string(""));
  Tab.resize(numero_de_estados);
  for (int lin = 0; lin < mat.size(); ++lin) {
    if (mat[lin].size() >= 2) {
      int est = meu_stoi(mat[lin][0]);//Ex. ("0", "NUM", "s2")
      string simbolo = mat[lin][1]; // simbolo = "NUM"
      cerr << est << " : " << simbolo << " -> " << Transicao(mat[lin][2]).impressao() << endl;
      Tab[est][simbolo] = Transicao(mat[lin][2]); // Transicao("s2")
    }
  }
}

void Tabela_LR1::debug() {
  cerr << "Tabela(" << Tab.size() << ")" << endl;
  for (int i = 0; i < Tab.size(); ++i) {
    for (map<string,Transicao>::iterator it = Tab[i].begin(); it != Tab[i].end(); ++it) {
      string simbolo = it->first;
      Transicao t = it->second;
      int tipo = t.tipo;
      cerr << i << " ";
      cerr << simbolo << " : ";
      cerr << (tipo == 0 ? "ZERO" :
	       (tipo == 1 ? "shift" :
		(tipo == 2 ? "goto" :
		 (tipo == 3 ? "reduce" :
		  (tipo == 4 ? "accept" :
		   (tipo == 5 ? "LR2" : "INVALIDO"))))));
      if (tipo == 1 || tipo == 2) {//shift e goto
        cerr << " " << t.prox_estado;
      } else if (tipo == 3) {
        cerr << " " << t.reducao;
      }
      cerr << ";";
      cerr << endl;
    }
  }
}

