#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "colours.h"
#include "funcs.h"
#include "menu_funcs.h"


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

// compute any missing buck converter parameters and calculate physical component requirements
int compute_buck(converter *active){
    // print current configuration
    print_converter(active, 0);
    int changes; // tracks changes made to converter
    int missing = 0; // tracks number of missing values
    do {
        changes = 0; // reset change counter
        // duty cycle  
        if (active->k == -1){ // check if missing
            missing++; // increment missing counter
            if (active->V_o >= 0 && active->V_i >= 0){ // check if k can be calculated from voltage
                active->k = active->V_o / active->V_i;
                if (active->k > 1 || active->k < 0){ // check calculated k is valid
                    printf(RED"Invalid V_o and V_i values.\nV_o must be less than or equal to V_i\n"RESET);
                    active->k = -1; // reset k
                    return 0; // return an error if k is invalid
                }
                // update counters, +1 change, -1 missing
                changes++;
                missing--;
                printf("Duty cycle computed as: %.4f\n", active->k);
            }
            else if (active->I_o >= 0 && active->I_i >= 0) { // check if k can be calculated from current
                active->k = active->I_i / active->I_o;
                if (active->k > 1 || active->k < 0){ // check calculated k is valid
                    printf(RED"Invalid V_o and V_i values.\nI_i must be less than or equal to I_o\n"RESET);
                    active->k = -1; // reset k
                    return 0; // return an error if k is invalid
                }
                // update counters, +1 change, -1 missing
                changes++;
                missing--;
                printf("Duty cycle computed as: %.4f\n", active->k);
            }
            
        }
        // V_o
        if (active->V_o == -1){ //check if missing
            missing++; // increment missing counter
            if (active->I_o >= 0 && active->R_l >= 0){ // calculate from Ohm's law
                active->V_o = active->I_o * active->R_l; // V = IR
                // update counters, +1 change, -1 missing
                changes++;
                missing--;
                printf("V_o computed as: %.4fV\n", active->V_o);
            }
            else if (active->V_i >= 0 && active->k >= 0){ // calculate from duty cycle
                active->V_o = active->V_i * active->k; // Vo = Vi * k
                // update counters, +1 change, -1 missing
                changes++;
                missing--;
                printf("V_o computed as: %.4f\n", active->V_o);
            }
            
        }
        // V_i
        if (active->V_i == -1){ // check if missing
            missing++; // increment missing counter
            if (active->k >= 0 && active->V_o >= 0){ // calculate from duty cycle
                active->V_i = active->V_o / active->k; // Vi = Vo / k
                // update counters, +1 change, -1 missing
                changes++;
                missing--;
                printf("V_i computed as: %.4f\n", active->V_i);
            }
            
        }
        // I_o
        if (active->I_o == -1){ // check if missing
            missing++; // increment missing counter
            if (active->V_o >= 0 && active->R_l >= 0){ // calculate from Ohm's law
                active->I_o = active->V_o / active->R_l; // Io = Vo / Rl
                // update counters, +1 change, -1 missing
                changes++;
                missing--;
                printf("I_o computed as: %.4f\n", active->I_o);
            }
            else if (active->I_i >= 0 && active->k >= 0){ // calculate from duty cycle
                active->I_o = active->I_i / active->k; // Io = Ii / k
                // update counters, +1 change, -1 missing
                changes++;
                missing--;
                printf("I_o computed as: %.4f\n", active->I_o);
            }
        }
        // I_i
        if (active->I_i == -1){ // check if missing
            missing++; // increment missing counter
            if (active->k >= 0 && active->I_o >= 0){ // calculate from duty cycle
                active->I_i = active->I_o * active->k; // Ii = Io * k
                // update counters, +1 change, -1 missing
                changes++;
                missing--;
                printf("I_i computed as: %.4f\n", active->I_i);
            }
        }
    } while (changes != 0 && missing != 0); // keep looping until no changes are made or missing counter is 0

    if (missing > 0){ // there are still missing values return an error
        
        return 0;
    }
    else {
        printf("Converter computation successful, moving to component calculations...\n");
    }
    // Compute required converter part values
    // start with required inductor
    if (active->i_rip > 0){ // check current ripple is valid
        // L = (1-k)*Vo / (fs*di)
        active->L = (1 - active->k) * active->V_o / (active->F_s * active->i_rip);
        printf("L computed as: %.4eH\n", active->L);
    }
    else if (active->i_rip == 0){ // desirable but physically impossible (requires inf L)
        printf("Achieving 0A of output ripple is not possible, please enter a non-zero value.\n");
        return 0;
    }
    else { return 0; } // other invalid value for i_rip

    // calculate output capacitor value
    if (active->v_rip > 0){ // check voltage ripple is valid
        // Co = k*(Vi-Vo) / (8*Fs^2*dVo*L)
        active->C_o = active->k * (active->V_i - active->V_o) / (8* active->F_s * active->F_s * active->v_rip * active->L);
        printf("C_o computed as %.4e\n", active->C_o);
    }
    else if (active->v_rip == 0){ // desirable but physically impossible (requires inf Co)
        printf("Achieving 0V of output ripple is not possible, please enter a non-zero value.\n");
        return 0;
    }
    else { return 0; } // other invalid value for v_rip

    return 1; // computation of buck converter was successful
}

