#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "funcs.h"
#include "menu_funcs.h"

#include "colours.h"


const char conv_types[][16] = {"Buck", "Boost", "Buck-Boost", "Cuk"};

// used by the user to set a name and type for the currently active converter design
void menu_item_1(converter *active) {
    char name_buf[128]; // read in buffer
    printf("\nPlease enter a filename for your new converter design: ");
    if (!fgets(name_buf, sizeof(name_buf), stdin)) {
            /* EOF or error; bail out gracefully */
            puts("\nInput error. Exiting.");
            exit(1);
        }
    // strip end of line
    name_buf[strcspn(name_buf, "\r\n")] = '\0';
    // strip invalid characters
    name_buf[strcspn(name_buf, " |?!/:;,()$.")] = 0;
    // copy santised input to converter.name 
    strncpy(active->name, name_buf, sizeof(active->name)-1);
    // ensure format is a string
    active->name[sizeof(active->name)-1] = '\0';
    // confirm name selected
    printf("Design saved under: %s\n", active->name);
    // print menyu of converter types
    printf("Please select converter type:\n"
        RED"1. Buck\n"
        GREEN"2. Boost\n"
        BLUE"3. Buck-Boost\n"
        YELLOW"4. Cuk\n"RESET
        );
    // get an int input between 1 and 4
    int check_val;
    int param;
    do {
        check_val = get_int_input(&param);
        if (check_val == 0 && param <= 4){
            // edit selected parameter
            active->type = param;
            printf("Selected type: "GREEN "%s", conv_types[active->type - 1]);
            printf(RESET);
        }
        else if (check_val == 1 || param > 4){
            printf(RED"Invalid menu selection" RESET "\nPlease select a converter type: ");
            check_val = 1;
        }
        else {
            printf("Cancelling edit");
            break;
        }
    } while (check_val != 0);
}

// called to sequentially set all of the converter's initial parameters
void menu_item_2(converter *active) {
    printf("\n>> New %s Converter\n", conv_types[active->type - 1]);
    printf("\nPlease enter the prompted values for your  converter\nLeave unknown values blank\n");
    // edit each converter input field individually
    for (int i = 0; i < 10; i++){
        edit_param(active, i);
    }
    printf("\n Entered converter specs:\n");
    print_converter(active);
}

// called to select and edit design indiviual params
void menu_item_3(converter *active) {
    printf("\n>> Edit design parameters\n");
    printf("\nEnter the field you wish to edit\n");
    // print current converter fields
    print_converter(active);
    // get a field to edit
    printf("Enter option (enter 'C' or 'c' to cancel): ");
    int check_val;
    int param;
    do {
        check_val = get_int_input(&param);
        if (check_val == 0 && param <= 10){
            // edit selected parameter
            edit_param(active, param);
        }
        else if (check_val == 1 || param > 10){
            printf("Invalid menu selection\nPlease select a field to edit: ");
        }
        else {
            printf("Edit cancelled");
            break;
        }
    } while (check_val != 0);
    
    
    
}

// computes converter components and missing values
void menu_item_4(converter *active) {
    printf("\n>> Design computation\n");
    switch (active->type)
    {
    case 0: // buck  converter
        compute_buck(active);
        break;
    case 1: // boost converter
        compute_boost(active);
        break;
    case 2: // buck-boost converter
        compute_buckboost(active);
        break;
    case 3: // Cuk converter
        compute_cuk(active);
        break;
    default:
        break;
    }
}

void menu_item_5(converter *active) {
    printf("\n>> Menu 4\n");
    printf("\nSome code here does something useful\n");
    /* you can call a function from here that handles menu 4 */
}

// prints values of the fields of the passed converter struct
void print_converter(converter *active) {
    printf(RED"\n\t\tName: %s", active->name);
    printf("\n--------------------------------------------");
    printf(RESET"\t\nType: %s", conv_types[active->type - 1]);
    printf("\n--------------------------------------------");
    printf("\n|1.\t V_o: %.4f   2. \tV_i: %.4f|", active->V_o, active->V_i);
    printf("\n|3.\t I_o: %.4f   4. \tI_i: %.4f|", active->I_o, active->I_i);
    printf("\n|5.\t R_l: %.4f   6. \tF_s: %.4f|", active->R_l, active->F_s);
    printf("\n|7.  delta_i: %.4f   8.   delta_v: %.4f|", active->i_rip,active->v_rip);
    printf("\n|9. delta_i2: %.4f  10.  delta_v2: %.4f|", active->i_rip2, active->v_rip2);
    printf("\n--------------------------------------------\n\n");
}
