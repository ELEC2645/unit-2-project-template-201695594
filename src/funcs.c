#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "include/colours.h"
#include "include/funcs.h"
#include "include/menu_funcs.h"
#include "include/conv_math.h"


// function used to set the colour of printed floats
// val == -1 -> red
// val > 0 -> green
// val == -2 -> hidden
void print_float(float val, int flag){
    if (flag){
        printf(BLUE"%.4e"RESET, val);
    }
    else {
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
}

// reads a float value from the user
float get_float_input(void){
    char buf[128]; // read in buffer
    char *conv_check;
    float result;
    int found = 0;
    do{
        if (!fgets(buf, sizeof(buf), stdin)) { // <- here is where string is read in
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
            result = -1; // set result to -1 indicating unknown value
            found = 1;            
        }
        else {found = 1;}
    } while (!found);
// return the result once a valid input is provided
return result;
}

// gets an integer input from the user, returns output via pointer
int get_int_input(int *out) {
    char buf[128]; // read in buffer

    if (!fgets(buf, sizeof(buf), stdin)) { // <- here is where string is read in
        /* EOF or error; bail out gracefully */
        puts("\nInput error. Exiting.");
        exit(1);
    }
    // strip trailing newline
    buf[strcspn(buf, "\r\n")] = '\0';

    if (!is_integer(buf)) { // check if string can be converted to int
        if (buf[0] == 'C' || buf[0] == 'c'){ // check for cancel char
            return 2; // return error indicating user cancelled operation
        }
        else {return 1;} // return error if input is not an integer
    }
    else {
        *out = (int)strtol(buf, NULL, 10); // convert str to int and write to out address
        return 0; // return 0 indicating no error
    }
}

/* Returns 1 if s is an optional [+/-] followed by one-or-more digits, else 0. */
int is_integer(const char *s)
{
    if (!s || !*s) return 0; // error if passed pointer is null

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

// edits a parameter of a converter struct from user input, int field flag selects field to edit
void edit_param(converter *active, int field) {
    // find correct field to edit and set value to user input
    switch (field)
    {
    case V_O:
        printf("\nEnter a new V_o value (V): ");
        active->V_o = get_float_input();
        printf("\nV_o set to: %.4fV", active->V_o);
        break;
    case V_I:
        printf("\nEnter a new V_i value (V): ");
        active->V_i = get_float_input();
        printf("\nV_i set to: %.4f(V)", active->V_i);
        break;
    case I_O:
        printf("\nEnter a new I_o value (A): ");
        active->I_o = get_float_input();
        printf("\nI_o set to: %.4fA", active->I_o);
        break;
    case I_I:
        printf("\nEnter a new I_i value (A): ");
        active->I_i = get_float_input();
        printf("\nI_i set to: %.4fA", active->I_i);
        break;
    case R_L:
        printf("\nEnter a new R_l value (Ohm): ");
        active->R_l = get_float_input();
        printf("\nR_l set to: %.4f Ohm", active->R_l);
        break;
    case F_S:
        printf("\nEnter a new F_s value (Hz): ");
        active->F_s = get_float_input();
        printf("\nF_s set to: %.4f Hz", active->F_s);
        break;
    case D_I:
        printf("\nEnter a new delta_i value (A): ");
        active->i_rip = get_float_input();
        printf("\ndelta_i set to: %.4fA", active->i_rip);
        break;
    case D_V:
        printf("\nEnter a new delta_v value (V): ");
        active->v_rip = get_float_input();
        printf("\ndelta_v set to: %.4fV", active->v_rip);
        break;
    case D_I2:
        if (active->type == 4){ // only allow to change if cuk type
            printf("\nEnter a new delta_i2 value (A): ");
            active->i_rip2 = get_float_input();
            printf("\ndelta_i2 set to: %.4fA", active->i_rip2);
            break;
        } else {active->i_rip2 = -2; break;} // skip if not Cuk converter
    case D_V2:
        if (active->type == 4){ // only allow to change if  cuk type
            printf("\nEnter a new delta_v2 value (V): ");
            active->v_rip2 = get_float_input();
            printf("\ndelta_v2 set to: %.4fV", active->v_rip2);
            break;
        } else {active->v_rip2 = -2; break;} // skip if not Cuk converter
    default:
        printf("Invalid Selection");
        break;
    }
}


int compute_converter(converter *active){
    // print current configuration
    print_converter(active, 1);
    int changes;
    int missing = 0;
    do
    {
        changes = 0; // reset changes counter
        // calculate duty cycle
        if (active->k == -1){ // check if missing
            missing++; // increment missing counter
            if (active->V_o > 0 && active->V_i > 0){ // check if k can be calculated from voltage
                if (!KfromV(active)){
                    // valid k found, update counters, +1 change, -1 missing
                    changes++;
                    missing--;
                    printf("Duty cycle computed as: %.4f\n", active->k);
                }
            }
            else if (active->I_o > 0 && active->I_i > 0) { // check if k can be calculated from current
                if (!KfromI(active)){
                    // valid k found, update counters, +1 change, -1 missing
                    changes++;
                    missing--;
                    printf("Duty cycle computed as: %.4f\n", active->k);
                }
            }
        }
        
        // calculate V_o
        if (active->V_o == -1){ //check if missing
            missing++; // increment missing counter
            // try and calculate output voltage using Ohm's Law
            if (active->I_o > 0 && active->R_l > 0){
                if (!VofromOhm(active)){
                    // V_o calculated, update counters, +1 change, -1 missing
                    changes++;
                    missing--;
                    printf("V_o computed as: %.4f\n", active->V_o);
                }
            }
            // try calculate output voltage using duty ratio
            else if (active->V_i > 0 && active->k > 0){
                if (!VofromK(active)){
                    // V_o calculated, update counters, +1 change, -1 missing
                    changes++;
                    missing--;
                    printf("V_o computed as: %.4f\n", active->V_o);
                }
            }         
            
        }

        // calculate V_i
        if (active->V_i == -1){ // check if missing
            missing++;
            if (active->k > 0 && active->V_o > 0){ // try to calculate from duty cycle
                if (!VifromK(active)){               
                    // update counters, +1 change, -1 missing
                    changes++;
                    missing--;
                    printf("V_i computed as: %.4f\n", active->V_i);
                }
            }
        }

        //calculate I_o
        if (active->I_o == -1){ // check if missing
            missing++; // increment missing counter
            if (active->V_o > 0 && active->R_l > 0){ // try to calculate from Ohm's law
                if (!IofromOhm(active)){
                    // update counters, +1 change, -1 missing
                    changes++;
                    missing--;
                    printf("I_o computed as: %.4f\n", active->I_o);
                }
            }
            else if (active->I_i > 0 && active->k > 0){ // try to calculate from duty cycle
                if (!IofromK(active)){                
                    // update counters, +1 change, -1 missing
                    changes++;
                    missing--;
                    printf("I_o computed as: %.4f\n", active->I_o);
                }
            }
        }

        // calculate I_i
        if (active->I_i == -1){ // check if missing
            missing++; // increment missing counter
            // try to calculate input current using duty ratio
            if (active->k > 0 && active->I_o > 0){
                if (!IifromK(active)){ 
                    // update counters, +1 change, -1 missing
                    changes++;
                    missing--;
                    printf("I_i computed as: %.4f\n", active->I_i);
                }
            }
        }
    // keep looping until no changes are made or missing counter is 0
    } while (missing != 0 && changes != 0);
    // there are still missing values, return an error
    if (missing > 0){
        printf("Value missing");
        return 0;
    }
    else {
        printf("Converter computation successful, moving to component calculations...\n");
    }
    // Compute required converter part values
    // start with required inductor
    if (active->i_rip > 0){ // check current ripple is valid
        if (!calc_L(active)){
            printf("L computed as: %.4eH\n", active->L);
        }
    }
    else if (active->i_rip == 0){ // desirable but physically impossible (requires inf L)
        printf("Achieving 0A of output ripple is not possible, please enter a non-zero value.\n");
        return 0;
    }
    else { return 0; } // other invalid value for i_rip

// calculate extra components for Cuk converter
    if (active->type == Cuk){
         // calculate C_n value
        if (active->v_rip2 > 0){ // check voltage ripple is valid
            if (!calc_Cn(active)){
                printf("C_n computed as %.4e\n", active->C_n);
            }
        }
        else if (active->v_rip2 == 0){ // desirable but physically impossible (requires inf Co)
            printf("Achieving 0V of output ripple is not possible, please enter a non-zero value.\n");
            return 0;
        }
        else { return 0; } // other invalid value for v_rip2

        // calculate second inductor
        if (active->i_rip2 > 0){ // check current ripple is valid
            if (!calc_L2(active)){
                printf("L2 computed as: %.4e\n", active->L2);
            }
        }
        else if (active->i_rip2 == 0){ // desirable but physically impossible (requires inf L)
            printf("Achieving 0A of ripple is not possible, please enter a non-zero value.\n");
            return 0;
        }
        else { return 0; } // other invalid value for i_rip2
    }

    // calculate output capacitor value
    if (active->v_rip > 0){ // check voltage ripple is valid
        if (!calc_Co(active)){        
            printf("C_o computed as %.4e\n", active->C_o);
        }
    }
    else if (active->v_rip == 0){ // desirable but physically impossible (requires inf Co)
        printf("Achieving 0V of output ripple is not possible, please enter a non-zero value.\n");
        return 0;
    }
    else { return 0; } // other invalid value for v_rip
    
    return 1; // computation of converter successful
}