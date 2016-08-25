/* *****************************************************************************
 * @project     Decomposição de Dantzig-Wolfe
 * @file        matrizes.c
 * @brief       Funções de carga de matrizes
 * @author      Gustavo H. Alves Pereira
 * ****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <glpk.h>
#include "definicoes.h"

/**
 * @function    carregaMatriz
 * @brief       Carrega a matriz para o problema, segundo a maneira do
 *              GLPK
 * @param       problema: DescProblema *, a descrição do problema
 */
void carregaMatriz(DescProblema * problema, glp_prob * lp, short verbose) {
        size_t i = 1, j = 1, k = 1;
        int tamanhoMatriz;
        // Matrizes do problema
        int *indices_i, *indices_j;
        double *A;
        // Inicializa a matriz A do problema como a identidade
        // Utilizo +1 porque as matrizes do GLPK são indexadas
        // a partir do 1
        tamanhoMatriz = problema->numeroDeInstancias *
                        problema->numeroDeInstancias;
        indices_i = (int *) calloc(tamanhoMatriz + 1, sizeof(int));
        indices_j = (int *) calloc(tamanhoMatriz + 1, sizeof(int));
        A = (double *) calloc(tamanhoMatriz + 1, sizeof(size_t));

        for(i = 1; i <= problema->numeroDeInstancias; i++) {
                for(j = 1; j <= problema->numeroDeInstancias; j++) {
                        indices_i[k] = (int)i;
                        indices_j[k] = (int)j;
                        if(i == j) {
                                A[k] = (double)1;
                        } else {
                                A[k] = (double)0;
                        }

                        if(verbose) {
                                printf("ia[%ld] = %d, ja[%ld] = %d,  ar[%ld] = %lf\n",
                                        k, indices_i[k], k, indices_j[k], k, A[k]);
                        }
                        k++;
                }
        }

        glp_load_matrix(lp, tamanhoMatriz, indices_i, indices_j, A);

        // Limpando o ambiente
        free(indices_i);
        free(indices_j);
        free(A);
}

void inicializaLinhas(glp_prob * lp, DescProblema * problema) {
        // Contém o nome da linha, para fins de identificação
        char nomeLinha[TAM_MAX_STR];
        size_t i;
        glp_add_rows(lp, problema->numeroDeInstancias);
        for(i = 1; i <= problema->numeroDeInstancias; i++) {
                // Identifica cada linha como "r_i" (restrição i)
                sprintf(nomeLinha, "r%ld", i);
                glp_set_row_name(lp, i, nomeLinha);

                // Coloca o limite da linha: limite inferior do problema
                //   de minimização
                glp_set_row_bnds(lp, i, GLP_LO,
                                 problema->cortes[i-1].qtdCortes, 0);
        }
}

void inicializaColunas(glp_prob * lp, DescProblema * problema) {
        // Contém o nome da linha, para fins de identificação
        char nomeColuna[TAM_MAX_STR];
        size_t i;
        glp_add_cols(lp, problema->numeroDeInstancias);
        for(i = 1; i <= problema->numeroDeInstancias; i++) {
                sprintf(nomeColuna, "x%ld", i);
                glp_set_col_name(lp, i, nomeColuna);
                glp_set_col_bnds(lp, i, GLP_LO, 0, 0);
                // Inicializa todos os custos reduzidos como 1
                glp_set_obj_coef(lp, i, 1);
        }
}

/**
 * @function    adicionaColunaAoProblema
 * @brief       Adiciona uma coluna x_(n+1) ao problema, onde n é o número
 *              atual de colunas
 * @param       lp: glp_prob *, estrutura de problema do GLPK
 * @param       larguraAtualMatriz: int, largura da matriz ANTES da
 *              atualização de coluna
 *              nLinhas: int, número de linhas da matriz
 * @param       coefObjetivo: double, coeficiente da variável a ser
 *              incluída na função objetivo
 * @param       indices: int *, vetor dos índices da coluna
 *              caso seja uma coluna com 2 elementos, por exemplo,
 *              indices = {0, 1, 2}, visto que o GLPK indexa a partir
 *              de 1, sendo necessário que colunas de tamanho n tenham
 *              alocados n+1 espaços
 * @param       valores: double *, vetor dos valores da coluna.
 *              A descrição do tamanho deste vetor é correspondente ao do
 *              de índices
 */
void adicionaColunaAoProblema(glp_prob * lp,
        int larguraAtualMatriz, int nLinhas,
        double coefObjetivo, int * indices, double * valores) {

        char nomeCol[TAM_MAX_STR];
        // Adiciona uma única coluna ao problema
        glp_add_cols(lp, 1);
        // Nomeia coluna
        larguraAtualMatriz++;
        sprintf(nomeCol, "x%d", larguraAtualMatriz);
        glp_set_col_name(lp, larguraAtualMatriz, nomeCol);
        // Limites
        glp_set_col_bnds(lp, larguraAtualMatriz, GLP_LO, 0, 0);
        glp_set_obj_coef(lp, larguraAtualMatriz, coefObjetivo);
        // glp_set_mat_col(PROBLEMA, Índice Coluna a ser mudada,
        // tamanho do vetor coluna, índices, valores
        glp_set_mat_col(lp, larguraAtualMatriz, nLinhas, indices, valores);
}

