#!/bin/bash

# Script para compilar o analisador léxico de Smalltalk

# Cria os diretórios necessários caso não existam
mkdir -p main/java/lexical
mkdir -p target/classes

# Executa o JavaCC para gerar os arquivos Java
javacc -OUTPUT_DIRECTORY=main/java/lexical main/jj/lexicGrammar.jj

# Compila os arquivos Java gerados
javac -d target/classes main/java/lexical/*.java

echo "Compilação concluída!"
