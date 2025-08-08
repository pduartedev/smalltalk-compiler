CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -g

# Diretórios
SRC_DIR = src/semantico-st
GRAM_DIR = $(SRC_DIR)/src-gram-st
FRAME_DIR = $(SRC_DIR)/frame-st
TREE_DIR = $(SRC_DIR)/tree-st
BIN_DIR = bin
LEXIC_DIR = src/lexico-st
INS_ST_DIR = ins-st
TOKENS_DIR = ins-tokens-st

# Arquivos fonte principais
MAIN_SOURCES = $(SRC_DIR)/sol.cpp \
               $(SRC_DIR)/Arvore.cpp \
               $(SRC_DIR)/Gramatica.cpp \
               $(SRC_DIR)/Parser.cpp \
               $(SRC_DIR)/TabelaLR1.cpp \
               $(SRC_DIR)/matriz-util.cpp \
               $(SRC_DIR)/debug-util.cpp

# Arquivos fonte da gramática
GRAM_SOURCES = $(GRAM_DIR)/Analisador.cpp \
               $(GRAM_DIR)/Comando.cpp \
               $(GRAM_DIR)/ComandoAtribuicao.cpp \
               $(GRAM_DIR)/ComandoLista.cpp \
               $(GRAM_DIR)/ComandoIf.cpp \
               $(GRAM_DIR)/ComandoWhile.cpp \
               $(GRAM_DIR)/Expressao.cpp \
               $(GRAM_DIR)/ExpressaoBoolean.cpp \
               $(GRAM_DIR)/ExpressaoFloat.cpp \
               $(GRAM_DIR)/ExpressaoInt.cpp \
               $(GRAM_DIR)/ExpressaoLogica.cpp \
               $(GRAM_DIR)/ExpressaoRelacional.cpp \
               $(GRAM_DIR)/ExpressaoCondicional.cpp \
               $(GRAM_DIR)/ExpressaoChamadaFuncao.cpp \
               $(GRAM_DIR)/ExpressaoNumero.cpp \
               $(GRAM_DIR)/ExpressaoVariavel.cpp \
               $(GRAM_DIR)/ExpressaoSoma.cpp \
               $(GRAM_DIR)/ExpressaoSubtracao.cpp \
               $(GRAM_DIR)/ExpressaoMultiplicacao.cpp \
               $(GRAM_DIR)/ExpressaoDivisao.cpp \
               $(GRAM_DIR)/ExpressaoMod.cpp \
               $(GRAM_DIR)/ExpressaoBinaria.cpp \
               $(GRAM_DIR)/ExpressaoSelf.cpp \
               $(GRAM_DIR)/Funcao.cpp \
               $(GRAM_DIR)/ID.cpp \
               $(GRAM_DIR)/NUM.cpp \
               $(GRAM_DIR)/Tipo.cpp \
               $(GRAM_DIR)/Variavel.cpp

# Arquivos fonte do frame (Lab 4)
FRAME_SOURCES = $(FRAME_DIR)/FrameFuncao.cpp \
                $(FRAME_DIR)/FrameAcesso.cpp \
                $(FRAME_DIR)/FrameAcessoNoFrame.cpp \
                $(FRAME_DIR)/FrameAcessoTemp.cpp

# Arquivos fonte da representação intermediária (Lab 5)
TREE_SOURCES = $(TREE_DIR)/Stm.cpp \
               $(TREE_DIR)/Exp.cpp \
               $(TREE_DIR)/ExpTemp.cpp \
               $(TREE_DIR)/ExpConst.cpp \
               $(TREE_DIR)/ExpMem.cpp \
               $(TREE_DIR)/ExpBinop.cpp \
               $(TREE_DIR)/ExpCall.cpp \
               $(TREE_DIR)/StmMove.cpp \
               $(TREE_DIR)/StmExp.cpp \
               $(TREE_DIR)/StmCJump.cpp \
               $(TREE_DIR)/StmLabel.cpp \
               $(TREE_DIR)/StmSeq.cpp

