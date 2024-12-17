#include "funciones.h"
#include "lista.h"
#include <stdio.h>
#include <string.h> // memcpy

#define max(a, b) ((a) > (b) ? (a) : (b))

static int nodosVisitados;

static void
BE(NODO *x) // beneficio estimado trivial-precisa
{
    x->BE = (x->CI + x->CS) / 2.0;
}

static void
CI_trivial(NODO *x) // cota inferior estimación trivial (D75)
{
    x->CI = x->bact;
}

static void
CS_trivial(NODO *x) // cota superior estimación trivial (D75)
{
    static int max = -1;

    /* Siembre va a ser el mismo numero no hace
     * falta calcularlo de cada vez */
    if (max == -1)

        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                if (B[i][j] > max)
                    max = B[i][j];

    x->CS = x->bact + (N - x->nivel - 1) * max;
}

// elimina de la lista
// Funciones privadas necesarias para CI_precisa() y CS_precisa()
static int
_AsignacionVoraz(NODO x) // Devuelve valor asignación voraz (D76)
{
    int bacum = 0;
    int k;
    int maxbeneficio;

    for (int i = x.nivel + 1; i < N; ++i)
    {
        k = maxbeneficio = -1;

        for (int j = 0; j < N; ++j)
            if (!x.usadas[j] && B[i][j] > maxbeneficio)
            {
                maxbeneficio = B[i][j];
                k = j;
            }

        if (k != -1)
        {
            x.usadas[k] = 1;
            bacum += B[i][k];
        }
    }
    return bacum;
}

static void
CI_precisa(NODO *x) // cota inferior estimación precisa (D76)
{
    x->CI = x->bact + _AsignacionVoraz(*x);
}

static int
_MaximosTareas(NODO x) // Devuelve valor máximos ataques (D77)
{
    int bacum = 0;
    int k;
    int maxbeneficio;
    for (int i = x.nivel + 1; i < N; ++i)
    {
        k = -1;
        maxbeneficio = -1;
        for (int j = 0; j < N; ++j)
            if (!x.usadas[j] && B[i][j] > maxbeneficio)
            {
                maxbeneficio = B[i][j];
                k = j;
            }

        if (k != -1)
            bacum += B[i][k];
    }
    return bacum;
}

static void
CS_precisa(NODO *x) // cota superior estimación precisa (D77)
{
    x->CS = x->bact + _MaximosTareas(*x);
}

static int
Solucion(NODO x) // determina si x es solución (D74)
{
    return x.nivel == N - 1;
}

static NODO
Seleccionar(TLISTA *LNV) // Devuelve nodo según estrategia MB-LIFO y lo
{
    TPOSICION curPos;
    TPOSICION mayorBEPos;
    NODO curNodo;
    NODO mayorBENodo;

    curPos = primeroLista(*LNV);
    mayorBEPos = curPos;
    recuperarElementoLista(*LNV, mayorBEPos, &mayorBENodo);

    while (curPos != finLista(*LNV))
    {
        recuperarElementoLista(*LNV, curPos, &curNodo);
        if (curNodo.BE > mayorBENodo.BE)
        {
            mayorBENodo = curNodo;
            mayorBEPos = curPos;
        }
        curPos = siguienteLista(*LNV, curPos);
    }
    suprimirElementoLista(LNV, mayorBEPos);

    return mayorBENodo;
}


