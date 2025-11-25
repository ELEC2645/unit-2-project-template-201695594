#ifndef CONVERTER_H
#define CONVERTER_H
/* Library of functions for calculating component parameters of a DC-DC converter*/

enum converter_type {BUCK, BOOST, BUCKBOOST, CUK};

typedef struct /*Converter parameters*/
{
    /* data */
    float V_out[2];
    float V_in[2];
    float I_out[2];
    float I_in[2];
    float I_rip;
    float V_rip;
    float R_load[2];
    float F_switch[2];
    int type;
} converter_param;

/*BUCK converter functions*/
void ccm_buck_duty(converter_param *target);

#endif /*CONVERTER_H*/