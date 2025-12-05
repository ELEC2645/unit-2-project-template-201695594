#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "colours.h"
#include "funcs.h"
#include "menu_funcs.h"


// function used to set the colour of printed floats
// x == -1 -> red
// x > 0 -> green
// x == -2 -> black (ignored)
void print_float(float val){
    if (val == -1.0){
        printf(RED"%.4f"WHITE, val);
    }
    else if (val == -2){
        printf(HIDE"%.1f"RESET, val);
    }
    else {
        printf(GREEN"%.4f"WHITE, val);
    }
}


float get_float_input(void){
    char buf[128]; // read in buffer
    char *conv_check;
    float result;
    int found = 0;
    do{
        if (!fgets(buf, sizeof(buf), stdin)) {
            /* EOF or error; bail out gracefully */
            puts("\nInput error. Exiting.");
            exit(1);
        }
        // strip trailing newline
        buf[strcspn(buf, "\r\n")] = '\0';
        result = strtof(buf, &conv_check); // try to convert input to float
        if (*conv_check != '\0'){ // if returned end value is not '\0' then conversion failed
            printf("\nConversion to float failed\nPlease try again:");
            continue;
        }
        else if (result < 0){ // negative inputs are not allowed
            printf("Negative values are not permitted\n Please try again:");
            continue;
        }
        else if (buf[0] == '\0') { // if a blank line was submitted
            result = -1;
            found = 1;            
        }
        else {found = 1;}
    } while (!found);
return result;
}


/* Return 1 if s is an optional [+/-] followed by one-or-more digits, else 0. */
int is_integer(const char *s)
{
    if (!s || !*s) return 0;

    /* optional sign */
    if (*s == '+' || *s == '-') s++;

    /* must have at least one digit */
    if (!isdigit((unsigned char)*s)) return 0;

    while (*s) {
        if (!isdigit((unsigned char)*s)) return 0;
        s++;
    }
    return 1;
}

int get_int_input(int *out) {
    char buf[128];
        if (!fgets(buf, sizeof(buf), stdin)) {
            /* EOF or error; bail out gracefully */
            puts("\nInput error. Exiting.");
            exit(1);
        }
        // strip trailing newline
        buf[strcspn(buf, "\r\n")] = '\0';

        if (!is_integer(buf)) {
            if (buf[0] == 'C' || buf[0] == 'c'){
                return 2; // additional error indicating user cancelled operation
            }
            else {return 1;} // error if input is not an integer
        }
        else {
            *out = (int)strtol(buf, NULL, 10);
            return 0;
        }
}

void edit_param(converter *active, int field) {

    switch (field)
    {
    case V_O:
        printf("\nEnter a new V_o value: ");
        active->V_o = get_float_input();
        printf("\nV_o set to: %.4f", active->V_o);
        break;
    case V_I:
        printf("\nEnter a new V_i value: ");
        active->V_i = get_float_input();
        printf("\nV_i set to: %.4f", active->V_i);
        break;
    case I_O:
        printf("\nEnter a new I_o value: ");
        active->I_o = get_float_input();
        printf("\nI_o set to: %.4f", active->I_o);
        break;
    case I_I:
        printf("\nEnter a new I_i value: ");
        active->I_i = get_float_input();
        printf("\nI_i set to: %.4f", active->I_i);
        break;
    case R_L:
        printf("\nEnter a new R_l value: ");
        active->R_l = get_float_input();
        printf("\nR_l set to: %.4f", active->R_l);
        break;
    case F_S:
        printf("\nEnter a new F_s value: ");
        active->F_s = get_float_input();
        printf("\nF_s set to: %.4f", active->F_s);
        break;
    case D_I:
        printf("\nEnter a new delta_i value: ");
        active->i_rip = get_float_input();
        printf("\ndelta_i set to: %.4f", active->i_rip);
        break;
    case D_V:
        printf("\nEnter a new delta_v value: ");
        active->v_rip = get_float_input();
        printf("\ndelta_v set to: %.4f", active->v_rip);
        break;
    case D_I2:
        if (active->type > 2){
            printf("\nEnter a new delta_i2 value: ");
            active->i_rip2 = get_float_input();
            printf("\ndelta_i2 set to: %.4f", active->i_rip2);
            break;
        } else {active->i_rip2 = -2; break;}
    case D_V2:
        if (active->type > 2){
            printf("\nEnter a new delta_v2 value: ");
            active->v_rip2 = get_float_input();
            printf("\ndelta_v2 set to: %.4f", active->v_rip2);
            break;
        } else {active->v_rip2 = -2; break;} 
    default:
        printf("Invalid Selection");
        break;
    }
}

