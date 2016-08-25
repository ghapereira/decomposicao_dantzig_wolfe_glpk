/* *****************************************************************************
 * @project     Decomposição de Dantzig-Wolfe
 * @file        decomposicaoDW.c
 * @brief       Funções relacionadas com a decomposição de
 *              Dantzig-Wolfe
 * @author      Gustavo H. Alves Pereira
 * ****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <glpk.h>
#include <math.h>
#include "definicoes.h"
#include "matrizes.h"

/**
 * @function    criaProblemaMestre
 * @brief       Recebe um ponteiro para problema mestre e o prepara
 *              para uso, carregando os dados iniciais do problema
 * @brief       mestre: glp_prob *, ponteiro para um problema já inicializado
 * @brief       problema: DescProblema *, ponteiro para a estrutura do
 *              problema lido do arquivo
 * @brief       nomeProblema: const char *, nome do problema para impressão
 */
void criaProblemaMestre(glp_prob * mestre, DescProblema * problema,
                        const char * nomeProblema) {
        glp_set_prob_name(mestre, nomeProblema);
        // Define direção da otimização; no nosso caso, para o
        // problema mestre, só utilizaremos minimização
        glp_set_obj_dir(mestre, GLP_MIN);
        // Lida com as linhas
        inicializaLinhas(mestre, problema);
        // Lida com as colunas
        inicializaColunas(mestre, problema);
        // Inicializa a matriz
        carregaMatriz(problema, mestre, 0);
}

/**
 * @function    criaSubProblema
 * @brief       Recebe um ponteiro para subproblema e o prepara para uso,
 *              mantendo a linha de acoplamento. O que irá mudar neste
 *              subproblema será o valor dos custos reduzidos
 * @param       dw_subprob: glp_prob, ponteiro para o subproblema de
 *              Dantzig-Wolfe (que é um problema da mochila)
 * @param       problema: DescProblema *, ponteiro para estrutura com dados
 *              lidos do arquivo original
 */
void criaSubProblema(glp_prob * dw_subprob, DescProblema * problema) {
        size_t i;
        int * indices_i = (int *) calloc(problema->numeroDeInstancias + 1,
                                         sizeof(int)); // Apenas uma linha
        int * indices_j = (int *) calloc(problema->numeroDeInstancias + 1,
                                         sizeof(int));
        double * A = (double *) calloc(problema->numeroDeInstancias + 1,
                                       sizeof(double));

        glp_set_prob_name(dw_subprob, "subproblema");
        // Como o mestre é de minimização, o subproblema é de
        // maximização
        glp_set_obj_dir(dw_subprob, GLP_MAX);
        // Os custos reduzidos do subproblema são os resultados duais
        // da solução do mestre
        char nomeColuna[TAM_MAX_STR];
        glp_add_cols(dw_subprob, problema->numeroDeInstancias);
        for(i = 1; i <= problema->numeroDeInstancias; i++) {
                sprintf(nomeColuna, "a%ld", i);
                glp_set_col_name(dw_subprob, i, nomeColuna);
                // Colunas do subproblema são inteiras
                glp_set_col_kind(dw_subprob, i, GLP_IV);
                // Não-negatividade
                glp_set_col_bnds(dw_subprob, i, GLP_LO, 0, 0);
        }
        // Adiciona linha de restrição acopladora, que é imutável
        // durante a execução
        glp_add_rows(dw_subprob, 1);
        glp_set_row_name(dw_subprob, 1, "l");
        glp_set_row_bnds(dw_subprob, 1, GLP_UP, 0, problema->tamanhoTotal);
        // Preenche indices j e A
        for(i = 1; i <= problema->numeroDeInstancias; i++) {
                indices_i[i] = 1; // Apenas 1 linha
                indices_j[i] = i;
                A[i] = problema->cortes[i-1].largura;
        }
        // Carrega a matriz preparada
        glp_load_matrix(dw_subprob, problema->numeroDeInstancias,
                        indices_i, indices_j, A);
        // Limpando o ambiente
        free(A);
        free(indices_j);
        free(indices_i);
}

/**
 * @function    padroesDeCorteValidos
 * @brief       Imprime os padrões de corte válidos da solução do último
 *              mestre reduzido de Dantzig-Wolfe
 * @param       dw_mestre: glp_prob *, problema GLPK
 * @param       padrao_corte: int *, vetor onde são armazenados os cortes
 *              do padrão para exibição. Mantido como ponteiro para evitar
 *              constante alocação e desalocação de memória
 * @param       ind: int *, vetor dos índices do glp_get_mat_col
 * @param       val: double *, vetor dos valores do glp_get_mat_col
 * @param       problema: DescProblema *, ponteiro para os dados do problema
 *              trabalhado
 */
