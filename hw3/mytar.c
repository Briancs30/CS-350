/* THIS CODE WAS MY OWN WORK , IT WAS WRITTEN WITHOUT CONSULTING ANY
SOURCES OUTSIDE OF THOSE APPROVED BY THE INSTRUCTOR. _Brian Hsu_ */

#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/time.h>

#include "inodemap.c"

void multNodesError(int count){
  if(count > 1){
    fprintf(stderr, "Error: Multiple modes specified.\n");
    exit(-1);
  }
}

void closeFile(FILE* ffile){
  if(fclose(ffile) != 0) {//close file
    perror("fclose");
    exit(-1);
  }
}

int checkmagicnum(FILE* ffile, int magic){//check and save magicnum
  if(!fread(&magic, 4, 1, ffile)){
    perror("fread");
    exit(-1);
  }
  if(magic != 0x7261746D){
    fprintf(stderr, "Error: Bad magic number (%d), should be: %d.\n", magic, 0x7261746D);
    exit(-1);
  }
  return magic;
}

void findAllFiles(FILE *file, char *directory){//loop and find all files/directories/hardlinks
  struct stat buf;
  struct dirent *dir;
  
  DIR * direct = opendir(directory);
  char* name = malloc((strlen(directory) + 258));
  
  if(direct == NULL){
    perror("opendir");
    exit(-1);
  }else if(name == NULL){
    perror("malloc");
    exit(-1);
  }
  
  while((dir = readdir(direct)) != NULL) {//loop in dir
    if(!sprintf(name, "%s/%s", directory, dir->d_name)){
      perror("sprintf");
      exit(-1);
    }
    
    if(lstat(name, &buf) != 0){
      perror("lstat");
      exit(-1);
    }
    if(!S_ISLNK(buf.st_mode) && strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0 ){
      if(!fwrite(&buf.st_ino, 8, 1, file)){//inode
        perror("fwrite");
        exit(-1);
      }
      
      int len = strlen(name);
      if(!fwrite(&len, 4, 1, file)){//file name len
        perror("fwrite");
        exit(-1);
      }else if(!fwrite(name, len, 1, file)){//file name
        perror("fwrite");
        exit(-1);
      }else if(!get_inode(buf.st_ino)){
        set_inode(buf.st_ino, name);
        
        if(!fwrite(&buf.st_mode, 4, 1, file)) {//mode
          perror("fwrite");
          exit(-1);
        }else if(!fwrite(&buf.st_mtim.tv_sec, 8, 1, file)) {//mode time
          perror("fwrite");
          exit(-1);
        }else if(!S_ISDIR(buf.st_mode)){
          if (!fwrite(&buf.st_size, 8, 1, file)) {//size
            perror("fwrite");
            exit(-1);
          }
          
          if(buf.st_size != 0) {
            FILE *input = fopen(name, "r");
            if (input == NULL) {
              perror("fopen");
              exit(-1);
            }
            
            char *word = malloc(buf.st_size * sizeof(char));
            if(word == NULL) {
              perror("malloc");
              exit(-1);
            }else if(!fread(word, buf.st_size, 1, input)){//contents
              perror("fread");
              exit(-1);
            }else if(!fwrite(word, buf.st_size, 1, file)){
                perror("fwrite");
                exit(-1);
            }else if (fclose(input) != 0){
              perror("fclose");
              exit(-1);
            }
            free(word);
          }
        }
      }
    }
    if (S_ISDIR(buf.st_mode) && strcmp(dir->d_name, ".") !=0 && strcmp(dir->d_name, "..") !=0 ) {
      findAllFiles(file, name);//recursion in directory til no new dir
    }
  }
  if (closedir(direct) != 0){
    perror("closedir");
    exit(-1);
  }
  free(name);
}

