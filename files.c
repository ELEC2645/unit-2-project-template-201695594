#include <stdio.h>
#include "cJSON.h"
#include "funcs.h"
#include "menu_funcs.h"
#include "files.h"

//const char conv_types[][16] = {"Buck", "Boost", "Buck-Boost", "Cuk"};

//int read_converter(char *path)
int write_converter(char *path, converter *data){
    //create json object
    cJSON *json = cJSON_CreateObject();
    // write converter fields to json
    // name and type
    cJSON_AddStringToObject(json, "name", data->name);
    cJSON_AddStringToObject(json, "type", data->s_type);
    // numerical fields
    cJSON_AddNumberToObject(json, "V_o", data->V_o);
    cJSON_AddNumberToObject(json, "V_i", data->V_i);
    cJSON_AddNumberToObject(json, "I_o", data->I_o);
    cJSON_AddNumberToObject(json, "I_i", data->I_i);
    cJSON_AddNumberToObject(json, "R_l", data->R_l);
    cJSON_AddNumberToObject(json, "F_s", data->F_s);
    cJSON_AddNumberToObject(json, "i_rip", data->i_rip);
    cJSON_AddNumberToObject(json, "i_rip2", data->i_rip2);
    cJSON_AddNumberToObject(json, "v_rip", data->v_rip);
    cJSON_AddNumberToObject(json, "v_rip2", data->v_rip2);
    // component values
    cJSON_AddNumberToObject(json, "C_o", data->C_o);
    cJSON_AddNumberToObject(json, "C_n", data->C_n);
    cJSON_AddNumberToObject(json, "L", data->L);
    cJSON_AddNumberToObject(json, "L2", data->L2);
    cJSON_AddNumberToObject(json, "k", data->k);

    // convert JSON to str
    char *json_str = cJSON_Print(json);
    FILE *fp = fopen(path, "w");
    if (fp == NULL){
        printf("Error: Unable to open file\n");
        return 1;
    }
    printf("%s\n", json_str);
   fputs(json_str, fp);
   fclose(fp);
   // free the JSON string and cJSON object
   cJSON_free(json_str);
   cJSON_Delete(json);
   return 0;
}