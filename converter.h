#ifndef CONVERTER_H
#define CONVERTER_H
/* Library of functions for calculating component parameters of a DC-DC converter*/

enum converter_type {BUCK, BOOST, BUCKBOOST, CUK};

typedef struct /*Converter parameters*/
{
    /* data */
    char save_name[64];
    float V_out;
    float V_in;
    float I_out;
    float I_in;
    float I_rip;
    float V_rip;
    float R_load;
    float F_switch;
    int type;
    int ccm;
    float k;
} converter_spec;



#endif /*CONVERTER_H*/
