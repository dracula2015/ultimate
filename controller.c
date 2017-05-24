/*
 * File:   controller.c
 * Author: dracula
 *
 * Created on January 6, 2017, 11:58 PM
 */

//#include "xc.h"
#include "user.h"
extern Parameter P;
extern Matrix *Kp;
extern Matrix *Kd;
Vector3f *OMRS_controller(Vector3f *qd, Vector3f *dqd, Vector3f *ddqd, Vector3f *q, Vector3f *dq)
{
    Vector3f *temp;
    Vector3f *uavc;
    Matrix *Ravc = m_constructor(NULL, NULL, cos(q->z), -sin(q->z), 0, sin(q->z), cos(q->z), 0, 0, 0 ,1);
    Matrix *DRavc = m_constructor(NULL, NULL, -sin(q->z)*dq->z, -cos(q->z)*dq->z, 0, cos(q->z)*dq->z, -sin(q->z)*dq->z, 0, 0, 0 ,0);
    Matrix *M2avc = m_constructor(NULL, NULL, 1.5*P.beta0 + P.m, 0, 0, 0, 1.5*P.beta0 + P.m, 0, 0, 0, 3*P.beta0*pow(P.La, 2) + P.Iv);
    Matrix *C2avc = m_constructor(NULL, NULL, 1.5*P.beta1, -P.m*dq->z, 0, P.m*dq->z, 1.5*P.beta1, 0, 0, 0, 3*P.beta1*pow(P.La, 2));
    Matrix *RavcRev = m_constructor(NULL, NULL, cos(q->z), sin(q->z), 0, -sin(q->z), cos(q->z), 0, 0, 0, 1);
    Matrix *Mavc = m_m_multiply(M2avc,RavcRev);
    Matrix *Cavc = m_minus(m_m_multiply(C2avc,RavcRev),m_m_multiply(M2avc,m_m_multiply(RavcRev,m_m_multiply(DRavc,RavcRev)))); 
    Matrix *BavcOri = m_constructor(NULL, NULL, -0.5, -0.5, 1, 0.866, -0.866, 0, P.La, P.La, P.La);
    Matrix *Bavc = m_s_multiply(BavcOri,P.beta2);
    temp = m_v_multiply(m_inverse(Bavc),m_v_multiply(Mavc,v_minus(ddqd,v_plus(m_v_multiply(Kd,v_minus(dq,dqd)),m_v_multiply(Kp,v_minus(q,qd))))));
    uavc = v_plus(temp,m_v_multiply(m_m_multiply(m_inverse(Bavc),Cavc),dq));
    /*
    m_destructor(Ravc,1);
    m_destructor(DRavc,1); 
    m_destructor(M2avc,1);
    m_destructor(C2avc,1);
    m_destructor(RavcRev,1);
    m_destructor(Mavc,1);
    m_destructor(Cavc,1);
    m_destructor(BavcOri,1);
    m_destructor(Bavc,1);
    */
    return uavc;
}