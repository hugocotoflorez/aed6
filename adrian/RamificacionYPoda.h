#ifndef RAMIFICACIONYPODA_H
#define RAMIFICACIONYPODA_H
#include "lista.h"


/*
 * Función que resuelve el problema de asignación
 * empleadno la técnica de ramificación y poda con asignación precisa.
 *
 * @param B[][N] matriz de costos
 * @param *s puntero al nodo solución
 */
void asignacionPrecisa(int B[][N], NODO *s);


/*
 * Función que resuelve el problema de asignación
 * empleando la técnica de ramificación y poda con asignación trivial.
 *
 * @param B[][N] matriz de costos
 * @param *s puntero al nodo solución
 */
void asignacionTrivial(int B[][N], NODO *s);

#endif // RAMIFICACIONYPODA_H
