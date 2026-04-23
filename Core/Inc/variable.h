/*
 * variable.h
 *
 *  Created on: 23 abr 2026
 *      Author: tomyl
 */

#ifndef INC_VARIABLE_H_
#define INC_VARIABLE_H_

#include <stdint.h>

/* States */
typedef enum {
    VAR_NORMAL,
    VAR_LO,
    VAR_HI,
    VAR_WAITING_ACK
} estado_t;

/* Events */
typedef enum {
    VAR_VAL_EVT,
    VAR_ACK_EVT
} evento_t;


typedef struct{
	float valor;
	float umbral_alto;
	float umbral_bajo;
	estado_t estado;
}var_t;



void var_init(var_t *v, float val_ini, float umbral_hi_ini, float umbral_lo_ini);
void var_set_val(var_t* v, float new_val);
void var_ack_alarm(var_t* v);
estado_t var_get_state(var_t* v);


#endif /* INC_VARIABLE_H_ */
