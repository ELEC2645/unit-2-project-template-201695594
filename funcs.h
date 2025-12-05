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
    float k;
} converter;

void print_float(float val);

int is_integer(const char *s); // checks iff parsed string can be converted to an int

float get_float_input(void);
int get_int_input(int *out);

int compute_buck(converter *active);
int compute_boost(converter *active);
int compute_buckboost(converter *active);
int compute_cuk(converter *active);

void edit_param(converter *active, int field);

enum conv_fields {V_O, V_I, I_O, I_I, R_L, F_S, D_I, D_V, D_I2 ,D_V2, L, C_O, L2, C_N};

#endif