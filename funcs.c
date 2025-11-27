#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
    do {
        printf("Please select converter type:\n"
            "1. Buck\n"
            "2. Boost\n"
            );
    if (!fgets(buf, sizeof(buf), stdin)) {
            /* EOF or error; bail out gracefully */
            puts("\nInput error. Exiting.");
            exit(1);
        }
    // strip trailing newline
    buf[strcspn(buf, "\r\n")] = '\0';
    if (buf[0] == '1' || buf[0] == '2'){
        active->type = (int)strtol(buf, NULL, 10);
    }
    else {
        printf("Invalid input\n");
        active->type = 0;
    }
    } while (!active->type);

}

void menu_item_2(converter *active) {
    int fields = 0;
    float vals[9] = { 0 };
    printf("\n>> New %s Converter\n", conv_types[active->type - 1]);
    printf("\nPlease enter the prompted values for your  converter\nLeave unknown values blank\n");
    do {
        printf("%s: ", input_fields[fields]);
        float val = get_float_input();
        if (val < -1){
            printf("Invalid input, please enter a positve float\n");
        }
        else {
            // save v_out
            vals[fields] = val;
            fields++;
        }
    } while (fields < sizeof(input_fields)/ sizeof(input_fields[0]));
    active->V_o = vals[0];
    active->V_i = vals[1];
    active->I_o = vals[2];
    active->I_i = vals[3];
    active->R_l = vals[4];
    active->F_s = vals[5];
    active->i_rip = vals[5];
    active->v_rip = vals[6];
    active->i_rip2 = vals[7];
    active->v_rip2 = vals[8];
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
    printf("Enter new value for %s", input_fields[param]);
    switch (param)
    {
    case V_O:
        printf("\nEnter a new V_o value: ");
        active->V_o = get_float_input();
        printf("\nV_o changed to: %.4f", active->V_o);
        break;
    case V_I:
        printf("\nEnter a new V_i value: ");
        active->V_i = get_float_input();
        printf("\nV_o changed to: %.4f", active->V_i);
        break;
    case I_O:
        printf("\nEnter a new I_o value: ");
        active->I_o = get_float_input();
        printf("\nV_o changed to: %.4f", active->I_o);
        break;
    case I_I:
        printf("\nEnter a new I_i value: ");
        active->I_i = get_float_input();
        printf("\nV_o changed to: %.4f", active->I_i);
        break;
    case R_L:
        printf("\nEnter a new R_l value: ");
        active->R_l = get_float_input();
        printf("\nV_o changed to: %.4f", active->R_l);
        break;
    case F_S:
        printf("\nEnter a new V_o value: ");
        active->F_s = get_float_input();
        printf("\nV_o changed to: %.4f", active->F_s);
        break;
    case D_I:
        printf("\nEnter a new delta_i value: ");
        active->i_rip = get_float_input();
        printf("\nV_o changed to: %.4f", active->i_rip);
        break;
    case D_V:
        printf("\nEnter a new delta_v value: ");
        active->v_rip = get_float_input();
        printf("\nV_o changed to: %.4f", active->v_rip);
        break;
    case D_I2:
        printf("\nEnter a new delta_i2 value: ");
        active->i_rip2 = get_float_input();
        printf("\nV_o changed to: %.4f", active->i_rip2);
        break;
    case D_V2:
        printf("\nEnter a new delta_v2 value: ");
        active->v_rip2 = get_float_input();
        printf("\nV_o changed to: %.4f", active->v_rip2);
        break;    
    default:
        printf("Invalid Selection");
        break;
    }
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

