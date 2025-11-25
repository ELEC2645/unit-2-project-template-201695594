#include "converter.h"


void ccm_buck_duty(converter_param *target, float *k){
    // check v_out and v_in are not null
    if (*target->V_out && *target->V_in){
        *k = *target->V_out / *target->V_in;
    }
    // check i_out and i_in are not null
    else if (!*target->I_out && !*target->I_in){
        *k = *target->I_out / *target->I_in;
    }
}