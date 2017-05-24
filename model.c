/*
 * File:   model.c
 * Author: dracula
 *
 * Created on January 6, 2017, 11:58 PM
 */

//#include "xc.h"
#include "user.h"
Vector3f OMRS_controller(Vector3f *qd, Vector3f *dqd, Vector3f *ddqd, Vector3f *q, Vector3f *dq)
{
    Vector3f DDQ;
    Matrix *Rav;
    Matrix *DRav;
    Matrix *M2av;
    Matrix *C2av;
    Matrix *RavRev;
    Matrix *Mav;
    Matrix *Cav;
    Matrix *BavOri;
    Matrix *Bav;
    m_constructor(Rav, NULL, cos(q->z), -sin(q->z), 0, sin(q->z), cos(q->z), 0, 0, 0 ,1);
    m_constructor(DRav, NULL, -sin(q->z)*dq->z, -cos(q->z)*dq->z, 0, cos(q->z)*dq->z, -sin(q->z)*dq->z, 0, 0, 0 ,0);
   
}