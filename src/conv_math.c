#include <stdio.h>
#include "include/conv_math.h"

#include "include/colours.h"

int KfromV(converter *active){
    switch (active->type)
        {
        case Buck:
            // k = Vo / Vi
            active->k = active->V_o / active->V_i; 
            break;
        case Boost:
            // (Vo - Vi) / Vo
            active->k = (active->V_o - active->V_i) / active->V_o;
            break;
        case BuckBoost:
            // k = Vo / (Vo + Vi)
            active->k = active->V_o / (active->V_i + active->V_o);
            break;
        case Cuk:
            // k = Vo / (Vo + Vi)
            active->k = active->V_o / (active->V_i + active->V_o);
            break;

        default:
            printf("Switch Error: Computing duty cycle from voltage failed");
            return 1;
        }
    if (active->k > 1 || active->k < 0){ // check calculated k is valid
        printf(RED"Duty cycle calculation failed.\nInvalid V_o and V_i values.\n"RESET);
        active->k = -1; // reset k
        return 1; // return an error if k is invalid
    }
    else { return 0;}
}

int KfromI(converter *active){
    switch (active->type)
        {
        case Buck:
            // k = Ii / Io
            active->k = active->I_i / active->I_o; 
            break;
        case Boost:
            // k = (Ii - Io) / Ii
            active->k = (active->I_i - active->I_o) / active->I_i;
            break;
        case BuckBoost:
            // k = Ii / (Io + Ii)
            active->k = active->I_i / (active->I_o + active->I_i);
            break;
        case Cuk:
            // k = Ii / (Io + Ii)
            active->k = active->I_i / (active->I_o + active->I_i);
            break;
        
        default:
            printf("Switch Error: Computing duty cycle from current failed");
            return 1;
        }
    if (active->k > 1 || active->k < 0){ // check calculated k is valid
        printf(RED"Duty cycle calculation failed.\nInvalid I_o and I_i values.\n"RESET);
        active->k = -1; // reset k
        return 1; // return an error if k is invalid
    }

    else { return 0; }
}

int VofromOhm(converter *active){
    // no switch-case, same for all converter types
    active->V_o = active->I_o * active->R_l; // v = IR
    return 0;
}

int VofromK(converter *active){
    switch (active->type)
        {
        case Buck:
            // Vo = Vi * k
            active->V_o = active->V_i * active->k; 
            break;
        case Boost:
            // Vo = Vi / (1-k)
            active->V_o = active->V_i / (1 - active->k);
            break;
        case BuckBoost:
            // Vo = k*Vi / (1-k)
            active->V_o = (active->V_i * active->k) / (1 - active->k);
            break;
        case Cuk:
            // Vo = k*Vi / (1-k)
            active->V_o = (active->V_i * active->k) / (1 - active->k);
            break;
        default:
            printf("Switch Error: Computing Vo from duty cycle failed");
            return 1;
        }
    return 0;
}

int VifromK(converter *active){
    switch (active->type)
        {
        case Buck:
            active->V_i = active->V_o / active->k; // Vi = Vo / k
            break;
        case Boost:
            active->V_i = active->V_o * (1 - active->k);
            break;
        case BuckBoost:
            // Vi = Vo*(1-k) / k
            active->V_i = active->V_o * (1 - active->k) / active->k;
            break;
        case Cuk:
            // Vi = Vo*(1-k) / k
            active->V_i = active->V_o * (1 - active->k) / active->k;
            break;
        default:
            printf("Switch Error: Computing Vi from duty cycle failed");
            return 1;
        }
    return 0;
}

int IofromOhm(converter* active){
    // no switch-case,same for all converter types
    active->I_o = active->V_o / active->R_l; // Io = Vo / Rl
    return 0;
}

int IofromK(converter *active){
    switch (active->type)
        {
        case Buck:
            // Io = Ii / k
            active->I_o = active->I_i / active->k;
            break;
        case Boost:
            active->I_o = active->I_i * (1 - active->k);
            break;
        case BuckBoost:
            // Io = Ii*(1-k) / k
            active->I_o = active->I_i * (1 - active->k) / active->k;
            break;
        case Cuk:
            // Io = Ii*(1-k) / k
            active->I_o = active->I_i * (1 - active->k) / active->k;
            break;
        default:
            printf("Switch Error: Computing Io from duty cycle failed");
            return 1;
        }
    return 0;
}

int IifromK(converter *active){
    switch (active->type)
        {
        case Buck:
            // Ii = Io * k
            active->I_i = active->I_o * active->k;
            break;
        case Boost:
            //Ii = Io / (1-k)
            active->I_i = active->I_o / (1 - active->k);
            break;
        case BuckBoost:
            // Ii = Io*k /(1-k)
            active->I_i = (active->I_o * active->k) / (1 - active->k);
            break;
        case Cuk:
            // Ii = Io*k /(1-k)
            active->I_i = (active->I_o * active->k) / (1 - active->k);
            break;
        default:
            printf("Switch Error: Computing Io from duty cycle failed");
            return 1;
        }
    return 0;               
}

int calc_L(converter *active){
     switch (active->type)
        {
        case Buck:
            // L = (1-k)*Vo / (fs*di)
            active->L = (1 - active->k) * active->V_o / (active->F_s * active->i_rip);
            break;
        case Boost:
            // L = k*Vi / (Fs*dI)
            active->L = active->k * active->V_i / (active->F_s * active->i_rip);
            break;
        case BuckBoost:
            // L = Vi*k / (Fs*di)
            active->L = active->k * active->V_i / (active->F_s * active->i_rip);
            break;
        case Cuk:
            // L (1) = k*Vi / (Fs*dI)
            active->L = active->k * active->V_i / (active->F_s * active->i_rip);
            break;
        default:
            printf("Switch Error: Computing L failed");
            return 1;
        }
    return 0;
}

int calc_Co(converter *active){
    switch (active->type)
        {
        case Buck:
            // Co = k*(Vi-Vo) / (8*Fs^2*dVo*L)
            active->C_o = active->k * (active->V_i - active->V_o) / (8* active->F_s * active->F_s * active->v_rip * active->L);
            break;
        case Boost:
            // Co = Io*k / (dVo*Fs) 
            active->C_o = (active->I_o * active->k) / (active->v_rip * active->F_s);
            break;
        case BuckBoost:
            // Co = Io*k / dVo*Fs
            active->C_o = (active->I_o * active->k) / (active->v_rip * active->F_s);
            break;
        case Cuk:
            // Co = Vi*k / (8*Fs^2*dVo*L2)
            active->C_o = (active->V_i * active->k) / (8 * active->F_s * active->F_s * active->v_rip * active->L2);
            break;
        default:
            printf("Switch Error: Computing L failed");
            return 1;
        }
    return 0;
}

int calc_Cn(converter *active){
    // Cn = Io*k / (Fs*dV2)
    active->C_n = (active->I_o * active->k) / (active->F_s * active->v_rip2);
    return 0;
}

int calc_L2(converter *active){
    // L2 = k*Vi / (Fs*dI2)
    active->L2 = active->k * active->V_i / (active->F_s * active->i_rip2);
    return 0;
}