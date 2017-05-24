/* 
 * File:   vector.h
 * Author: dracula
 * Comments:
 * Revision history: EDITION 1.0
 */

// This is a guard condition so that contents of this file are not included more than once.  
#ifndef XC_HEADER_VECTOR_H
#define	XC_HEADER_VECTOR_H

//#include <xc.h> // include processor files - each processor file is guarded.  

// TODO Insert appropriate #include <>

// TODO Insert C++ class definitions if appropriate

//object oriented c

typedef struct Vector3f
{
    float x,y,z;
    
    struct Vector3f *(*v_constructor)(struct Vector3f*v, float x, float y, float z);

    void (*v_destructor)(struct Vector3f*v, bool dynamic);

    struct Vector3f *(*v_plus)(struct Vector3f*v,struct Vector3f*w);

    struct Vector3f *(*v_minus)(struct Vector3f*v,struct Vector3f*w);

    float (*v_v_multiply)(struct Vector3f*v,struct Vector3f*w);

    struct Vector3f *(*v_s_multiply)(struct Vector3f*v,float s);

    struct Vector3f *(*v_equal)(struct Vector3f*v,struct Vector3f*w);

    float (*v_length)(struct Vector3f*v);

    void (*v_normalize)(struct Vector3f*v);
}Vector3f;

// TODO Insert declarations
Vector3f *v_constructor(Vector3f*v, float x, float y, float z);

void v_destructor(Vector3f*v, bool dynamic);

Vector3f *v_plus(Vector3f*v,Vector3f*w);

Vector3f *v_minus(Vector3f*v,Vector3f*w);

float v_v_multiply(Vector3f*v,Vector3f*w);

Vector3f *v_s_multiply(Vector3f*v,float s);

Vector3f *v_equal(Vector3f*v,Vector3f*w);

float v_length(Vector3f*v);

void v_normalize(Vector3f*v);

// Comment a function and leverage automatic documentation with slash star star

// TODO Insert declarations or function prototypes (right here) to leverage live documentation
#endif
