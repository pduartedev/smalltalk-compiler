#include "matriz-util.hpp"
#define TAM_LIN 1000

vector<string> vetor_de_uma_linha(string linha, char delim_col) {
  vector<string> res;
  istringstream input_lin(linha);
  while(!input_lin.eof()) {
    char col_aux[TAM_LIN];
    input_lin.getline(col_aux, TAM_LIN-1, delim_col);
    string col(col_aux);
    res.push_back(col);
  }
  return res;
}

vector<vector<string> > matriz_de_arquivo(ifstream &arq, char delim_lin, char delim_col) {
  vector<vector<string> > mat;
  while(!arq.eof()) {
    char lin_aux[TAM_LIN];
    arq.getline(lin_aux, TAM_LIN-1, delim_lin);
    string linha(lin_aux);
    vector<string> vec_linha = vetor_de_uma_linha(linha, delim_col);
    mat.push_back(vec_linha);
    /*    if (mat.size() > 1 && (mat[0].size() != mat[mat.size()-1].size())){
      cerr << "matriz nao retangular(" << mat[0].size() << "," << mat[mat.size()-1].size() << ")" <<endl; 
      }*/
  }
  return mat;
}

void debug_mat(vector<vector<string> > mat) {
  for (int l = 0; l < mat.size(); ++l){ 
    cerr << mat[l].size() << "(";
    for (int c = 0; c < mat[l].size();++c){
      cerr <<mat[l][c] << ":";
    }
    cerr << endl;
  }
}
