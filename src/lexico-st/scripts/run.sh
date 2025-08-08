#!/bin/bash

# Script para executar o analisador léxico de Smalltalk

# Verifica se o diretório de classes compiladas existe
if [ ! -d "target/classes" ]; then
    echo "Erro: Diretório target/classes não encontrado."
    echo "Execute primeiro o script de compilação: ./scripts/compile.sh"
    exit 1
fi

# Verifica se o arquivo de classe principal existe
if [ ! -f "target/classes/lexical/SmallTalkLexer.class" ]; then
    echo "Erro: Classe SmallTalkLexer não foi compilada."
    echo "Verifique se o JavaCC está instalado e se a compilação foi bem-sucedida."
    echo "Execute: ./scripts/compile.sh"
    exit 1
fi

# Verifica se um arquivo foi especificado
if [ $# -ge 1 ]; then
    # Verifica se o arquivo de entrada existe
    if [ ! -f "$1" ]; then
        echo "Erro: Arquivo de entrada '$1' não encontrado."
        exit 1
    fi
    
    # Define o diretório de saída (pode ser passado como segundo parâmetro)
    if [ $# -eq 2 ]; then
        OUTPUT_DIR="$2"
    else
        OUTPUT_DIR="../../ins-tokens-st"
    fi
    
    # Cria o diretório de saída se não existir
    mkdir -p "$OUTPUT_DIR"
    
    # Obtém o nome base do arquivo sem o caminho e extensão
    FILENAME=$(basename -- "$1")
    BASENAME="${FILENAME%.*}"
    
    # Define o arquivo de saída
    OUTPUT_FILE="$OUTPUT_DIR/${BASENAME}.tokens"
    
    # Executa o analisador e redireciona a saída para o arquivo
    echo "Executando o analisador léxico para o arquivo: $1"
    echo "Gerando tokens em: $OUTPUT_FILE"
    java -cp target/classes lexical.SmallTalkLexer "$1" > "$OUTPUT_FILE"
    
    # Exibe confirmação
    echo "Análise concluída. Tokens salvos em $OUTPUT_FILE"
else
    # Instrui o usuário sobre como usar o script
    echo "Uso: ./run.sh <arquivo_entrada>"
    echo "Exemplo: ./run.sh samples/sample1.st"
fi