# Gerador de RI (Lab 5)
RI_SOURCES = $(SRC_DIR)/GeradorRI.cpp

# Todos os arquivos fonte
ALL_SOURCES = $(MAIN_SOURCES) $(GRAM_SOURCES) $(FRAME_SOURCES) $(TREE_SOURCES) $(RI_SOURCES)

# Executável
TARGET = $(BIN_DIR)/analisador

# Regra principal
$(TARGET): $(ALL_SOURCES) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -I $(SRC_DIR) -o $@ $^

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(TOKENS_DIR):
	mkdir -p $(TOKENS_DIR)

# Setup inicial - prepara tudo automaticamente
setup: $(TARGET)
	@echo "🚀 Configurando projeto Smalltalk..."
	@cd $(LEXIC_DIR) && ./scripts/compile.sh
	@mkdir -p $(TOKENS_DIR)
	@echo "✅ Setup completo! Use 'make compile-N' para compilar arquivo N"

# Gerar tokens para um arquivo específico (automático)
tokens-%: $(TOKENS_DIR)
	@echo "🔤 Gerando tokens para $*.smalltalk..."
	@cd $(LEXIC_DIR) && ./scripts/run.sh ../../$(INS_ST_DIR)/$*.smalltalk ../../$(TOKENS_DIR)
	@echo "✅ Tokens salvos em $(TOKENS_DIR)/$*.tokens"

# Executar análise semântica em um arquivo (automático)
semantic-%: $(TARGET) tokens-%
	@echo "🧠 Executando análise semântica para $*.smalltalk..."
	@$(TARGET) src/gramatica-st/gramatica.conf src/gramatica-st/tabela_lr1.conf < $(TOKENS_DIR)/$*.tokens
	@echo "✅ Análise semântica de $*.smalltalk concluída!"

# Pipeline completo para um arquivo (COMANDO ÚNICO)
compile-%: semantic-%
	@echo "🎉 ✅ COMPILAÇÃO COMPLETA DE $*.smalltalk FINALIZADA!"

# Compilar arquivos em sequência
compile-batch-5:
	@for i in 1 2 3 4 5; do \
		echo ""; echo "🔄 ========== PROCESSANDO ARQUIVO $$i =========="; \
		make compile-$$i; \
	done
	@echo ""; echo "🎉 ✅ BATCH 1-5 CONCLUÍDO!"

compile-all: 
	@for i in $$(seq 1 30); do \
		echo ""; echo "🔄 ========== PROCESSANDO ARQUIVO $$i =========="; \
		make compile-$$i; \
	done
	@echo ""; echo "🎉 ✅ TODOS OS 30 ARQUIVOS PROCESSADOS!"

# Limpeza
clean:
	rm -rf $(BIN_DIR)
	rm -f *.o

# Help
help:
	@echo "📋 COMANDOS DISPONÍVEIS:"
	@echo ""
	@echo "🔧 SETUP E COMPILAÇÃO:"
	@echo "  make setup          - Configura tudo automaticamente"
	@echo "  make                - Compila o analisador com Lab 5"
	@echo ""
	@echo "📁 PROCESSAMENTO DE ARQUIVOS:"
	@echo "  make compile-N      - Processa arquivo N.smalltalk (N=1-30)"
	@echo "  make tokens-N       - Gera apenas tokens para arquivo N"
	@echo "  make semantic-N     - Executa apenas análise semântica do arquivo N"
	@echo ""
	@echo "📦 PROCESSAMENTO EM LOTE:"
	@echo "  make compile-batch-5   - Processa arquivos 1-5"
	@echo "  make compile-all       - Processa TODOS os 30 arquivos"
	@echo ""
	@echo "🧹 MANUTENÇÃO:"
	@echo "  make clean          - Remove binários"
	@echo "  make help           - Exibe esta ajuda"
	@echo ""
	@echo "🎯 EXEMPLO DE USO:"
	@echo "  make setup"
	@echo "  make compile-1     # Ver Lab 5 em ação!"

.PHONY: setup tokens-% semantic-% compile-% compile-batch-5 compile-all clean help
