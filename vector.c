/*
 * File:   vector.c
 * Author: dracula
 *
 * Created on December 28, 2016, 10:26 PM
 */
//#include "xc.h"
#include "user.h"
extern int countVector;
extern Vector3f *pointerVector[];
Vector3f *v_constructor(Vector3f*v, float x, float y, float z)
{
    if(v == NULL)
    {
        pointerVector[countVector] = (Vector3f*)malloc(sizeof(Vector3f));
        v = pointerVector[countVector];
        countVector++;
    }
        /*
    Vector3f *w;
    if(v == NULL)
    {
        v = w;
    }
    */
    v->x = x;
    v->y = y;
    v->z = z;
    v->v_plus = v_plus;
    v->v_minus = v_minus;
    v->v_v_multiply = v_v_multiply;
    v->v_s_multiply = v_s_multiply;
    v->v_equal = v_equal;
    v->v_length = v_length;
    v->v_normalize = v_normalize;
    return v;
}

void v_destructor(Vector3f*v, bool dynamic)
{
    if(dynamic)
    {
        free(v);
    }
}

Vector3f *v_plus(Vector3f*v,Vector3f*w)
{
    //Vector3f *temp = NULL;
    return v_constructor(NULL, v->x + w->x, v->y + w->y, v->z + w->z);
}

Vector3f *v_minus(Vector3f*v,Vector3f*w)
{
    //Vector3f *temp = NULL;
    return v_constructor(NULL, v->x - w->x, v->y - w->y, v->z - w->z);
}

float v_v_multiply(Vector3f*v,Vector3f*w)
{
    return v->x * w->x + v->y * w->y + v->z * w->z;
}

Vector3f *v_s_multiply(Vector3f*v,float s)
{
    //Vector3f *temp = NULL;
    return v_constructor(NULL, v->x * s, v->y * s, v->z * s);
}

Vector3f *v_equal(Vector3f*v,Vector3f*w)
{
    v->x = w->x;
    v->y = w->y;
    v->z = w->z;
    return v;
}

float v_length(Vector3f*v)
{
    return sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
}

void v_normalize(Vector3f*v)
{
    float len = v_length(v);
    v->x /= len;
    v->y /= len;
    v->z /= len;
}
