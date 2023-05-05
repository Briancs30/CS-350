/* THIS CODE WAS MY OWN WORK , IT WAS WRITTEN WITHOUT CONSULTING ANY
SOURCES OUTSIDE OF THOSE APPROVED BY THE INSTRUCTOR. _Brian Hsu_ */
#include "pbm.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<getopt.h>
int multioptcheck(int repeatedoptcheck);



int main( int argc, char *argv[] )
{
  int c;
  int repeatedoptcheck = 0;
  long grayscaleval = 0; //[1-65535]
  char *strtolptr; //save nonint char from strol()
  int scalefactor = 0; //n scale [1-8]
  int flag_b = 1, flag_g = 0, flag_i = 0, flag_r = 0, flag_s = 0, flag_m = 0, flag_t = 0, flag_n = 0, flag_o = 0;//default = b;
  char *g_opt = (char*)malloc(sizeof(char)*100);//save obj from getopt()
  char *i_opt = (char*)malloc(sizeof(char)*100);
  char *r_opt = (char*)malloc(sizeof(char)*100);
  char *t_opt = (char*)malloc(sizeof(char)*100);
  char *n_opt = (char*)malloc(sizeof(char)*100);
  char *o_opt = (char*)malloc(sizeof(char)*100);
  
  //Getopt() and multiple transformation error check
  while((c = getopt(argc, argv, ":bg:i:r:smt:n:o:"))!= -1)
  {
    switch (c)
    {
      case 'b':
        repeatedoptcheck = multioptcheck(repeatedoptcheck);
        flag_b = 1;
        break;
      
      case 'g':
        repeatedoptcheck = multioptcheck(repeatedoptcheck);
        flag_g = 1;
        flag_b = 0;
        strcpy(g_opt, optarg);
        break;
      
      case 'i':
        repeatedoptcheck = multioptcheck(repeatedoptcheck);
        flag_i = 1;
        flag_b = 0;
        strcpy(i_opt, optarg);
        break;
        
      case 'r':
        repeatedoptcheck = multioptcheck(repeatedoptcheck);
        flag_r = 1;
        flag_b = 0;
        strcpy(r_opt, optarg);
        break;
      
      case 's':
        repeatedoptcheck = multioptcheck(repeatedoptcheck);
        flag_s = 1;
        flag_b = 0;
        break;
      
      case 'm':
        repeatedoptcheck = multioptcheck(repeatedoptcheck);
        flag_m = 1;
        flag_b = 0;
        break;
      
      case 't':
        repeatedoptcheck = multioptcheck(repeatedoptcheck);
        flag_t = 1;
        flag_b = 0;
        strcpy(t_opt, optarg);
        break;
      
      case 'n':
        repeatedoptcheck = multioptcheck(repeatedoptcheck);
        flag_n = 1;
        flag_b = 0;
        strcpy(n_opt, optarg);
        flag_b = 0;
        break;
      
      case 'o':
        flag_o = 1;
        strcpy(o_opt, optarg);
        if((flag_g + flag_i + flag_r + flag_s + flag_m + flag_t + flag_n + flag_o) == 1)
          flag_b = 1;
        else
          flag_b = 0;
        
        break;
      
      case '?':
        fprintf(stderr, "Usage: ppmcvt [-bgirsmtno] [FILE]\n");
        exit(1);
        break;
        
    
    }
  }
  //Error Check
  if(flag_g){
    grayscaleval = strtol(g_opt, &strtolptr , 10);
    if(grayscaleval < 1 || grayscaleval > 65536){
      fprintf(stderr, "Error: Invalid max grayscale pixel value: %s; must be less than 65,536\n", g_opt);
      exit(1);
    }
  }
  if(flag_i){
    if((strcmp(i_opt,"red") && strcmp(i_opt,"blue") && strcmp(i_opt,"green")) || i_opt == NULL){
      fprintf(stderr, "Error: Invalid channel specification: (%s); should be 'red', 'green', or 'blue'\n", i_opt);
      exit(1);
    }
  }
  if(flag_r){
    if((strcmp(r_opt,"red") && strcmp(r_opt,"blue") && strcmp(r_opt,"green")) || r_opt == NULL){
      fprintf(stderr, "Error: Invalid channel specification: (%s); should be 'red', 'green', or 'blue'\n", r_opt);
      exit(1);
    }
  }
  if(flag_t){
    scalefactor = strtol(t_opt, &strtolptr , 10);
    if(scalefactor < 1 || scalefactor > 8){
      fprintf(stderr, "Error: Invalid scale factor: %d; must be 1-8\n", scalefactor);
      exit(1);
    }
  }
  if(flag_n){
    scalefactor = strtol(n_opt, &strtolptr , 10);
    if(scalefactor < 1 || scalefactor > 8){
      fprintf(stderr, "Error: Invalid scale factor: %d; must be 1-8\n", scalefactor);
      exit(1);
    }
  }
  
  
  
  char* inputfile = NULL;
  char* outputfile = o_opt;
  //Error Check - No input/Output files
  if(optind == argc -1)
    inputfile = argv[optind];
  else
  {
    fprintf(stderr, "Error: No input file specified\n");
    exit(1);
  }
  if(o_opt == NULL){
    fprintf(stderr, "Error: No output file specified\n");
    exit(1);
  }
  
  //Read input file for transformation
  PPMImage *readppm = read_ppmfile(inputfile);
  
  //Bitmap Conversion
  if(flag_b){
    PBMImage *bitmap = new_pbmimage(readppm -> width, readppm -> height);
    
    for(int w = 0; w < readppm-> width; w++){//loop and implement the bitmap formula
      for(int h = 0; h < readppm-> height; h++){
        if(((float)(readppm -> pixmap[0][h][w] + readppm -> pixmap[1][h][w] + readppm -> pixmap[2][h][w])/3) < ((float)readppm -> max /2))
          bitmap -> pixmap[h][w] = 1;
        else
          bitmap -> pixmap[h][w] = 0;
      }
    }
    
    write_pbmfile(bitmap, outputfile);
    del_pbmimage(bitmap);
  }
  
  //GrayScale Conversion -Done
  if(flag_g){
    PGMImage *grayscale = new_pgmimage(readppm -> width, readppm -> height, grayscaleval);
    
    for(int w = 0; w < readppm-> width; w++){//loop and implement the grayscale formula
      for(int h = 0; h < readppm-> height; h++){
        grayscale -> pixmap[h][w] = (((float)(readppm -> pixmap[0][h][w] + readppm -> pixmap[1][h][w] + readppm -> pixmap[2][h][w])/3) / ((float)readppm -> max)) * (float)grayscale -> max;
      }
    }
    
    write_pgmfile(grayscale, outputfile);
    del_pgmimage(grayscale);
  }
  
  //Isolate
  int color = 0;
  if(flag_i){
    PPMImage *isolate = new_ppmimage(readppm -> width, readppm -> height, readppm -> max);
    if(strcmp(i_opt, "red") == 0)//match input color for isolation
      color = 0;
    else if(strcmp(i_opt, "green") == 0)
      color = 1;
    else
      color = 2;
    
    for(int w = 0; w < readppm-> width; w++){
      for(int h = 0; h < readppm-> height; h++){
        isolate -> pixmap[0][h][w] = 0;
        isolate -> pixmap[1][h][w] = 0;
        isolate -> pixmap[2][h][w] = 0;
        isolate -> pixmap[color][h][w] = readppm -> pixmap[color][h][w]; // keep isolated color
      }
    }
    
    write_ppmfile(isolate, outputfile);
    
    del_ppmimage(isolate);
  }
  
  //Remove
  if(flag_r){    
    PPMImage *remove = new_ppmimage(readppm -> width, readppm -> height, readppm -> max);
    if(strcmp(r_opt, "red") == 0)//match input color for removal
      color = 0;
    else if(strcmp(r_opt, "green") == 0)
      color = 1;
    else
      color = 2;
    
    for(int w = 0; w < readppm-> width; w++){
      for(int h = 0; h < readppm-> height; h++){
        remove -> pixmap[0][h][w] = readppm -> pixmap[0][h][w];
        remove -> pixmap[1][h][w] = readppm -> pixmap[1][h][w];
        remove -> pixmap[2][h][w] = readppm -> pixmap[2][h][w];
        remove -> pixmap[color][h][w] = 0; // remove input color
      }
    }
    
    write_ppmfile(remove, outputfile);
    del_ppmimage(remove);
    
  }
  
  //Sepia - assign new color value
  if(flag_s){
    PPMImage *sepia = new_ppmimage(readppm -> width, readppm -> height, readppm -> max);
    
    
    for(int w = 0; w < readppm-> width; w++){
      for(int h = 0; h < readppm-> height; h++){
        sepia -> pixmap[0][h][w] = (0.393 * (float)(readppm -> pixmap[0][h][w])) + (0.769 * (float)(readppm -> pixmap[1][h][w])) + (0.189 * (float)(readppm -> pixmap[2][h][w]));
        sepia -> pixmap[1][h][w] = (0.349 * (float)(readppm -> pixmap[0][h][w])) + (0.686 * (float)(readppm -> pixmap[1][h][w])) + (0.168 * (float)(readppm -> pixmap[2][h][w]));
        sepia -> pixmap[2][h][w] = (0.272 * (float)(readppm -> pixmap[0][h][w])) + (0.534 * (float)(readppm -> pixmap[1][h][w])) + (0.131 * (float)(readppm -> pixmap[2][h][w]));
        
        if(sepia -> pixmap[0][h][w] > sepia -> max)//ensure num stay below max val
          sepia -> pixmap[0][h][w] = sepia -> max;
        if(sepia -> pixmap[1][h][w] > sepia -> max)
          sepia -> pixmap[1][h][w] = sepia -> max;
        if(sepia -> pixmap[2][h][w] > sepia -> max)
          sepia -> pixmap[2][h][w] = sepia -> max;
      }
    }
    
    write_ppmfile(sepia, outputfile);
    del_ppmimage(sepia);
  }
  
  //Mirror
  if(flag_m){
    PPMImage *mirror = new_ppmimage(readppm -> width, readppm -> height, readppm -> max);
    
    for(int w = 0; w < readppm-> width/2; w++){
      for(int h = 0; h < readppm-> height; h++){//Begin mirror from start and end, stop until reach mid
        mirror -> pixmap[0][h][w] = readppm -> pixmap[0][h][w];//Begin
        mirror -> pixmap[1][h][w] = readppm -> pixmap[1][h][w];
        mirror -> pixmap[2][h][w] = readppm -> pixmap[2][h][w];
        
        mirror -> pixmap[0][h][readppm -> width - w] = readppm -> pixmap[0][h][w];//End
        mirror -> pixmap[1][h][readppm -> width - w] = readppm -> pixmap[1][h][w];
        mirror -> pixmap[2][h][readppm -> width - w] = readppm -> pixmap[2][h][w];
      }
    }
    
    write_ppmfile(mirror, outputfile);
    del_ppmimage(mirror);
  }
  
  //Thumbnail - 1/n scale
  if(flag_t){
    float newwidth = readppm -> width / scalefactor;
    float newheight = readppm -> height / scalefactor;
    if(readppm -> width % scalefactor ==0 && readppm -> height / scalefactor ==0){

    }else if(readppm -> width % scalefactor ==0 && readppm -> height / scalefactor !=0){
      newheight++;
    }else if(readppm -> width % scalefactor !=0 && readppm -> height / scalefactor ==0){
      newwidth++;
    }else{
      newwidth++;
      newheight++;
    }
    PPMImage *thumbnail = new_ppmimage(newwidth, newheight, readppm -> max);
    
    for(int w = 0; w < readppm-> width / scalefactor; w++){
      for(int h = 0; h < readppm-> height / scalefactor; h++){
        thumbnail -> pixmap[0][h][w] = readppm -> pixmap[0][h * scalefactor][w * scalefactor];
        thumbnail -> pixmap[1][h][w] = readppm -> pixmap[1][h * scalefactor][w * scalefactor];
        thumbnail -> pixmap[2][h][w] = readppm -> pixmap[2][h * scalefactor][w * scalefactor];
      }
    }
    
    write_ppmfile(thumbnail, outputfile);
    del_ppmimage(thumbnail);
  }
  
  //Nup
  if(flag_n){
    PPMImage *nup = new_ppmimage(readppm -> width, readppm -> height, readppm -> max);
    
    for(int w = 0; w < readppm-> width / scalefactor; w++){
      for(int h = 0; h < readppm-> height / scalefactor; h++){
        for(int movew = 0; movew < scalefactor; movew++){//move width and height to get mutilple copies
          for(int moveh = 0; moveh < scalefactor; moveh++){
            nup -> pixmap[0][moveh * (readppm-> height / scalefactor) + h][movew * (readppm-> width / scalefactor) + w] = readppm -> pixmap[0][h * scalefactor][w * scalefactor];
            nup -> pixmap[1][moveh * (readppm-> height / scalefactor) + h][movew * (readppm-> width / scalefactor) + w] = readppm -> pixmap[1][h * scalefactor][w * scalefactor];
            nup -> pixmap[2][moveh * (readppm-> height / scalefactor) + h][movew * (readppm-> width / scalefactor) + w] = readppm -> pixmap[2][h * scalefactor][w * scalefactor];
          }
        }
      }
    }
    write_ppmfile(nup, outputfile);
    del_ppmimage(nup);
  }

  free(g_opt);
  free(i_opt);
  free(r_opt);
  free(t_opt);
  free(n_opt);
  free(o_opt);
  
  
  del_ppmimage(readppm);
  return 0;
}

int multioptcheck(int repeatedoptcheck){
  if(repeatedoptcheck){
    fprintf(stderr, "Error: Multiple transformations specified\n");
    exit(1);
  }
  repeatedoptcheck = 1;
  return repeatedoptcheck;
}



