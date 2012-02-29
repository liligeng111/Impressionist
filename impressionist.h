/*
Impressionist, Project 1 of COMP4411
Based on some ugly skeleton codes

Collaborators:
WANG Heng
hwangad@ust.hk

LI Ligeng
lliaa@ust.hk

Events:
Feb - 9:
Repository created, try to add some comments to 
help understand the codes.

*/
#ifndef IMPRESSIONIST_H
#define IMPRESSIONIST_H
#define SAMPLE_SOLUTION
#include <stdio.h>

#include <FL/Fl.H>
#include <FL/gl.h>
#include <GL/glu.h>

#include "impbrush.h"

#define TRUE	1
#define FALSE	0

float frand();
int irand(int);

// #ifdef WIN32
// this is to make it be able to compile in linux
#define M_PI	3.1415926535F
#define NUMBER_OF_EDGES_ 20
// #endif


#endif