void
AsignacionTrivial(NODO *s)
{
    TLISTA LNV;
    NODO raiz, x, y;
    float C = 0;

    nodosVisitados = 0;
    raiz.n = ++nodosVisitados;
    raiz.bact = 0;
    raiz.nivel = -1;

    memset(raiz.usadas, 0, sizeof raiz.usadas);
    memcpy(raiz.tupla, (int[]) SINICIAL, sizeof raiz.tupla);

    CI_trivial(&raiz);
    CS_trivial(&raiz);
    BE(&raiz);

    C = raiz.CI;
    crearLista(&LNV);
    insertarElementoLista(&LNV, primeroLista(LNV), raiz);

    while (!esListaVacia(LNV))
    {
        x = Seleccionar(&LNV);
        if (x.CS > C)
        {
            for (int i = 0; i < N; ++i)
            {
                y.nivel = x.nivel + 1;
                memcpy(y.tupla, x.tupla, sizeof x.tupla);
                memcpy(y.usadas, x.usadas, sizeof x.usadas);


                if (!x.usadas[i])
                {
                    y.tupla[y.nivel] = i;
                    y.usadas[i] = 1;
                    y.bact = x.bact + B[y.nivel][i];
                    CI_trivial(&y);
                    CS_trivial(&y);
                    BE(&y);
                    y.n = ++nodosVisitados;

                    if (Solucion(y) && y.bact > s->bact)
                    {
                        *s = y;
                        C = max(C, (float) y.bact);
                    }

                    else if (!Solucion(y) && y.CS > C)
                    {
                        // inserto por el principio (LIFO)
                        insertarElementoLista(&LNV, primeroLista(LNV), y);
                        // inserto por el final (FIFO)
                        // insertarElementoLista(&LNV, finLista(LNV), y);
                        C = max(C, y.CI);
                    }
                }
            }
        }
    }

    printf("Nodos visitados: %d\n", nodosVisitados);
    printf("Valor optimo: %f\n", C);

    destruirLista(&LNV);
}

static NODO
_SolAsignacionVoraz(NODO x)
{
    int Bmax;
    int tmax;

    for (int i = x.nivel + 1; i < N; ++i)
    {
        Bmax = -1;
        for (int j = 0; j < N; ++j)

            if (!x.usadas[j] && B[i][j] > Bmax)
            {
                Bmax = B[i][j];
                tmax = j;
            }

        x.usadas[tmax] = 1;
        x.tupla[i] = tmax;
        x.bact += Bmax;
        x.n = ++nodosVisitados;
    }

    x.nivel = N - 1;
    return x;
}

void
AsignacionPrecisa(NODO *s)
{
    TLISTA LNV;
    NODO raiz, x, y;
    float C = 0;

    nodosVisitados = 0;
    raiz.n = ++nodosVisitados;
    raiz.bact = 0;
    raiz.nivel = -1;

    memset(raiz.usadas, 0, sizeof raiz.usadas);
    memcpy(raiz.tupla, (int[]) SINICIAL, sizeof raiz.tupla);

    CI_precisa(&raiz);
    CS_precisa(&raiz);
    BE(&raiz);

    C = raiz.CI;
    crearLista(&LNV);
    insertarElementoLista(&LNV, primeroLista(LNV), raiz);

    while (!esListaVacia(LNV))
    {
        x = Seleccionar(&LNV);
        if (x.CS > C)
        {
            for (int i = 0; i < N; ++i)
            {
                y.nivel = x.nivel + 1;
                memcpy(y.tupla, x.tupla, sizeof x.tupla);
                memcpy(y.usadas, x.usadas, sizeof x.usadas);

                if (!x.usadas[i])
                {
                    y.tupla[y.nivel] = i;
                    y.usadas[i] = 1;
                    y.bact = x.bact + B[y.nivel][i];
                    CI_trivial(&y);
                    CS_trivial(&y);
                    BE(&y);
                    y.n = ++nodosVisitados;

                    if (!Solucion(y) && y.CS >= C && y.CS == y.CI)
                    {
                        y = _SolAsignacionVoraz(y);
                        *s = y;
                        C = max(C, y.bact);
                        continue;
                    }

                    if (Solucion(y) && y.bact > s->bact)
                    {
                        *s = y;
                        C = max(C, y.bact);
                    }

                    else if (!Solucion(y) && y.CS > C)
                    {
                        insertarElementoLista(&LNV, primeroLista(LNV), y);
                        C = max(C, y.CI);
                    }
                }
            }
        }

        else if (x.CS == C && x.CS == x.CI)
        {
            *s = _SolAsignacionVoraz(x);
        }
    }

    printf("Nodos visitados: %d\n", nodosVisitados);
    printf("Valor optimo: %f\n", C);

    destruirLista(&LNV);
}
