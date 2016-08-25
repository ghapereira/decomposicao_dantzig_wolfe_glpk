#ifndef __DECOMPOSICAO_DW__
#define __DECOMPOSICAO_DW__
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
                        const char * nomeProblema);


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
void criaSubProblema(glp_prob * dw_subprob, DescProblema * problema);


/**
 * @function    decomposicaoDantzigWolfe
 * @brief       Recebe os parâmetros do problema, monta
 *              e manda resolver usando a API do GLPK
 * @param       cortes: TipoCorte *, o vetor dos cortes utilizado
 * @param       problema: DescProblema *, informações sobre o problema
 * @param       nomeProblema: const char * com o nome do problema
 */
void decomposicaoDantzigWolfe(DescProblema * problema,
                              const char * nomeProblema);

#endif // __DECOMPOSICAO_DW__

