/* *****************************************************************************
 * @project     Decomposição de Dantzig-Wolfe
 * @file        definicoes.h
 * @brief       Definições globais do projeto
 * @author      Gustavo H. Alves Pereira
 * ****************************************************************************/
#ifndef __DEFINICOES_DW_H__
#define __DEFINICOES_DW_H__

#define COEFICIENTE_NOVO_CUSTO_REDUZIDO 1
#define TAM_MAX_STR 100
#define TAM_MAX_LINHA_PROB 30
#define DW_TRUE (1 == 1)

#define PARADA_DW 1
#define ITERACOES_ANTES_DESISTIR 100000

// Cores
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

// Struct representando um tipo de corte, com largura e quantidade requerida
typedef struct TipoCorte {
        int largura;
        int qtdCortes;
}TipoCorte;

// Struct representando um problema, com tamanho máximo e n. de instâncias
typedef struct DescProblema {
        int tamanhoTotal;
        size_t numeroDeInstancias;
        TipoCorte * cortes;
}DescProblema;

#endif // __DEFINICOES_DW_H__

