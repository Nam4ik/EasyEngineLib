#ifndef CONVERT_H
#define CONVERT_H

#include <stdio.h>
#include <stdlib.h>
#include <cjson/cJSON.h>
#include <time.h>

time_t now = time(&now);
struct tm *local = localtime(&now);

char[] filename = "voicerec%d.log", local;

system("touch", filename);

#ifdef __WIN32
  
  return 0; 
    


#endif 

