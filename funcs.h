#ifndef FUNCS_H
#define FUNCS_H

typedef struct {
    // init parameter
    int type; // 1-Buck 2-Boost 3-BuckBoost 4-Cuk 
    char name[128];
    // input target specs
    float V_o; // average output voltage - V
    float V_i; // average input voltage - V
    float I_o; // average output current - A
    float I_i; // average input current - A
    float R_l; // resistance of load - Ohm
    float F_s; // switching frequency - Hz
    float i_rip; // current ripple in inductor - A
    float v_rip; // voltage ripple in C_o  - V
    float i_rip2; // current ripple in L2 (Cuk only) - A
    float v_rip2; // voltage ripple in C_n (Cuk only) - V
    //design values
    float L; // primary inductor value (L1 in Cuk) - H
    float C_o; // output capacitor (parallel to load) - F
    float L2; // second inductor (only in Cuk) - H
    float C_n; // second capacitor (only in Cuk) - H
    float k; // duty ratio of converter (ratio between 0-1)
} converter;

// prints passed float to 4 dp with additional colour formatting
void print_float(float val);

 // checks if parsed string can be converted to an int
int is_integer(const char *s);

// gets a float input from the user, returns -1 if blank inputted
float get_float_input(void);

// gets an integer input from the user via a pointer to the output
// returns 2 if user cancelled the input 
// returns 1 if input is not an int
int get_int_input(int *out);

// each function computes missing values and component values for a converter type
int compute_buck(converter *active);
int compute_boost(converter *active);
int compute_buckboost(converter *active);
int compute_cuk(converter *active);

// used to edit 1 field of a converter struct
// int field corresponds to followinng enum
void edit_param(converter *active, int field);

enum conv_fields {V_O, V_I, I_O, I_I, R_L, F_S, D_I, D_V, D_I2 ,D_V2, L, C_O, L2, C_N};

#endif