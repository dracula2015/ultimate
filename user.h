/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */
#include <math.h>
#include "matrix.h"
#include "vector.h"
/* TODO Application specific user parameters used in user.c may go here */

/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

/* TODO User level functions prototypes (i.e. InitApp) go here */

void InitApp(void);         /* I/O and Peripheral Initialization */
void PwmInit(void);
void UartInit(void);
void motor_drive(void);
Vector3f *OMRS_controller(Vector3f *qd, Vector3f *dqd, Vector3f *ddqd, Vector3f *q, Vector3f *dq);
//Vector3f OMRS_model(Vector3f *u, Vector3f *q, Vector3f *dq);
typedef struct Parameter
{
    float m;
    float Iv;
    float r;
    float Din;
    float Dout;
    float La;
    float I0;
    float kt;
    float kb;
    float n;
    float b0;
    float Ra;
    float beta0;
    float beta1;
    float beta2;
}Parameter;