void archive(char* name, char* directory){//create an archive
  FILE *file = fopen(name, "w+");
 
  int magicnum = 0x7261746D;
  struct stat buf;
  
  if(file == NULL){
    perror("fopen");
    exit(-1);
  }
  
  if(!fwrite(&magicnum, 4, 1, file)){//magic number
    perror("fwrite");
    exit(-1);
  }
  
  if(lstat(directory, &buf) != 0){
    perror("lstat");
    exit(-1);
  }
  
  if(!fwrite(&buf.st_ino, 8, 1, file)){//inode
    perror("fwrite");
    exit(-1);
  }
  set_inode(buf.st_ino, directory);
  
  int len = strlen(directory);
  if(!fwrite(&len, 4, 1, file)){//file name len
    perror("fwrite");
    exit(-1);
  }
  
  if(!fwrite(directory, len, 1, file)){//file name
    perror("fwrite");
    exit(-1);
  }
  
  if(!fwrite(&buf.st_mode, 4, 1, file)){//mode
    perror("fwrite");
    exit(-1);
  }
  
  if(!fwrite(&buf.st_mtim.tv_sec, 8, 1, file)){//modtime
    perror("fwrite");
    exit(-1);
  }
  
  findAllFiles(file, directory);
  
  closeFile(file);
  
}

void extractFiles(FILE* file){//extract files in archive
  int64_t setinodevalue;
  ino_t inodevalue;
  while(fread(&setinodevalue, 8, 1, file) ==1){
    inodevalue = (ino_t) setinodevalue;
    int len;
    if (!fread(&len, 4, 1, file)){//file name length
      perror("fread");
      exit(-1);
    }
    char *filename = (char *) malloc(len * sizeof(char));
    if (filename == NULL){
      perror("malloc");
      exit(-1);
    }
    if (!fread(filename, len, 1, file)){//file name
      perror("fread");
      exit(-1);
    }
    filename[len] = '\0';
    
    if(get_inode(inodevalue) != NULL){//extract hardlink
      char* path = (char *) malloc(sizeof(char) * (1 + strlen(get_inode(inodevalue))));
      if(path == NULL){
        perror("malloc");
        exit(-1);
      }else if(strcpy(path, get_inode(inodevalue)) == NULL){
        perror("strcpy");
        exit(-1);
      }else if(link(path, filename) != 0){
        perror("link");
        exit(-1);
      }
      free(path);
      
    }else{//reg and directory
      int32_t mode;
      int64_t modtime;
      if(!fread(&mode, 4, 1, file)){
        perror("fread");
        exit(-1);
      }else if(!fread(&modtime, 8, 1, file)){
        perror("fread");
        exit(-1);
      }
      
      if(!S_ISDIR((mode_t) mode)){//check directory
        int64_t size;
        FILE* newfile = fopen(filename, "w+");
        
        if(newfile == NULL){
          perror("fopen");
          exit(-1);
        }
        if(!fread(&size, 8, 1, file)){
          perror("fread");
          exit(-1);
        }
        if(size != 0){
          char* word = malloc(sizeof(char) * size);//save content
          if(word == NULL){
            perror("malloc");
            exit(-1);
          }else if(!fread(word , size, 1, file)){
            perror("fread");
            exit(-1);
          }else if(!fwrite(word , size, 1, newfile)){
            perror("fwrite");
            exit(-1);
          }
          free(word);
        }
        struct timeval actime; //time
        struct timeval modetime;
        if(chmod(filename, (mode_t)mode) != 0){
          perror("chmod");
          exit(-1);
        }
        if(gettimeofday(&actime, NULL) != 0){
          perror("gettimeofday");
            exit(-1);
        }
        modetime.tv_usec = 0;
        modetime.tv_sec = (time_t) modtime;
        struct timeval time[2] = {actime, modetime};
        
        if(utimes(filename, time) != 0){
          perror("utimes");
          exit(-1);
        }
        
        set_inode(inodevalue, filename);
        
        closeFile(newfile);
        
      }else{
        if(mkdir(filename, (mode_t) mode) != 0){
          perror("mkdir");
          exit(-1);
        }
        set_inode(inodevalue, filename);
      }
    }
    free(filename);
  }
}

void extract(char* name){
  FILE* file = fopen(name, "r");
  if(file == NULL){//ERROR CHECKS
    perror("fopen");
    exit(-1);
  }
  int magicnum;
  magicnum = checkmagicnum(file, magicnum);
  
  extractFiles(file);
  
  if(fclose(file)!= 0){
    perror("fclose");
    exit(-1);
  }
}

