#include <stdio.h>
#include "funciones.h"
#include "lista.h"

int contadorNodos=0;


int max(int num1, int num2) {
    return (num1 > num2) ? num1 : num2;
}


void BE(NODO *x) {
    x->BE = (x->CI + x->CS) / 2.0;

}


int AsignacionVoraz(NODO x) {
    int bacum = 0;

    for (int i = x.nivel + 1; i < N; i++) {
        int k = -1;
        int maxBeneficio = -1;

        for (int j = 0; j < N; j++) {
            if (!x.usadas[j] && B[i][j] > maxBeneficio) {
                maxBeneficio = B[i][j];
                k = j;
            }
        }

        if (k != -1) {
            x.usadas[k] = 1;
            bacum += B[i][k];
        }
    }

    return bacum;
}


void CI_precisa(NODO *x, int B[][N]) {
    x->CI = x->bact + AsignacionVoraz(*x);
}

int MaximosTareas(NODO x, int B[][N]) {
    int bacum = 0;

    for (int i = x.nivel + 1; i < N; i++) {
        int k = -1;
        int maxBeneficio = -1;

        for (int j = 0; j < N; j++) {
            if (!x.usadas[j] &&  B[i][j] > maxBeneficio) {
                maxBeneficio = B[i][j];
                k = j;
            }
        }

        if (k != -1) {
            bacum += B[i][k];
        }
    }

    return bacum;
}



void CS_precisa(NODO *x, int B[][N]) {
    x->CS = x->bact + MaximosTareas(*x, B);
}


int Solucion(NODO x) {
    return x.nivel == N - 1;
}

NODO Seleccionar(TLISTA LNV) {
    TPOSICION posActual, posMayorBE;
    NODO nodoActual, nodoMayorBE;

    posActual = primeroLista(LNV);
    posMayorBE = posActual;
    recuperarElementoLista(LNV, posActual, &nodoMayorBE);


    while (posActual != finLista(LNV)) {
        recuperarElementoLista(LNV, posActual, &nodoActual);
        if (nodoActual.BE > nodoMayorBE.BE) {
            nodoMayorBE = nodoActual;
            posMayorBE = posActual;
        }
        posActual = siguienteLista(LNV, posActual);
    }


    suprimirElementoLista(&LNV, posMayorBE);


    return nodoMayorBE;
}



NODO SolAsignacionVoraz(NODO x, int B[][N]) {
    int Bmax;
    int tmax;

    for (int i = x.nivel + 1; i < N; i++) {
        Bmax = -1;
        tmax = 0;
        for (int j = 0; j < N; j++) {
            if (!x.usadas[j] && B[i][j] > Bmax) {
                Bmax = B[i][j];
                tmax = j;
            }
        }

        x.tupla[i] = tmax;
        x.usadas[tmax] = 1;
        x.bact = x.bact + Bmax;
        contadorNodos++;
    }



    x.nivel = N - 1;

    return x;
}