int compute_buck(converter *active){
    print_converter(active);
    int changes;
    int missing = 0;
    do {
        changes = 0;
        // duty cycle  
        if (active->k == -1){      
            if (active->V_o >= 0 && active->V_i >= 0){
                active->k = active->V_o / active->V_i;
                changes++;
                printf("Duty cycle computed as: %.4f\n", active->k);
            }
            else if (active->I_o >= 0 && active->I_i >= 0) {
                active->k = active->I_i / active->I_o;
                changes++;
                printf("Duty cycle computed as: %.4f\n", active->k);
            }
            else {missing++;}
        }
        // V_o
        if (active->V_o == -1){
            if (active->I_o >= 0 && active->R_l >= 0){
                active->V_o = active->I_o * active->R_l;
                changes++;
                printf("V_o computed as: %.4f\n", active->V_o);
            }
            else if (active->V_i >= 0 && active->k >= 0){
                active->V_o = active->V_i * active->k;
                changes++;
                printf("V_o computed as: %.4f\n", active->V_o);
            }
            else {missing++;}
        }
        // V_i
        if (active->V_i == -1){
            if (active->k >= 0 && active->V_o >= 0){
                active->V_i = active->V_o / active->k;
                changes++;
                printf("V_i computed as: %.4f\n", active->V_i);
            }
            else {missing++;}
        }
        // I_o
        if (active->I_o == -1){
            if (active->V_o >= 0 && active->R_l >= 0){
                active->I_o = active->V_o / active->R_l;
                changes++;
                printf("I_o computed as: %.4f\n", active->I_o);
            }
            else if (active->I_i >= 0 && active->k >= 0){
                active->I_o = active->I_i * active->k;
                changes++;
                printf("I_o computed as: %.4f\n", active->I_o);
            }
            else {missing++;}
        }
        // I_i
        if (active->I_i == -1){
            if (active->k >= 0 && active->I_o >= 0){
                active->I_i = active->I_o * active->k;
                changes++;
                printf("I_i computed as: %.4f\n", active->I_i);
            }
            else {missing++;}
        }
    } while (changes != 0 && missing != 0);
    if (missing > 0){
        printf("Converter computation failed, please provide more input parameters\n");
    }
    else {
        printf("Converter computation successful, moving to component calculations...\n");
    }
}

int compute_boost(converter *active){
    print_converter(active);
    int changes;
    int missing = 0;
    do {
        changes = 0;
        // duty cycle  
        if (active->k == -1){ 
            // calculate k from voltage     
            if (active->V_o >= 0 && active->V_i >= 0){
                active->k = (active->V_o - active->V_i) / active->V_o;
                changes++;
                printf("Duty cycle computed as: %.4f\n", active->k);
            }
            // calculate k from current
            else if (active->I_o >= 0 && active->I_i >= 0) {
                active->k = (active->I_i - active->I_o) / active->I_i;
                changes++;
                printf("Duty cycle computed as: %.4f\n", active->k);
            }
            else {missing++;} // unable to calculate, increment missing value counter
        }
        // V_o
        if (active->V_o == -1){
            // calculate output voltage using Ohm's Law
            if (active->I_o >= 0 && active->R_l >= 0){
                active->V_o = active->I_o * active->R_l;
                changes++;
                printf("V_o computed as: %.4f\n", active->V_o);
            }
            // calculate output voltage using duty ratio
            else if (active->V_i >= 0 && active->k >= 0){
                active->V_o = active->V_i / (1 - active->k);
                changes++;
                printf("V_o computed as: %.4f\n", active->V_o);
            }
            else {missing++;} // unable to calculate, increment missing value counter
        }
        // V_i
        if (active->V_i == -1){
            // calculate input voltage using duty ratio
            if (active->k >= 0 && active->V_o >= 0){
                active->V_i = active->V_o * (1 - active->k);
                changes++;
                printf("V_i computed as: %.4f\n", active->V_i);
            }
            else {missing++;} // unable to calculate, increment missing value counter
        }
        // I_o
        if (active->I_o == -1){
            // calculate output current using Ohm's Law
            if (active->V_o >= 0 && active->R_l >= 0){
                active->I_o = active->V_o / active->R_l;
                changes++;
                printf("I_o computed as: %.4f\n", active->I_o);
            }
            // calculate output current using duty ratio
            else if (active->I_i >= 0 && active->k >= 0){
                active->I_o = active->I_i * (1 - active->k);
                changes++;
                printf("I_o computed as: %.4f\n", active->I_o);
            }
            else {missing++;} // unable to calculate, increment missing value counter
        }
        // I_i
        if (active->I_i == -1){
            // calculate input current using duty ratio
            if (active->k >= 0 && active->I_o >= 0){
                active->I_i = active->I_o / (1 - active->k);
                changes++;
                printf("I_i computed as: %.4f\n", active->I_i);
            }
            else {missing++;} // unable to calculate, increment missing value counter
        }
    // continue looping until no more changes are made or all the missing values are calculated
    } while (changes != 0 && missing != 0);
    // warn user if not all values were able to be calculated
    if (missing > 0){
        printf("Converter computation failed, please provide more input parameters\n");
        return 1;
    }
    else {
        printf("Converter computation successful, moving to component calculations...\n");
    }
}
int compute_buckboost(converter *active){
    printf("Function not yet implemented");
    return 0;
}
int compute_cuk(converter *active){
    printf("Function not yet implemented");
    return 0;
}