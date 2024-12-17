#ifndef FUNCIONES_H
#define FUNCIONES_H

#include "lista.h"


// clang-format off

#if N == 3
static int B[N][N] = {
//    { 11, 17,  8 },
//    {  9,  7,  6 },
//    { 13, 15, 16 }
  {  4,  9,  1 },
  {  7,  2,  3 },
  {  6,  3,  5 }
};

#elif N == 6

constexpr int B[N][N] = {
    { 11, 17,  8, 16, 20, 14 },
    {  9,  7,  6, 12, 15, 18 },
    { 13, 15, 16, 12, 16, 18 },
    { 21, 24, 28, 17, 26, 20 },
    { 10, 14, 12, 11, 15, 13 },
    { 12, 20, 19, 13, 22, 17 },
};
#endif

// clang-format on

#if N == 3
#define SINICIAL { -1, -1, -1 }

#elif N == 6
#define SINICIAL { -1, -1, -1, -1, -1, -1 }

#endif


void Backtracking(int *s);
void Backtracking_usadas(int *s);

void AsignacionPrecisa(NODO *s);
void AsignacionTrivial(NODO *s);


#endif // !FUNCIONES_H
