#include <stdio.h>
#include <glpk.h>

// Lida com o problem
void problemaStidsen(void){
        double z, x1, x2, x3;

        glp_prob *lp;
        lp = glp_create_prob();

        glp_set_prob_name(lp, "Problema exemplo Stidsen");
        // Define direção da otimização
        glp_set_obj_dir(lp, GLP_MIN);

        // Insere linhas
        const int NumeroDeLinhas = 3;
        glp_add_rows(lp, NumeroDeLinhas);
        // Nomeia as linhas
        glp_set_row_name(lp, 1, "p");
        glp_set_row_name(lp, 2, "q");
        glp_set_row_name(lp, 3, "r");

        // Limites das linhas. No nosso caso, limite inferior do
        // problema de minimização
        glp_set_row_bnds(lp, 1, GLP_LO, 44, 0);
        glp_set_row_bnds(lp, 2, GLP_LO, 3, 0);
        glp_set_row_bnds(lp, 3, GLP_LO, 48, 0);

        // Insere colunas
        const int NumeroDeColunas = 3;
        glp_add_cols(lp, NumeroDeColunas);
        // Nomeia as colunas
        glp_set_col_name(lp, 1, "x1");
        glp_set_col_name(lp, 2, "x2");
        glp_set_col_name(lp, 3, "x3");

        // Limites das colunas
        glp_set_col_bnds(lp, 1, GLP_LO, 0, 0);
        glp_set_col_bnds(lp, 2, GLP_LO, 0, 0);
        glp_set_col_bnds(lp, 3, GLP_LO, 0, 0);

        // Prepara os coeficientes do objetivo
        // glp_set_obj_coef(PROBLEMA, COLUNA, COEFICIENTE DA COLUNA)
        glp_set_obj_coef(lp, 1, 1);
        glp_set_obj_coef(lp, 2, 1);
        glp_set_obj_coef(lp, 3, 1);

        // Matriz A
        int ia[10], ja[10];
        double ar[10];

        ia[1] = 1, ja[1] = 1, ar[1] = 1; // (1, 1) = 1
        ia[2] = 1, ja[2] = 2, ar[2] = 0;
        ia[3] = 1, ja[3] = 3, ar[3] = 0;
        ia[4] = 2, ja[4] = 1, ar[4] = 0;
        ia[5] = 2, ja[5] = 2, ar[5] = 1; // (2, 2) = 1
        ia[6] = 2, ja[6] = 3, ar[6] = 0;
        ia[7] = 3, ja[7] = 1, ar[7] = 0;
        ia[8] = 3, ja[8] = 2, ar[8] = 0;
        ia[9] = 3, ja[9] = 3, ar[9] = 1; // (3, 3) = 1

        glp_load_matrix(lp, NumeroDeLinhas * NumeroDeColunas, ia, ja, ar);

        // Usa o simplex
        glp_simplex(lp, NULL);
        z = glp_get_obj_val(lp);
        x1 = glp_get_row_dual(lp, 1);
        x2 = glp_get_row_dual(lp, 2);
        x3 = glp_get_row_dual(lp, 3);
        printf("MEU OBJETIVO É %lf\n", z);
        printf("X1: %lf, X2: %lf, X3: %lf\n", x1, x2, x3);

        glp_delete_prob(lp);
        glp_free_env();
}
