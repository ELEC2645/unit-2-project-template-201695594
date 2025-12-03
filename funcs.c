#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "colours.h"
#include "funcs.h"



const char *input_fields[] = {"V_o\0", "V_i\0", "I_o\0", "I_i\0", "R_l\0", "F_s\0", "d_i\0", "d_v\0", "d_i2\0", "d_v2\0"};



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
        printf("\nV_o changed to: %.4f", active->V_o);
        break;
    case V_I:
        printf("\nEnter a new V_i value: ");
        active->V_i = get_float_input();
        printf("\nV_i changed to: %.4f", active->V_i);
        break;
    case I_O:
        printf("\nEnter a new I_o value: ");
        active->I_o = get_float_input();
        printf("\nI_o changed to: %.4f", active->I_o);
        break;
    case I_I:
        printf("\nEnter a new I_i value: ");
        active->I_i = get_float_input();
        printf("\nI_i changed to: %.4f", active->I_i);
        break;
    case R_L:
        printf("\nEnter a new R_l value: ");
        active->R_l = get_float_input();
        printf("\nR_l changed to: %.4f", active->R_l);
        break;
    case F_S:
        printf("\nEnter a new F_s value: ");
        active->F_s = get_float_input();
        printf("\nF_s changed to: %.4f", active->F_s);
        break;
    case D_I:
        printf("\nEnter a new delta_i value: ");
        active->i_rip = get_float_input();
        printf("\ndelta_i changed to: %.4f", active->i_rip);
        break;
    case D_V:
        printf("\nEnter a new delta_v value: ");
        active->v_rip = get_float_input();
        printf("\ndelta_v changed to: %.4f", active->v_rip);
        break;
    case D_I2:
        if (active->type > 2){
            printf("\nEnter a new delta_i2 value: ");
            active->i_rip2 = get_float_input();
            printf("\ndelta_i2 changed to: %.4f", active->i_rip2);
            break;
        } else {active->i_rip2 = -2; break;}
    case D_V2:
        if (active->type > 2){
            printf("\nEnter a new delta_v2 value: ");
            active->v_rip2 = get_float_input();
            printf("\ndelta_v2 changed to: %.4f", active->v_rip2);
            break;
        } else {active->v_rip2 = -2; break;} 
    default:
        printf("Invalid Selection");
        break;
    }
}