#include <stdio.h>
#include "converter.h"




/* void ccm_buck_duty(converter_spec *target, float *k){
    // check v_out and v_in are not null
    if (target->V_out && target->V_in){
        *k = target->V_out / target->V_in;
    }
    // check i_out and i_in are not null
    else if (target->I_out && target->I_in){
        *k = target->I_out / target->I_in;
    }
    else {
        printf("Error in ccm_buck_duty, invalid inputs\n");
    }
}

void buck_inductor(converter_spec *target, float *L){
    *L = (1 - target->k) * (target->V_out) / (target->F_switch * target->I_rip);
}

void buck_capacitor(converter_spec *target, float L, float *C){
    *C = (target->V_out/target->V_rip)*(1-target->k)/ 8*(target->F_switch)*(target->F_switch)*L;
} */