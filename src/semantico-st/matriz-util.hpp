#ifndef _MATRIZ_UTIL_
#define _MATRIZ_UTIL_
#include<vector>
#include<string>
#include<iostream>
#include <fstream>   
#include<sstream>
using namespace std;

vector<string> vetor_de_uma_linha(string linha, char delim_col);

vector<vector<string> > matriz_de_arquivo(ifstream &arq, char delim_lin, char delim_col);

void debug_mat(vector<vector<string> > mat);

#endif
