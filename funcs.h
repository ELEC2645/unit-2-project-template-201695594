#ifndef FUNCS_H
#define FUNCS_H

typedef struct {
    // init parameter
    int type;
    char name[128];
    // input target specs
    float V_o;
    float V_i;
    float I_o;
    float I_i;
    float R_l;
    float F_s;
    float i_rip;
    float v_rip;
    float i_rip2;
    float v_rip2;
    //design values
    float L;
    float C_o;
    float L2;
    float C_n;
} converter;


void menu_item_1(converter *active);
void menu_item_2(converter *active);
void menu_item_3(converter *active);
void menu_item_4(converter *active);
void menu_item_5(converter *active);

float get_float_input();
void print_converter(converter *active);

#endif