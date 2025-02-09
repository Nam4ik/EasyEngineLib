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

FILE fp* = filename

#ifdef __WIN32  
  fputs(fp, "Unsupported system type. Try use python voice recognizer");
  return 0; 
#else 
  void transcribe_audio(const char *audio_file_path, bool use_json);
  

#endif 

