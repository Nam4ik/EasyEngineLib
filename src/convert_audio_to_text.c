#include <stdio.h>
#include <stdlib.h>
#include <pocketsphinx.h>
#include <stdbool.h>
#include <cjson/cJSON.h>

void transcribe_audio(const char *audio_file_path, bool use_json) {
    ps_decoder_t *ps;
    cmd_ln_t *config;
    FILE *fh;
    int16 buf[512];
    int nread;
    int16 const *data;
    int32 score;
    if (use_json == true){
      cJSON *json = cJSON_CreateObject();
        if (json == NULL) {
          fprintf(stderr, "Ошибка при создании JSON-объекта\n");
          return;
    }    
    } 
    


    config = cmd_ln_init(NULL, ps_args(), TRUE,
                         "-hmm", "/home/$USER/.cache/yay/pocketsphinx/usr/share/pocketsphinx/en-us/en-us",
                         "-lm",  "/home/$USER/.cache/yay/pocketsphinx/usr/share/pocketsphinx/model/en-us/en-us.lm.bin",
                         "-dict","/home/$USER/.cache/yay/pocketsphinx/usr/share/pocketsphinx/model/en-us/cmudict-en-us.dict",
                         NULL);

    ps = ps_init(config);

    
    fh = fopen(audio_file_path, "rb");
    if (fh == NULL) {
        fprintf(stderr, "Could not open audio file\n");
        return;
    }

  
    ps_start_utt(ps);
    while ((nread = fread(buf, sizeof(int16), 512, fh)) > 0) {
        ps_process_raw(ps, buf, nread, FALSE, FALSE);
    }
    ps_end_utt(ps);

    
    const char *hyp = ps_get_hyp(ps, &score);
    if (hyp != NULL && use_json == true) {
        cJSON_AddStringToObject(json, "stdin" ,hyp);
    } else {
        cJSON_AddStringToObject(json, "stderr", "Could not understand audio\n");
    }
    if (hyp != NULL && use_json == false) {
      printf("Recognized audio:", hyp);
    } else {
      printf("Error! Could not understand audio\n");
    }
    
    fclose(fh);
    ps_free(ps);
    cmd_ln_free_r(config);
}



int main(int argc, char *argv[] ) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <audio_file_path>\n", argv[0]);
        return 1;
    }

    transcribe_audio(argv[1]);
    return 0;
}

