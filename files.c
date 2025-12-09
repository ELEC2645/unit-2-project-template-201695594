#include <stdio.h>
#include <string.h>
#include "dirent.h" // library for retrieving info about files and directories: https://github.com/tronkko/dirent
#include "cJSON.h" // library that implements basic .json parsing. from: https://github.com/DaveGamble/cJSON 
#include "funcs.h"
#include "menu_funcs.h"
#include "files.h"

int read_converter(char *path, converter *target){
    // open file
    FILE *fp = fopen(path, "r");
    if (fp == NULL){
        printf("Error: Unable to open file\n");
        return 1;
    }
    // read file into buffer as a string
    char buffer[1024];
    fread(buffer, 1, sizeof(buffer),fp);
    fclose(fp); // close file

    // create json object and parse read string
    cJSON *json = cJSON_Parse(buffer);
    // error handling
    if (json == NULL){
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL){
            printf("Error: %s", error_ptr);
        }
        cJSON_Delete(json);
        return 1;
    }

    // access json data and add to target converter struct
    // string data:
    strncpy(target->name, cJSON_GetObjectItemCaseSensitive(json, "name")->valuestring, sizeof(target->name));
    strncpy(target->s_type, cJSON_GetObjectItemCaseSensitive(json, "type")->valuestring, sizeof(target->s_type));
    target->type = cJSON_GetObjectItemCaseSensitive(json, "type_f")->valueint;
    // design parameters:
    target->V_o = cJSON_GetObjectItemCaseSensitive(json, "V_o")->valuedouble;
    target->V_i = cJSON_GetObjectItemCaseSensitive(json, "V_i")->valuedouble;
    target->I_o = cJSON_GetObjectItemCaseSensitive(json, "I_o")->valuedouble;
    target->I_i = cJSON_GetObjectItemCaseSensitive(json, "I_i")->valuedouble;
    target->R_l = cJSON_GetObjectItemCaseSensitive(json, "R_l")->valuedouble;
    target->F_s = cJSON_GetObjectItemCaseSensitive(json, "F_s")->valuedouble;
    target->i_rip = cJSON_GetObjectItemCaseSensitive(json, "i_rip")->valuedouble;
    target->i_rip2 = cJSON_GetObjectItemCaseSensitive(json, "i_rip2")->valuedouble;
    target->v_rip = cJSON_GetObjectItemCaseSensitive(json, "v_rip")->valuedouble;
    target->v_rip2 = cJSON_GetObjectItemCaseSensitive(json, "v_rip2")->valuedouble;
    // component values
    target->C_o = cJSON_GetObjectItemCaseSensitive(json, "C_o")->valuedouble;
    target->C_n = cJSON_GetObjectItemCaseSensitive(json, "C_n")->valuedouble;
    target->L = cJSON_GetObjectItemCaseSensitive(json, "L")->valuedouble;
    target->L2 = cJSON_GetObjectItemCaseSensitive(json, "L2")->valuedouble;
    target->k = cJSON_GetObjectItemCaseSensitive(json, "k")->valuedouble;
    // delete json to prevent any memory leaks
    cJSON_Delete(json);
    return 0;
}

int write_converter(char *path, converter *data){
    //create json object
    cJSON *json = cJSON_CreateObject();
    // write converter fields to json
    // name and type
    cJSON_AddStringToObject(json, "name", data->name);
    cJSON_AddStringToObject(json, "type", data->s_type);
    cJSON_AddNumberToObject(json, "type_f", data->type); // numerical type flag
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
    
    fputs(json_str, fp);
    fclose(fp);
    // free the JSON string and cJSON object
    cJSON_free(json_str);
    cJSON_Delete(json);
    return 0;
}

void print_saves(){
    DIR *d;
    struct dirent *dir;
    d = opendir("./saves");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            printf("%s\n", dir->d_name);
        }
        closedir(d);
    }
}