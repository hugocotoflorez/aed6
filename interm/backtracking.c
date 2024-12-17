#include "funciones.h"
#include "lista.h"
#include <stdio.h>
#include <string.h> // memcpy

#define max(a, b) (a) > (b) ? (a) : (b)

static int nodosVisitados = 0;
static int pasosCriterio = 0;
static int pasosGenerar = 0;
static int pasosSolucion = 0;
static int pasosMasHermanos = 0;
static int pasosRetroceder = 0;

static int
Criterio(int nivel, int *s)
{
    ++pasosCriterio;
    // pintf("Criterio ");
    for (int i = 0; i < nivel; ++i)
        if (s[nivel] == s[i])
        {
            // printf("NO\n");
            return 0;
        }
    // printf("SI\n");
    return 1;
}

static int
MasHermanos(int nivel, int *s)
{
    ++pasosMasHermanos;
    // printf("MasHermanos? (nivel: %d) %d\n",nivel, s[nivel] < N - 1);
    return s[nivel] < N - 1;
}

static void
Retroceder(int *nivel, int *s, int *bact)
{
    ++pasosRetroceder;
    // printf("Retroceder\n");
    *bact = *bact - B[*nivel][s[*nivel]];
    s[*nivel] = -1;
    (*nivel)--;
}

static void
Generar(int nivel, int *s, int *bact)
{
    ++pasosGenerar;
    // printf("Generar [nivel: %d]\n", nivel);
    s[nivel]++;
    if (s[nivel] == 0)
        *bact = *bact + B[nivel][s[nivel]];
    else
        *bact = *bact + B[nivel][s[nivel]] - B[nivel][s[nivel] - 1];
}

static int
Solucion(int nivel, int *s)
{
    ++pasosSolucion;
    return (nivel == (N - 1) && Criterio(nivel, s));
}

static int
Criterio_usadas(int *s, int *usadas, int nivel)
{
    ++pasosCriterio;
    return usadas[s[nivel]] == 1;
}

static void
Retroceder_usadas(int *s, int *usadas, int *bact, int *nivel)
{
    ++pasosRetroceder;
    *bact = *bact - B[*nivel][s[*nivel]];
    usadas[s[*nivel]]--;
    (s[*nivel]) = -1;
    --(*nivel);
}

static void
Generar_usadas(int *s, int nivel, int *usadas, int *bact)
{
    ++pasosGenerar;
    if (s[nivel] != -1)
        --usadas[s[nivel]];

    s[nivel]++;
    usadas[s[nivel]]++;

    if (s[nivel] == 0)
        *bact = *bact + B[nivel][s[nivel]];
    else
        *bact = *bact + B[nivel][s[nivel]] - B[nivel][s[nivel] - 1];
}

void
Backtracking(int *s)
{
    int nivel = 0;
    int voa = -1;
    int bact = 0;
    int soa[N] = SINICIAL;

    memcpy(s, soa, sizeof(soa));

    ++nodosVisitados;
    do
    {
        Generar(nivel, s, &bact);
        if (Solucion(nivel, s) && (bact > voa))
        {
            voa = bact;
            memcpy(soa, s, sizeof(soa));
        }

        if (Criterio(nivel, s))
        {
            ++nodosVisitados;
            if (nivel < (N - 1))
                nivel++;
        }

        while (!MasHermanos(nivel, s) && (nivel >= 0))
        {
            Retroceder(&nivel, s, &bact);
        }

    } while (nivel != -1);

    memcpy(s, soa, sizeof(soa));
    printf("voa              = %d\n", voa);
    printf("nodosVisitados   = %d\n", nodosVisitados);
    printf("pasosCriterio    = %d\n", pasosCriterio);
    printf("pasosGenerar     = %d\n", pasosGenerar);
    printf("pasosSolucion    = %d\n", pasosSolucion);
    printf("pasosMasHermanos = %d\n", pasosMasHermanos);
    printf("pasosRetroceder  = %d\n", pasosRetroceder);

    nodosVisitados = 0;
    pasosCriterio = 0;
    pasosGenerar = 0;
    pasosSolucion = 0;
    pasosMasHermanos = 0;
    pasosRetroceder = 0;
}

void
Backtracking_usadas(int *s)
{
    int nivel = 0;
    int voa = -1;
    int bact = 0;
    int soa[N] = SINICIAL;
    int usadas[N] = { 0 };
    memcpy(s, soa, sizeof(soa));

    ++nodosVisitados;
    do
    {
        Generar_usadas(s, nivel, usadas, &bact);
        if (Solucion(nivel, s) && (bact > voa))
        {
            voa = bact;
            memcpy(soa, s, sizeof(soa));
        }

        if (Criterio_usadas(s, usadas, nivel))
        {
            ++nodosVisitados;
            if (nivel < (N - 1))
                nivel++;
        }

        while (!MasHermanos(nivel, s) && (nivel >= 0))
            Retroceder_usadas(s, usadas, &bact, &nivel);

    } while (nivel != -1);

    memcpy(s, soa, sizeof(soa));
    printf("voa              = %d\n", voa);
    printf("nodosVisitados   = %d\n", nodosVisitados);
    printf("pasosCriterio    = %d\n", pasosCriterio);
    printf("pasosGenerar     = %d\n", pasosGenerar);
    printf("pasosSolucion    = %d\n", pasosSolucion);
    printf("pasosMasHermanos = %d\n", pasosMasHermanos);
    printf("pasosRetroceder  = %d\n", pasosRetroceder);

    nodosVisitados = 0;
    pasosCriterio = 0;
    pasosGenerar = 0;
    pasosSolucion = 0;
    pasosMasHermanos = 0;
    pasosRetroceder = 0;
}
