/* THIS CODE WAS MY OWN WORK , IT WAS WRITTEN WITHOUT CONSULTING ANY
SOURCES OUTSIDE OF THOSE APPROVED BY THE INSTRUCTOR. _Brian Hsu_ */
#include "pbm.h"
#include<stdio.h>
#include<stdlib.h>
PPMImage * new_ppmimage( unsigned int w, unsigned int h, unsigned int m )
{
  PPMImage *ppm = malloc(sizeof(PPMImage));
  ppm -> width = w;
  ppm -> height = h;
  ppm -> max = m;
  
  ppm -> pixmap[0] = malloc(sizeof(unsigned int *)*h);
  ppm -> pixmap[1] = malloc(sizeof(unsigned int *)*h);
  ppm -> pixmap[2] = malloc(sizeof(unsigned int *)*h);
  for(int i=0; i<h; i++){
    ppm -> pixmap[0][i] = malloc(sizeof(unsigned int *)*w);
    ppm -> pixmap[1][i] = malloc(sizeof(unsigned int *)*w);
    ppm -> pixmap[2][i] = malloc(sizeof(unsigned int *)*w);
  }
  return ppm;
}

PBMImage * new_pbmimage( unsigned int w, unsigned int h )
{
  PBMImage *pbm = malloc(sizeof(PBMImage));
  pbm -> width = w;
  pbm -> height = h;
  
  pbm -> pixmap = malloc(sizeof(unsigned int *)*h);
  for(int i=0; i<h; i++){
    pbm -> pixmap[i] = malloc(sizeof(unsigned int *)*w);
  }
  return pbm;
}

PGMImage * new_pgmimage( unsigned int w, unsigned int h, unsigned int m )
{
  PGMImage *pgm = malloc(sizeof(PGMImage));
  pgm -> width = w;
  pgm -> height = h;
  pgm -> max = m;
  
  pgm -> pixmap = malloc(sizeof(unsigned int *)*h);
  for(int i=0; i<h; i++){
    pgm -> pixmap[i] = malloc(sizeof(unsigned int *)*w);
  }
  return pgm;
}

void del_ppmimage( PPMImage * p )
{
  for(int i = 0; i < p -> height; i++){
    free(p -> pixmap[0][i]);
    free(p -> pixmap[1][i]);
    free(p -> pixmap[2][i]);
  }  
  free(p -> pixmap[0]);
  free(p -> pixmap[1]);
  free(p -> pixmap[2]);
  free(p);
}

void del_pgmimage( PGMImage * p )
{
  for(int i = 0; i < p -> height; i++){
    free(p -> pixmap[i]);
  }  
  free(p -> pixmap);
  free(p);
}

void del_pbmimage( PBMImage * p )
{
  for(int i = 0; i < p -> height; i++){
    free(p -> pixmap[i]);
  }  
  free(p -> pixmap);
  free(p);
}