// compute any missing buck converter parameters and calculate physical component requirements
int compute_boost(converter *active){
    // print current configuration
    print_converter(active,0);
    int changes; // tracks changes made to converter
    int missing = 0; // tracks number of missing values
    do {
        changes = 0; // reset change counter
        // duty cycle  
        if (active->k == -1){ // check if missing
            missing++; // increment missing counter
            // calculate k from voltage     
            if (active->V_o >= 0 && active->V_i >= 0){
                active->k = (active->V_o - active->V_i) / active->V_o;
                if (active->k > 1 || active->k < 0){ // check calculated k is valid
                    printf(RED"Invalid V_o and V_i values.\nV_i must be less than or equal to V_o\n"RESET);
                    active->k = -1; // reset k
                    return 0; // return an error if k is invalid
                }
                // update counters, +1 change, -1 missing
                changes++;
                missing--;
                printf("Duty cycle computed as: %.4f\n", active->k);
            }
            // calculate k from current
            else if (active->I_o >= 0 && active->I_i >= 0) {
                active->k = (active->I_i - active->I_o) / active->I_i;
                if (active->k > 1 || active->k < 0){ // check calculated k is valid
                    printf(RED"Invalid V_o and V_i values.\nV_i must be less than or equal to V_o\n"RESET);
                    active->k = -1; // reset k
                    return 0; // return an error if k is invalid
                }
                // update counters, +1 change, -1 missing
                changes++;
                missing--;
                printf("Duty cycle computed as: %.4f\n", active->k);
            }
        }
        // V_o
        if (active->V_o == -1){ //check if missing
            missing++; // increment missing counter
            // calculate output voltage using Ohm's Law
            if (active->I_o >= 0 && active->R_l >= 0){
                active->V_o = active->I_o * active->R_l; // V = IR
                // update counters, +1 change, -1 missing
                changes++;
                missing--;
                printf("V_o computed as: %.4f\n", active->V_o);
            }
            // calculate output voltage using duty ratio
            else if (active->V_i >= 0 && active->k >= 0){
                active->V_o = active->V_i / (1 - active->k); // Vo = Vi / (1-k)
                // update counters, +1 change, -1 missing
                changes++;
                missing--;
                printf("V_o computed as: %.4f\n", active->V_o);
            }
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
        if (active->I_o == -1){ // check if missing
            missing++; // increment missing counter
            // calculate output current using Ohm's Law
            if (active->V_o >= 0 && active->R_l >= 0){
                active->I_o = active->V_o / active->R_l; // Io = Vo / Rl
                // update counters, +1 change, -1 missing
                changes++;
                missing--;
                printf("I_o computed as: %.4f\n", active->I_o);
            }
            // calculate output current using duty ratio
            else if (active->I_i >= 0 && active->k >= 0){
                active->I_o = active->I_i * (1 - active->k);
                // update counters, +1 change, -1 missing
                changes++;
                missing--;
                printf("I_o computed as: %.4f\n", active->I_o);
            }
        }
        // I_i
        if (active->I_i == -1){ // check if missing
            missing++; // increment missing counter
            // calculate input current using duty ratio
            if (active->k >= 0 && active->I_o >= 0){
                active->I_i = active->I_o / (1 - active->k); //Ii = Io / (1-k)
                // update counters, +1 change, -1 missing
                changes++;
                missing--;
                printf("I_i computed as: %.4f\n", active->I_i);
            }
        }
    // keep looping until no changes are made or missing counter is 0
    } while (changes != 0 && missing != 0);

    if (missing > 0){ // there are still missing values, return an error
        return 0;
    }
    else {
        printf("Converter computation successful, moving to component calculations...\n");
    }
    // Compute required converter part values
    // start with required inductor
    if (active->i_rip > 0){ // check current ripple is valid
        // L = k*Vi / (Fs*dI)
        active->L = active->k * active->V_i / (active->F_s * active->i_rip);
        printf("L computed as: %.4e\n", active->L);
    }
    else if (active->i_rip == 0){ // desirable but physically impossible (requires inf L)
        printf("Achieving 0A of output ripple is not possible, please enter a non-zero value.\n");
        return 0;
    }
    else { return 0; } // other invalid value for i_rip

    // calculate output capacitor value
    if (active->v_rip > 0){ // check voltage ripple is valid
        // Co = Io*k / (dVo*Fs) 
        active->C_o = (active->I_o * active->k) / (active->v_rip * active->F_s);
        printf("C_o computed as %.4e\n", active->C_o);
    }
    else if (active->v_rip == 0){ // desirable but physically impossible (requires inf Co)
        printf("Achieving 0V of output ripple is not possible, please enter a non-zero value.\n");
        return 0;
    }
    else { return 0; }// other invalid value for v_rip

    return 1; // computation of boost converter successful
}

// compute any missing buck-boost converter parameters and calculate physical component requirements
int compute_buckboost(converter *active){
    // print current configuration
    print_converter(active,0);
    int changes; // tracks changes made to converter
    int missing = 0; // tracks number of missing values
    do {
        changes = 0; // reset change counter
        // duty cycle  
        if (active->k == -1){ // check if missing
            missing++; // increment missing counter
            // calculate k from voltage     
            if (active->V_o >= 0 && active->V_i >= 0){ // check if k can be calculated from voltage
                // k = Vo / (Vo + Vi)
                active->k = active->V_o / (active->V_i + active->V_o);
                if (active->k > 1 || active->k < 0){ // check calculated k is valid (shouldn't be a probem with buck-boost)
                    printf(RED"Invalid V_o and V_i values.\n"RESET);
                    active->k = -1; // reset k
                    return 0; // return an error if k is invalid
                }
                // update counters, +1 change, -1 missing
                changes++;
                missing--;
                printf("Duty cycle computed as: %.4f\n", active->k);
            }
            // calculate k from current
            else if (active->I_o >= 0 && active->I_i >= 0) {
                // k = Ii / (Io + Ii)
                active->k = active->I_i / (active->I_o + active->I_i);
                if (active->k > 1 || active->k < 0){ // check calculated k is valid (shouldn't be a probem with buck-boost)
                    printf(RED"Invalid I_o and I_i values.\n"RESET);
                    active->k = -1; // reset k
                    return 0; // return an error if k is invalid
                }
                // update counters, +1 change, -1 missing
                changes++;
                missing--;
                printf("Duty cycle computed as: %.4f\n", active->k);
            }
        }
        // V_o
        if (active->V_o == -1){ //check if missing
            // calculate output voltage using Ohm's Law
            if (active->I_o >= 0 && active->R_l >= 0){
                active->V_o = active->I_o * active->R_l; // V = IR
                // update counters, +1 change, -1 missing
                changes++;
                missing--;
                printf("V_o computed as: %.4f\n", active->V_o);
            }
            // calculate output voltage using duty ratio
            else if (active->V_i >= 0 && active->k >= 0){
                // Vo = k*Vi / (1-k)
                active->V_o = (active->V_i * active->k) / (1 - active->k);
                // update counters, +1 change, -1 missing
                changes++;
                missing--;
                printf("V_o computed as: %.4f\n", active->V_o);
            }
        }
        // V_i
        if (active->V_i == -1){ // check if missing
            missing++; // increment missing counter
            // calculate input voltage using duty ratio
            if (active->k >= 0 && active->V_o >= 0){
                // Vi = Vo*(1-k) / k
                active->V_i = active->V_o * (1 - active->k) / active->k;
                // update counters, +1 change, -1 missing
                changes++;
                missing--;
                printf("V_i computed as: %.4f\n", active->V_i);
            }
        }
        // I_o
        if (active->I_o == -1){ // check if missing
            missing++; // increment missing counter
            // calculate output current using Ohm's Law
            if (active->V_o >= 0 && active->R_l >= 0){
                active->I_o = active->V_o / active->R_l; // Io = Vo / Rl
                // update counters, +1 change, -1 missing
                changes++;
                missing--;
                printf("I_o computed as: %.4f\n", active->I_o);
            }
            // calculate output current using duty ratio
            else if (active->I_i >= 0 && active->k >= 0){
                // Io = Ii*(1-k) / k
                active->I_o = active->I_i * (1 - active->k) / active->k;
                // update counters, +1 change, -1 missing
                changes++;
                missing--;
                printf("I_o computed as: %.4f\n", active->I_o);
            }
        }
        // I_i
        if (active->I_i == -1){ // check if missing
            missing++; // increment missing counter
            // calculate input current using duty ratio
            if (active->k >= 0 && active->I_o >= 0){
                // Ii = Io*k /(1-k)
                active->I_i = (active->I_o * active->k) / (1 - active->k);
                // update counters, +1 change, -1 missing
                changes++;
                missing--;
                printf("I_i computed as: %.4f\n", active->I_i);
            }
        }
    // keep looping until no changes are made or missing counter is 0
    } while (changes != 0 && missing != 0);
    // there are still missing values, return an error
    if (missing > 0){
        return 0;
    }
    else {
        printf("Converter computation successful, moving to component calculations...\n");
    }
    // Compute required converter part values
    // start with required inductor
    if (active->i_rip > 0){ // check current ripple is valid
        // L = Vi*k / (Fs*di)
        active->L = active->k * active->V_i / (active->F_s * active->i_rip);
        printf("L computed as: %.4e\n", active->L);
    }
    else if (active->i_rip == 0){ // desirable but physically impossible (requires inf L)
        printf("Achieving 0A of output ripple is not possible, please enter a non-zero value.\n");
        return 0;
    }
    else { return 0; } // other invalid value for i_rip

    // calculate output capacitor value
    if (active->v_rip > 0){ // check voltage ripple is valid
        // Co = Io*k / dVo*Fs
        active->C_o = (active->I_o * active->k) / (active->v_rip * active->F_s);
        printf("C_o computed as %.4e\n", active->C_o);
    }
    else if (active->v_rip == 0){ // desirable but physically impossible (requires inf Co)
        printf("Achieving 0V of output ripple is not possible, please enter a non-zero value.\n");
        return 0;
    }
    else { return 0; } // other invalid value for v_rip

    return 1; // computation of buck-boost converter was successful
}

// compute any missing cuk converter parameters and calculate physical component requirements
int compute_cuk(converter *active){
    // print current configuration
    print_converter(active,0);
    int changes; // tracks changes made to converter
    int missing = 0; // tracks number of missing values
    do {
        changes = 0; // reset change counter
        // duty cycle  
        if (active->k == -1){ // check if missing
            missing++; // increment missing counter
            // calculate k from voltage     
            if (active->V_o >= 0 && active->V_i >= 0){
                // k = Vo / (Vi+ Vo)
                active->k = active->V_o / (active->V_i + active->V_o);
                if (active->k > 1 || active->k < 0){ // check calculated k is valid
                    printf(RED"Invalid V_o and V_i values.\n"RESET);
                    active->k = -1; // reset k
                    return 0; // return an error if k is invalid
                }
                // update counters, +1 change, -1 missing
                changes++;
                missing--;
                printf("Duty cycle computed as: %.4f\n", active->k);
            }
            // calculate k from current
            else if (active->I_o >= 0 && active->I_i >= 0) {
                // k = Ii / (Io + Ii)
                active->k = active->I_i / (active->I_o + active->I_i);
                if (active->k > 1 || active->k < 0){ // check calculated k is valid
                    printf(RED"Invalid V_o and V_i values.\n"RESET);
                    active->k = -1; // reset k
                    return 0; // return an error if k is invalid
                }
                // update counters, +1 change, -1 missing
                changes++;
                missing--;
                printf("Duty cycle computed as: %.4f\n", active->k);
            }
        }
        // V_o
        if (active->V_o == -1){ //check if missing
            missing++; // increment missing counter
            // calculate output voltage using Ohm's Law
            if (active->I_o >= 0 && active->R_l >= 0){
                active->V_o = active->I_o * active->R_l; // v = IR
                // update counters, +1 change, -1 missing
                changes++;
                missing--;
                printf("V_o computed as: %.4f\n", active->V_o);
            }
            // calculate output voltage using duty ratio
            else if (active->V_i >= 0 && active->k >= 0){
                // Vo = k*Vi / (1-k)
                active->V_o = (active->V_i * active->k) / (1 - active->k);
                changes++;
                printf("V_o computed as: %.4f\n", active->V_o);
            }
        }
        // V_i
        if (active->V_i == -1){ // check if missing
             missing++; // increment missing counter
            // calculate input voltage using duty ratio
            if (active->k >= 0 && active->V_o >= 0){
                // Vi = Vo*(1-k) / k
                active->V_i = active->V_o * (1 - active->k) / active->k;
                // update counters, +1 change, -1 missing
                changes++;
                missing--;
                printf("V_i computed as: %.4f\n", active->V_i);
            }
        }
        // I_o
        if (active->I_o == -1){ // check if missing
            missing++; // increment missing counter
            // calculate output current using Ohm's Law
            if (active->V_o >= 0 && active->R_l >= 0){
                // Io = Vo / Rl
                active->I_o = active->V_o / active->R_l;
                // update counters, +1 change, -1 missing
                changes++;
                missing--;
                printf("I_o computed as: %.4f\n", active->I_o);
            }
            // calculate output current using duty ratio
            else if (active->I_i >= 0 && active->k >= 0){
                // Io = Ii*(1-k) / k
                active->I_o = active->I_i * (1 - active->k) / active->k;
                // update counters, +1 change, -1 missing
                changes++;
                missing--;
                printf("I_o computed as: %.4f\n", active->I_o);
            }
        }
        // I_i
        if (active->I_i == -1){ // check if missing
            missing++; // increment missing counter
            // calculate input current using duty ratio
            if (active->k >= 0 && active->I_o >= 0){
                // Ii = Io*k /(1-k)
                active->I_i = (active->I_o * active->k) / (1 - active->k);
                // update counters, +1 change, -1 missing
                changes++;
                missing--;
                printf("I_i computed as: %.4f\n", active->I_i);
            }
        }
    // keep looping until no changes are made or missing counter is 0
    } while (changes != 0 && missing != 0);
    // there are still missing values, return an error
    if (missing > 0){
        return 0;
    }
    else {
        printf("Converter computation successful, moving to component calculations...\n");
    }
    // Compute required converter part values
    // start with first inductor
    if (active->i_rip > 0){ // check current ripple is valid
        // L (1) = k*Vi / (Fs*dI)
        active->L = active->k * active->V_i / (active->F_s * active->i_rip);
        printf("L computed as: %.4e\n", active->L);
    }
    else if (active->i_rip == 0){ // desirable but physically impossible (requires inf L)
        printf("Achieving 0A of ripple is not possible, please enter a non-zero value.\n");
        return 0;
    }
    else { return 0; } // other invalid value for i_rip

    // calculate second inductor
    if (active->i_rip2 > 0){ // check current ripple is valid
        // L2 = k*Vi / (Fs*dI2)
        active->L2 = active->k * active->V_i / (active->F_s * active->i_rip2);
        printf("L2 computed as: %.4e\n", active->L2);
    }
    else if (active->i_rip2 == 0){ // desirable but physically impossible (requires inf L)
        printf("Achieving 0A of ripple is not possible, please enter a non-zero value.\n");
        return 0;
    }
    else { return 0; } // other invalid value for i_rip2

    // calculate output capacitor value
    if (active->v_rip > 0){ // check voltage ripple is valid
        // Co = Vi*k / (8*Fs^2*dVo*L2)
        active->C_o = (active->V_i * active->k) / (8 * active->F_s * active->F_s * active->v_rip * active->L2);
        printf("C_o computed as %.4e\n", active->C_o);
    }
    else if (active->v_rip == 0){ // desirable but physically impossible (requires inf Co)
        printf("Achieving 0V of input ripple is not possible, please enter a non-zero value.\n");
        return 0;
    }
    else { return 0; } // other invalid value for v_rip

    // calculate C_n value
    if (active->v_rip2 > 0){ // check voltage ripple is valid
        // Cn = Io*k / (Fs*dV2)
        active->C_n = (active->I_o * active->k) / (active->F_s * active->v_rip2);
        printf("C_n computed as %.4e\n", active->C_n);
    }
    else if (active->v_rip2 == 0){ // desirable but physically impossible (requires inf Co)
        printf("Achieving 0V of output ripple is not possible, please enter a non-zero value.\n");
        return 0;
    }
    else { return 0; } // other invalid value for v_rip2

    return 1; // computation of cuk converter was successful
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
            if (active->V_o >= 0 && active->V_i >= 0){ // check if k can be calculated from voltage
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
                    return 0;
                }
                if (active->k > 1 || active->k < 0){ // check calculated k is valid
                    printf(RED"Duty cycle calculation failed.\nInvalid V_o and V_i values.\n"RESET);
                    active->k = -1; // reset k
                    return 0; // return an error if k is invalid
                }
                // valid k found, update counters, +1 change, -1 missing
                changes++;
                missing--;
                printf("Duty cycle computed as: %.4f\n", active->k);
            }
            else if (active->I_o >= 0 && active->I_i >= 0) { // check if k can be calculated from current
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
                    return 0;
                }
                if (active->k > 1 || active->k < 0){ // check calculated k is valid
                    printf(RED"Duty cycle calculation failed.\nInvalid I_o and I_i values.\n"RESET);
                    active->k = -1; // reset k
                    return 0; // return an error if k is invalid
                }
                // valid k found, update counters, +1 change, -1 missing
                changes++;
                missing--;
                printf("Duty cycle computed as: %.4f\n", active->k);
            }
        }
        
        // calculate V_o
        if (active->V_o == -1){ //check if missing
            missing++; // increment missing counter
            // try and calculate output voltage using Ohm's Law
            if (active->I_o >= 0 && active->R_l >= 0){
                // no switch-case, same for all converter types
                active->V_o = active->I_o * active->R_l; // v = IR
                // V_o calculated, update counters, +1 change, -1 missing
                changes++;
                missing--;
                printf("V_o computed as: %.4f\n", active->V_o);
            }
            // calculate output voltage using duty ratio
            else if (active->V_i >= 0 && active->k >= 0){
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
                    return 0;
                }
                // V_o calculated, update counters, +1 change, -1 missing
                changes++;
                missing--;
                printf("V_o computed as: %.4f\n", active->V_o);
            }         
            
        }

        // calculate V_i
        if (active->V_i == -1){ // check if missing
            missing++;
            if (active->k >= 0 && active->V_o >= 0){ // calculate from duty cycle
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
                    return 0;
                }
                
                // update counters, +1 change, -1 missing
                changes++;
                missing--;
                printf("V_i computed as: %.4f\n", active->V_i);
            }
        }

        //calculate I_o
        if (active->I_o == -1){ // check if missing
            missing++; // increment missing counter
            if (active->V_o >= 0 && active->R_l >= 0){ // calculate from Ohm's law
                // no switch-case,same for all converter types
                active->I_o = active->V_o / active->R_l; // Io = Vo / Rl
                // update counters, +1 change, -1 missing
                changes++;
                missing--;
                printf("I_o computed as: %.4f\n", active->I_o);
            }
            else if (active->I_i >= 0 && active->k >= 0){ // calculate from duty cycle
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
                    return 0;
                }
                
                // update counters, +1 change, -1 missing
                changes++;
                missing--;
                printf("I_o computed as: %.4f\n", active->I_o);
            }
        }

        // calculate I_i
        if (active->I_i == -1){ // check if missing
            missing++; // increment missing counter
            // calculate input current using duty ratio
            if (active->k >= 0 && active->I_o >= 0){
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
                    return 0;
                }                
                // update counters, +1 change, -1 missing
                changes++;
                missing--;
                printf("I_i computed as: %.4f\n", active->I_i);
            }
        }
    // keep looping until no changes are made or missing counter is 0
    } while (missing != 0 && changes != 0);
    // there are still missing values, return an error
    if (missing > 0){
        return 0;
    }
    else {
        printf("Converter computation successful, moving to component calculations...\n");
    }
    // Compute required converter part values
    // start with required inductor
    if (active->i_rip > 0){ // check current ripple is valid
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
                return 0;
            } 
        printf("L computed as: %.4eH\n", active->L);
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
            // Cn = Io*k / (Fs*dV2)
            active->C_n = (active->I_o * active->k) / (active->F_s * active->v_rip2);
            printf("C_n computed as %.4e\n", active->C_n);
        }
        else if (active->v_rip2 == 0){ // desirable but physically impossible (requires inf Co)
            printf("Achieving 0V of output ripple is not possible, please enter a non-zero value.\n");
            return 0;
        }
        else { return 0; } // other invalid value for v_rip2

        // calculate second inductor
        if (active->i_rip2 > 0){ // check current ripple is valid
            // L2 = k*Vi / (Fs*dI2)
            active->L2 = active->k * active->V_i / (active->F_s * active->i_rip2);
            printf("L2 computed as: %.4e\n", active->L2);
        }
        else if (active->i_rip2 == 0){ // desirable but physically impossible (requires inf L)
            printf("Achieving 0A of ripple is not possible, please enter a non-zero value.\n");
            return 0;
        }
        else { return 0; } // other invalid value for i_rip2
    }

    // calculate output capacitor value
    if (active->v_rip > 0){ // check voltage ripple is valid
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
                return 0;
            } 
        
        printf("C_o computed as %.4e\n", active->C_o);
    }
    else if (active->v_rip == 0){ // desirable but physically impossible (requires inf Co)
        printf("Achieving 0V of output ripple is not possible, please enter a non-zero value.\n");
        return 0;
    }
    else { return 0; } // other invalid value for v_rip
    
    return 1; // computation of converter successful
}