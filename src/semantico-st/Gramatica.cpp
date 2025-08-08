#include "Gramatica.hpp"
#include "matriz-util.hpp"
#include<vector>
#include<string>
#include<iostream>
#include<fstream>   
#include<sstream>
using namespace std;

void Regra::debug() {
  cerr << "Regra:" << esq << "->[" << dir.size() << "]->";
  for (int i = 0 ; i < dir.size(); ++i) {
    cerr << dir[i] << ":";
  }
  cerr << endl;
}

Gramatica::Gramatica(ifstream &arq_gramatica) {
    vector<vector<string> > mat = matriz_de_arquivo(arq_gramatica, '\n', ' ');
    if (mat[mat.size()-1].size() == 1) mat.pop_back(); //remove ultima linha vazia.
    //    debug_mat(mat);
    for (int i = 0; i < mat.size(); ++i) {
      //      cerr << "Gramatica("<< i << ":" << mat[i].size() << ":" << mat[i][0] << ")" << endl;
      if (mat[i].size() < 2) {
        cerr << "Erro leitura gramatica 1:"<< i << ":" << mat[i].size() << ":" << mat[i][0] << ":" << endl;
      } else {
        Regra r;
        r.esq = mat[i][0];
        // ignora-se a seta em mat[i][1].
        for (int j = 2; j < mat[i].size();++j) {
	  if (mat[i][j].size() > 0) {
	    r.dir.push_back(mat[i][j]);
	  }
        }
        R.push_back(r);
      }
    }
  }

void Gramatica::debug() {
  for (int r = 0; r < R.size(); ++r) {
    cerr << R[r].esq << "->";
    for (int j = 0; j < R[r].dir.size(); ++j){
      cerr << ":" << R[r].dir[j];
    }
    cerr << endl;
  }
}
