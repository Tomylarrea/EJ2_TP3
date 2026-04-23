### Ejercicio 2

La estructura `var_t` representa una magnitud física utilizada por un sistema SCADA (control y monitoreo de datos) y permite almacenar un valor real, junto con un umbral alto de alarma, un umbral bajo de alarma, y una variable de estado que indica distintas condiciones asociadas a las alarmas.

Una librería compuesta por los archivos variable.h/.c permite crear variables de tipo `var_t` y manipularlas mediante una serie de funciones publicadas en el .h:

```c
#ifndef _VARIABLE_H
#define _VARIABLE_H

typedef enum {
    VAR_NORMAL,
    VAR_HI,
    VAR_LO,
    VAR_WAITING_ACK
} estado_t;

typedef struct {
    float valor;
    float umbral_alto;
    float umbral_bajo;
    estado_t estado;
} var_t;

/* inicializa el valor de la variable v pasada por referencia, los umbrales de alarma superior e inferior, e inicializa el estado interno */
void var_init(var_t *v, float val_ini, float umbral_hi_ini, float umbral_lo_ini);

/* setea el valor de la variable v pasada por referencia */
void var_set_val(var_t* v, float new_val);

/* reconoce la alarma y reinicia el estado de la variable (si la situación de alarma se extinguió) */
void var_ack_alarm(var_t* v);

/* obtiene el estado interno de la variable */
estado_t var_get_state(var_t* v);

#endif
```

El archivo variable.c contiene las definiciones de las funciones expuestas en el .h, junto con la definición de una función interna que ejecuta una FSM y una enumeración de eventos, que pueden ser VAR_VAL_EVT o VAR_ACK_EVT. Los estados son los definidos en el .h. La FSM cumple con las siguientes reglas:

a) Deberá dispararse un evento VAR_VAL_EVT cada vez que se cambia el valor principal de la variable con la función correspondiente. Si la variable está en estado VAR_NORMAL o VAR_WAITING_ACK se verificará si excede los límites pasando a los estados de alarma. Si el valor es menor al umbral bajo, la variable estará en estado VAR_LO, si es mayor al umbral alto estará en estado VAR_HI.
b) Si la variable está en alguno de los estados de alarma y vuelve a tener un valor en el rango normal pasará a VAR_WAITING_ACK.
c) Si se está en el estado VAR_WAITING_ACK y se dispara el evento VAR_ACK_EVT, se volverá al estado normal.
d) Si está en el estado de alarma VAR_LO (VAR_HI) y se actualiza el valor a uno mayor (menor) al umbral alto (bajo) se cambia al otro estado de alarma VAR_HI (VAR_LO).
e) En los estados de alarma se ignora el VAR_ACK_EVT.

**Consignas:**

1. Dibuje el esquema de la FSM mencionada, identificando correctamente cada elemento.
2. Implemente el archivo variable.c con los elementos mencionados anteriormente.
3. Realice un programa de prueba en el cual llamen secuencialmente a las funciones correspondientes para hacer pasar la máquina por todos los estados, verificando el resultado correcto en una sesión de debug.
4. Implemente un sistema que utilice la librería creada y agregue la capacidad de imprimir por UART mensajes, setear valor de la variables con el ADC, y utilizar un pulsador para reconocer la alarma. Respete la siguiente funcionalidad con una estructura time triggered con tres tareas:
    * Una tarea toma una muestra del ADC y setea el valor de la variable con la función correspondiente cada 100 ms.
    * Otra, que se ejecuta cada 1 segundo, imprime mensajes por UART según el estado en el que esté la variable (dicho estado se obtiene con la función correspondiente):
        i. En estado normal: se verificará si el valor cambió en más de un 5 % respecto al anterior, solo en ese caso se imprimirá una línea con el valor de la variable.
        ii. En estado de alarma se imprimirá el valor actual y la leyenda "ALARMA ALTA" o "ALARMA BAJA" según corresponda.
        iii. En estado de espera del acknowledge se imprimirá el valor actual y la leyenda "RECONOCIMIENTO DE ALARMA PENDIENTE".
    * Otra lee el estado de un pulsador cada 20 ms, cuando se presiona envía el reconocimiento de alarma mediante la función correspondiente.

Como ayuda proponemos el siguiente esquema:

```c
var_t v;
var_init(&v, 0, 3000, 1000);

while (1) {
    if(HAL_GetTick() - lastTick1 >= periodo1) {
        lastTick1 = HAL_GetTick();
        tarea1(&v);
    }
    
    if(HAL_GetTick() - lastTick2 >= periodo2) {
        lastTick2 = HAL_GetTick();
        tarea2(&v);
    }
    
    if(HAL_GetTick() - lastTick3 >= periodo3) {
        lastTick3 = HAL_GetTick();
        tarea3(&v);
    }
}
```
