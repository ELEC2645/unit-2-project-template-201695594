#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "colours.h"
#include "funcs.h"

enum conv_fields {V_O, V_I, I_O, I_I, R_L, F_S, D_I, D_V, D_I2 ,D_V2, L, C_O, L2, C_N};

const char input_fields[][6] = {{"V_o\0"}, {"V_i\0"}, {"I_o\0"}, {"I_i\0"}, {"R_l\0"}, {"F_s\0"}, {"d_i\0"}, {"d_v\0"}, {"d_i2\0"}, {"d_v2\0"}};
const char conv_types[][6] = {{"Buck\0"}, {"Boost\0"}};

void menu_item_1(converter *active) {
    char name_buf[128];
    char buf[2];
    printf("\nPlease enter a filename for your new converter design: ");
    if (!fgets(name_buf, sizeof(name_buf), stdin)) {
            /* EOF or error; bail out gracefully */
            puts("\nInput error. Exiting.");
            exit(1);
        }
    name_buf[strcspn(name_buf, "\r\n")] = '\0';
    name_buf[strcspn(name_buf, " |?!/:;,()$.")] = 0;
    printf("1\n");
    strncpy(active->name, name_buf, sizeof(active->name)-1);
    printf("1\n");
    active->name[sizeof(active->name)-1] = '\0';
    printf("Design saved under: %s\n", active->name);
    printf(u8"Please select converter type:\n"
        RED"1. Buck\n"
        GREEN"2. Boost\n"
        BLUE"3. Buck-Boost\n"
        YELLOW"4. Cuk\n"RESET
        );
    active->type = get_int_input(4);
}

void menu_item_2(converter *active) {
    printf("\n>> New %s Converter\n", conv_types[active->type - 1]);
    printf("\nPlease enter the prompted values for your  converter\nLeave unknown values blank\n");
    for (int i = 0; i < sizeof(input_fields)/ sizeof(input_fields[0]); i++){
        edit_param(active, i);
    }
    printf("\n Entered converter specs:\n");
    print_converter(active);
}

// edit design params
void menu_item_3(converter *active) {
    printf("\n>> Edit design parameters\n");
    printf("\nEnter the field you wish to edit\n");
    print_converter(active);
    printf("Enter option:");
    int param = get_int_input(sizeof(input_fields)/ sizeof(input_fields[0])) - 1;
    edit_param(active, param);
    
    
    /* you can call a function from here that handles menu 3 */
}

void menu_item_4(converter *active) {
    printf("\n>> Menu 4\n");
    printf("\nSome code here does something useful\n");
    /* you can call a function from here that handles menu 4 */
}

void menu_item_5(converter *active) {
    printf("\n>> Menu 4\n");
    printf("\nSome code here does something useful\n");
    /* you can call a function from here that handles menu 4 */
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

void print_converter(converter *active) {
    printf("\nName: %s Type: %s", active->name, conv_types[active->type - 1]);
    printf("\n1. V_o: %.4f 2. V_i: %.4f", active->V_o, active->V_i);
    printf("\n3. I_o: %.4f 4. I_i: %.4f", active->I_o, active->I_i);
    printf("\n5. R_l: %.4f 6. F_s: %.4f", active->R_l, active->F_s);
    printf("\n7. delta_i: %.4f 8. delta_v: %.4f", active->i_rip,active->v_rip);
    printf("\n9. delta_i2: %.4f 10. delta_v2: %.4f\n", active->i_rip2, active->v_rip2);
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

int get_int_input(int max) {
    char buf[128];
    int valid_input = 0;
    int value = 0;
    do {
        if (!fgets(buf, sizeof(buf), stdin)) {
            /* EOF or error; bail out gracefully */
            puts("\nInput error. Exiting.");
            exit(1);
        }

        // strip trailing newline
        buf[strcspn(buf, "\r\n")] = '\0';

        if (!is_integer(buf)) {
            printf("Enter an integer!\n");
            valid_input = 0;
            
        } else {
            value = (int)strtol(buf, NULL, 10);
            if (value >= 1 && value <= max) {
                valid_input = 1;
            } else {
                printf("Invalid input\n");
                valid_input = 0;
            }
        }
    } while (!valid_input);

    return value;
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