void AsignacionPrecisa(NODO *s) {
    TLISTA LNV; // Listade nodos vivos
    NODO raiz, x, y;//Nodo raiz, nodo seleccionado(x) y nodo hijo de x(y)
    float C = 0;//Variable de poda

    contadorNodos++;//cuento la raiz
    raiz.bact = 0;//no hay asiganciones, beneficio acumulado 0
    raiz.nivel = -1;//primer nivel del arbol
    for (int i = 0; i < N; i++) {
        raiz.tupla[i] = -1;//tupla de solucion sin ataques asignados
        raiz.usadas[i] = 0;//todos los ataques están sin usar
    }

    CI_precisa(&raiz, B);//cota ingerior de raiz
    CS_precisa(&raiz, B);//cota superior de raiz
    BE(&raiz);//beneficio estimado de raiz

    C=raiz.CI;
    crearLista(&LNV);//creo la lista de nodos vivos
    insertarElementoLista(&LNV, primeroLista(LNV), raiz);//inserto la raiz en la lista de nodos vivos

    while (!esListaVacia(LNV)) {
        x = Seleccionar(LNV);//selecciona x y lo elimina de la LNV
        if (x.CS > C) { //ramificamos: generamos cada hijo
            for (int i = 0; i < N; i++) { //para cada hijo y de x
                y.nivel = x.nivel + 1;
                for (int k = 0; k < N; k++) {
                    y.tupla[k] = x.tupla[k];
                    y.usadas[k] = x.usadas[k];
                }

                if (!x.usadas[i]) {//nodo valido
                    y.tupla[y.nivel] = i;//ciudad 'i' galeón 'nivel'
                    y.usadas[i] = 1;
                    y.bact = x.bact + B[y.nivel][i];

                    contadorNodos++;

                    CI_precisa(&y, B);
                    CS_precisa(&y, B);
                    BE(&y);


                    if(!Solucion(y) && y.CS>=C && y.CS==y.CI){//y es solución
                        y= SolAsignacionVoraz(y,B);
                        *s=y;
                        C=max(C,y.bact);
                        continue;//termine la búsqueda por esta rama, ya no analizao los demás hermanos
                    }

                    if (Solucion(y) && (y.bact > s->bact)) {
                        *s = y;
                        if(y.bact>C){
                            C=y.bact;
                        }
                    } else if (!Solucion(y) && y.CS > C) {
                        insertarElementoLista(&LNV, primeroLista(LNV), y);//inserto en la LNV
                        C = max(C, y.CI);
                    }
                }
            }
        }

        else if( x.CS==C && x.CS==x.CI){//nodo x seleccionado es solucion voraz
            *s = SolAsignacionVoraz(x,B);
        }
    }

    printf("Usando asignación precisa y tamaño %d\n",N);
    printf("Valor óptimo: %f\n", C);
    for (int i=0;i<N; i++){
        printf("%d ", (s->tupla[i]) + 1);
    }
    printf("Número de nodos explorados: %d\n", contadorNodos);
    printf("\n");

    contadorNodos=0;
    destruirLista(&LNV);
}

void CI_trivial(NODO *x, int B[][N]) {
    x->CI = x->bact;
}

// Función para calcular CS trivial
void CS_trivial(NODO *x, int B[][N]) {
    int max_val = 0;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (B[i][j] > max_val) {
                max_val = B[i][j];
            }
        }
    }


    x->CS = x->bact + (N - x->nivel - 1) * max_val;
}


void AsignacionTrivial(NODO *s) {
    TLISTA LNV;
    NODO raiz, x, y;
    float C = 0;

    contadorNodos++;
    raiz.bact = 0;
    raiz.nivel = -1;
    for (int i = 0; i < N; i++) {
        raiz.tupla[i] = -1;
        raiz.usadas[i] = 0;
    }

    CI_trivial(&raiz, B);
    CS_trivial(&raiz, B);
    BE(&raiz);

    C=raiz.CI;
    crearLista(&LNV);
    insertarElementoLista(&LNV, primeroLista(LNV), raiz);

    while (!esListaVacia(LNV)) {
        x = Seleccionar(LNV);//seleccionar x y lo elimina de la LNV
        if (x.CS > C) {//ramificamos: generamos cada hijo

            for (int i = 0; i < N; i++) {//para cada hijo y de x
                y.nivel = x.nivel + 1;


                for (int k = 0; k < N; k++) {
                    y.tupla[k] = x.tupla[k];
                    y.usadas[k] = x.usadas[k];
                }

                if (!x.usadas[i]) {//nodo valido
                    y.tupla[y.nivel] = i;//ciudad 'i' galeón 'nivel'
                    y.usadas[i] = 1;
                    y.bact = x.bact + B[y.nivel][i];

                    CI_trivial(&y, B);
                    CS_trivial(&y, B);
                    BE(&y);

                    contadorNodos++;

                    if (Solucion(y) && y.bact > s->bact) {
                        *s = y;
                        C = max(C, y.bact);
                    } else if (!Solucion(y) && y.CS > C) {
                        insertarElementoLista(&LNV, finLista(LNV), y);//inserto por el final(FIFO)
                        C = max(C, y.CI);
                    }
                }
            }
        }
    }

    printf("Usando asignación trivial y tamaño %d\n", N);
    printf("Valor óptimo: %f\n", C);
    for(int i=0;i<N;i++){
        printf("%d ", (s->tupla[i]) + 1);
    }
    printf("Número de nodos explorados: %d\n", contadorNodos);
    printf("\n");

    contadorNodos=0;
    destruirLista(&LNV);
}