void padroesDeCorteValidos(glp_prob * dw_mestre,
                           int * padrao_corte,
                           int * ind,
                           double * val,
                           DescProblema * problema) {
        int col_len = 0;
        size_t i, j;

        printf(ANSI_COLOR_CYAN"Padrões válidos:"ANSI_COLOR_RESET"\n");
        for(i = 1; i <= glp_get_num_cols(dw_mestre); i++) {
                if(glp_get_col_prim(dw_mestre, i) > 0) {
                        printf("%d padrões de:\n",
                               (int)ceil(glp_get_col_prim(dw_mestre, i)));
                        /* Devido à maneira como é retornada a coluna, é
                         * necessário preparar o vetor que a armazena */
                        for(j = 1; j <= problema->numeroDeInstancias; j++) {
                                padrao_corte[j] = 0;
                        }
                        /* Obtém a coluna glp_get_mat_col retorna o número de
                         * variáveis que não estão em 0 na coluna, colocando
                         * seus índices e valores nos vetores ind e val,
                         * respectivamente. */
                        col_len = glp_get_mat_col(dw_mestre, i, ind, val);
                        for(j = 1; j <= col_len; j++) {
                                padrao_corte[ind[j]] = val[j];
                        }
                        for(j = 1; j <= problema->numeroDeInstancias; j++) {
                                // Mostra apenas cortes presentes no padrão
                                if(padrao_corte[j] >= 1) {
                                        printf("        %d cortes de largura %d\n",
                                                (int)floor(padrao_corte[j]),
                                                (int)floor(problema->cortes[j-1].largura)
                                        );
                                }
                        }
                }
        }
}

/**
 * @function    decomposicaoDantzigWolfe
 * @brief       Recebe os parâmetros do problema, monta
 *              e manda resolver usando a API do GLPK
 * @param       cortes: TipoCorte *, o vetor dos cortes utilizado
 * @param       problema: DescProblema *, informações sobre o problema
 * @param       nomeProblema: const char * com o nome do problema
 */
void decomposicaoDantzigWolfe(DescProblema * problema,
                              const char * nomeProblema) {
        // Problema mestre de Dantzig-Wolfe
        glp_prob * dw_mestre = glp_create_prob();
        // Subproblema de Dantzig-Wolfe
        glp_prob * dw_subprob = glp_create_prob();
        // Vetor de índices do problema
        int * ind = NULL;
        size_t i = 0, mestres_resolvidos = 0;

        // Define vetor coluna que armazena as soluções duais a serem
        // adicionadas como nova coluna ao mestre
        double * val;
        val = (double *) calloc((problema->numeroDeInstancias + 1), sizeof(double));
        // Aloca os índices das colunas a serem colocadas
        ind = (int *) calloc((problema->numeroDeInstancias + 1), sizeof(int));
        for(i = 0; i <= problema->numeroDeInstancias; i++) {
                ind[i] = i;
        }
        /* Vetor que armazena os padrões de corte no fim da execução. Ele tem
         * o tamanho (m linhas) + 1 porque haverão no máximo m elementos
         * preenchidos a cada iteração do método; conferir a necessidade disso
         * no método padroesDeCorteValidos, onde explico sobre a função
         * glp_get_mat_col */
        int * padrao_corte = (int *) calloc((problema->numeroDeInstancias + 1),
                                          sizeof(int));
        // Parâmetro de execução do simplex
        glp_smcp parm;
        glp_init_smcp(&parm);
        parm.msg_lev = GLP_MSG_ERR; // Exibe apenas mensagens de erro

        // Prepara o mestre e o subproblema iniciais
        criaProblemaMestre(dw_mestre, problema, nomeProblema);
        criaSubProblema(dw_subprob, problema);
        // Iterações do método
        do {
                // Resolve o simplex no mestre
                glp_simplex(dw_mestre, &parm);
                // Atribui resuldados da solução para uso do subproblema
                for(i = 1; i <= problema->numeroDeInstancias; i++) {
                       glp_set_obj_coef(dw_subprob, i,
                                        glp_get_row_dual(dw_mestre, i));
                }
                // Executa o simplex no subproblema
                glp_simplex(dw_subprob, &parm);
                // Condição de parada
                if(floor(glp_get_obj_val(dw_subprob)) <= PARADA_DW) {
                        break;
                }
                if(mestres_resolvidos > ITERACOES_ANTES_DESISTIR) {
                        printf(ANSI_COLOR_RED"Saída após %d iterações"ANSI_COLOR_RESET"\n",
                               ITERACOES_ANTES_DESISTIR);
                        break;
                }
                mestres_resolvidos++;
                // Obtem o resultado inteiro do subproblema
                for(i = 1; i <= problema->numeroDeInstancias; i++) {
                        val[i] = floor(glp_get_col_prim(dw_subprob, i));
                }
                // Com o resultado do subproblema cria nova coluna
                adicionaColunaAoProblema(
                        dw_mestre, // problema glp_prob
                        glp_get_num_cols(dw_mestre), // largura atual matriz
                        problema->numeroDeInstancias, // nlinhas
                        COEFICIENTE_NOVO_CUSTO_REDUZIDO,
                        ind, // indices
                        val // valores
                );
        }while(DW_TRUE);
        // Obtém os padrões de corte válidos
        // padroesDeCorteValidos(dw_mestre, padrao_corte, ind, val, problema);
        printf("Objetivo: %d\n", (int) ceil(glp_get_obj_val(dw_mestre)));
        printf("Mestres resolvidos: %ld\n", mestres_resolvidos);

        // Limpando o ambiente
        free(padrao_corte);
        free(ind);
        free(val);
        glp_delete_prob(dw_mestre);
        glp_free_env();
}

