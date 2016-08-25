#ifndef __DW_MATRIZES_H__
#define __DW_MATRIZES_H__

/**
 * @function    carregaMatriz
 * @brief       Carrega a matriz para o problema, segundo a maneira do
 *              GLPK
 * @param       problema: DescProblema *, a descrição do problema
 */
void carregaMatriz(DescProblema * problema, glp_prob * lp, short verbose);


void inicializaLinhas(glp_prob * lp, DescProblema * problema);

void inicializaColunas(glp_prob * lp, DescProblema * problema);

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
        double coefObjetivo, int * indices, double * valores);

#endif // __DW_MATRIZES_H__