void print(char* name){
  FILE * file = fopen(name, "r");
  if(file == NULL){//ERROR CHECKS
    perror("fopen");
    exit(-1);
  }
  int magicnum;
  magicnum = checkmagicnum(file, magicnum);
  
  int int_inodevalue;
  ino_t inodevalue;
  while(fread(&int_inodevalue, 8, 1, file) == 1){  
    inodevalue = (ino_t) int_inodevalue;
    int len;
    if (!fread(&len, 4, 1, file)){//file name length
      perror("fread");
      exit(-1);
    }
    char *filename = (char *) malloc(len * sizeof(char));
    if (filename == NULL){
      perror("malloc");
      exit(-1);
    }
    if (!fread(filename, len, 1, file)){//file name
      perror("fread");
      exit(-1);
    }
    filename[len] = '\0'; 
    
    if(get_inode(inodevalue) != NULL){//print hardlink
      printf("%s/ inode: %llu\n", filename, (unsigned long long)inodevalue);
    
    }else{
      mode_t mode;
      int setmode;
      
      time_t modtime;
      int setmodtime;
      
      if(fread(&setmode, 4, 1, file) == -1){//mode
        perror("fread");
        exit(-1);
      }
      mode = (mode_t) setmode;
      if (!fread(&setmodtime, 8, 1, file)){//modtime
        perror("fread");
        exit(-1);
      }
      modtime = (time_t) setmodtime;
      set_inode(inodevalue, filename);
      if(S_ISDIR(mode)){//print directory
        printf("%s/ inode: %llu, mode: %o, mtime: %llu\n", filename, (unsigned long long)inodevalue, mode, (unsigned long long)modtime);
      
      }else{
        off_t size;
        int intsize;
        if(!fread(&intsize, 8, 1, file)){//size
          perror("fread");
          exit(-1);
        }
        size = (off_t) intsize;
        if(size != 0){
          char * word = (char*)malloc(sizeof(char) * size);
          if(word == NULL){
            perror("malloc");
            exit(-1);
          }else if(!fread(word, size, 1, file)){//content
            perror("fread");
            exit(-1);
          }
        }
        if ((mode & S_IXUSR) || (mode & S_IXGRP) || (mode & S_IXOTH)){ //executable file
          printf("%s* -- inode: %llu, mode: %o, mtime: %llu, size: %llu\n", filename, (unsigned long long)inodevalue, mode, (unsigned long long) modtime, (unsigned long long)size);
        }else{ //regular file
          printf("%s -- inode: %llu, mode: %o, mtime: %llu, size: %llu\n", filename, (unsigned long long)inodevalue, mode, (unsigned long long) modtime, (unsigned long long)size);
        }
      
      }
    }
    free(filename);
  }
  closeFile(file);
}

int main(int argc, char *argv[])//case selection for mytar
{
  char c;
  char option;
  int count = 0;
  char* archivename = NULL;
  char* directory;
  while((c = getopt(argc, argv, "cxtf:")) != -1){
    switch (c){//Error Check
      case'c'://Create
        count++;
        multNodesError(count);
        option = 'c';
        break;
        
      case'x'://Extract
        count++;
        multNodesError(count);
        option = 'x';
        break;
        
      case't'://Print
        count++;
        multNodesError(count);
        option = 't';
        break;
        
      case'f'://Archive File Name
        archivename = optarg;
        break;
        
      case'?'://ERROR
        fprintf(stderr, "Error: No tarfile specified.\n");
        exit(-1);
        
      default:
        fprintf(stderr, "Error: No mode specified.\n");
        exit(-1);
    }
  }
  switch(option){
    case 'c':
      directory = argv[optind];
      if(archivename == NULL){
        fprintf(stderr, "Error: No tarfile specified.\n");
        exit(-1);
      }
      if(directory == NULL){
        fprintf(stderr, "Error: No directory target specified.\n");
        exit(-1);
      }else{
        struct stat buf;
        if(lstat(directory, &buf) !=0){
          fprintf(stderr, "Error: Specified target(\"%s\") does not exist.\n", directory);
          exit(-1);
        }else if(!S_ISDIR(buf.st_mode)){
          fprintf(stderr, "Error: Specified target(\"%s\") is not a directory.\n", directory);
          exit(-1);
        }
      }
      archive(archivename, directory);
      
      break;
        
    case 'x':
      if(optind < argc){
        fprintf(stderr, "Error: No tarfile specified.\n");
        exit(-1);
      }
      extract(archivename);
      break;
        
    case 't':
      if(optind < argc){
        fprintf(stderr, "Error: No tarfile specified.\n");
        exit(-1);
      }
      print(archivename);
      break;
      
  }
  return 0;
}