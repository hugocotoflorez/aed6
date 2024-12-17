#include "funciones.h"
#include <string.h>
#include <stdio.h>

int
main(void)
{
    int s[N];
    NODO ns = {0};
    NODO ns2 = {0};

    printf("Matriz B: {\n");
    for (int i = 0; i < N; ++i)
    {
        printf("( ");
        for (int j = 0; j < N; ++j)
            printf("%2d ", B[i][j]);
        puts(")");
    }
    puts("}");

    char tipo = 0;

    while (tipo != 'e')
    {
        puts("");
        puts("Elige algoritmo");
        puts("[a] Asignacion Trivial");
        puts("[b] Asignacion Precisa");
        puts("[c] Backtracking");
        puts("[d] Backtracking (usadas)");
        puts("[e] Salir");
        printf(" >> ");

        scanf(" %c", &tipo);

        switch (tipo)
        {
            case 'a':
                memset(&ns, 0, sizeof ns);
                AsignacionTrivial( &ns);
                printf("Vector solucion asignacion trivial: ( ");
                for (int i = 0; i < N; ++i)
                    printf("%d ", ns.tupla[i]);
                puts(")");
                break;

            case 'b':
                memset(&ns, 0, sizeof ns);
                AsignacionPrecisa(&ns2);
                printf("Vector solucion asignacion precisa: ( ");
                for (int i = 0; i < N; ++i)
                    printf("%d ", ns2.tupla[i]);
                puts(")");
                break;

            case 'c':
                Backtracking(s);
                printf("Vector solucion sin usadas: ( ");
                for (int i = 0; i < N; ++i)
                    printf("%d ", s[i]);
                puts(")");
                break;

            case 'd':
                Backtracking_usadas(s);
                printf("Vector solucion con usadas: ( ");
                for (int i = 0; i < N; ++i)
                    printf("%d ", s[i]);
                puts(")");
                break;
        }
    }
    return 0;
}
