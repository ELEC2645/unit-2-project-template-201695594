#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcs.h"

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

void menu_item_3(converter *active) {
    printf("\n>> Menu 3\n");
    printf("\nSome code here does something useful\n");
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
    if (!fgets(buf, sizeof(buf), stdin)) {
        /* EOF or error; bail out gracefully */
        puts("\nInput error. Exiting.");
        exit(1);
    }
    // strip trailing newline
    buf[strcspn(buf, "\r\n")] = '\0';
    result = strtof(buf, &conv_check); // try to convert input to float
    if (*conv_check != '\0'){ // if returned end value is not '\0' then conversion failed
        printf("\nConversion to float failed\n");
        return -2;
    }
    else if (result < 0){ // negative inputs are not allowed
        result = -3;
        return result;
    }
    else if (buf[0] == '\0') { // if a blank line was submitted
        result = -1;
        return result;
    }
    else {return result;}
}
void print_converter(converter *active) {
    printf("\nName: %s Type: %s", active->name, conv_types[active->type - 1]);
    printf("\nV_o: %.4f V_i: %.4f", active->V_o, active->V_i);
    printf("\nI_o: %.4f I_i: %.4f", active->I_o, active->I_i);
    printf("\nR_l: %.4f F_s: %.4f", active->R_l, active->F_s);
    printf("\ndelta_i: %.4f delta_v: %.4f", active->i_rip,active->v_rip);
    printf("\ndelta_i2: %.4f delta_v2: %.4f\n", active->i_rip2, active->v_rip2);
}