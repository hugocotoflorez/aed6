#include <stdio.h>
#include "RamificacionYPoda.h"

int main() {

    //Hay que modificar el N en lista.h
#if N==3
    int B[N][N]= {
//    { 11, 17,  8 },
//    {  9,  7,  6 },
//    { 13, 15, 16 }
  {  4,  9,  1 },
  {  7,  2,  3 },
  {  6,  3,  5 }
};

#else
    int B[N][N] = {
        {11, 17, 8, 16, 20, 14},
        {9, 7, 6, 12, 15, 18},
        {13, 15, 16, 12, 16, 18},
        {21, 24, 28, 17, 26, 20},
        {10, 14, 12, 11, 15, 13},
        {12, 20, 19, 13, 22, 17}
    };
#endif

    NODO s;
    s.bact = 0;
    s.nivel = -1;
    for (int i = 0; i < N; i++) {
        s.tupla[i] = -1;
        s.usadas[i] = 0;
    }

    asignacionTrivial(B, &s) ;

    asignacionPrecisa(B, &s);

    return 0;
}
