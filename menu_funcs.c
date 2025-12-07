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
    // print menu of converter types
    printf("Please select converter type:\n"
        RED"1. Buck\n"
        GREEN"2. Boost\n"
        BLUE"3. Buck-Boost\n"
        YELLOW"4. Cuk\n"RESET
        );
    // set unknown component values
    active->k = -1;
    active->C_o = -1;
    active->L = -1;
    // assume addition parts for Cuk converter are not required
    active->C_n = -2;
    active->L2 = -2;
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
    print_converter(active, 0);
}

// called to select and edit design indiviual params
void menu_item_3(converter *active) {
    printf("\n>> Edit design parameters\n");
    printf("\nEnter the field you wish to edit\n");
    // print current converter fields
    print_converter(active, 0);
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
    int solved;
    printf("\n>> Design computation\n");
    switch (active->type)
    {
    case 1: // buck  converter
        solved = compute_buck(active);
        break;
    case 2: // boost converter
        solved = compute_boost(active);
        break;
    case 3: // buck-boost converter
        solved = compute_buckboost(active);
        break;
    case 4: // Cuk converter
        solved = compute_cuk(active);
        break;
    default:
        break;
    }
    if (!solved) {
        printf("\nInsufficent input parameters, please provide more information\n");
        printf("Redirecting to edit parameters menu...\n");
        menu_item_3(active);
    }
}

void menu_item_5(converter *active) {
    printf("\n>> Menu 4\n");
    printf("\nSome code here does something useful\n");
    /* you can call a function from here that handles menu 4 */
}

// prints values of the fields of the passed converter struct
void print_converter(converter *active, int flag) {
    printf(RED"\n\t\tName: %s", active->name);
    printf("\n---------------------------------------------------------");
    printf(RESET"\t\nType: %s", conv_types[active->type - 1]);
    printf("\n---------------------------------------------------------");
    printf("\n|1.\t V_o: ");print_float(active->V_o);printf("\t\t 2.    \tV_i: ");print_float(active->V_i);printf("\t|");
    printf("\n|3.\t I_o: ");print_float(active->I_o);printf("\t\t 4.    \tI_i: ");print_float(active->I_o);printf("\t|");
    printf("\n|5.\t R_l: ");print_float(active->R_l);printf("\t\t 6.    \tF_s: ");print_float(active->F_s);printf("\t|");
    printf("\n|7.  delta_i: ");print_float(active->i_rip);printf("\t\t 8. delta_v: ");print_float(active->v_rip);printf("\t|");
    if (active->type == 4){
    printf("\n|9. delta_i2: ");print_float(active->i_rip2);printf("\t\t10.delta_v2: ");print_float(active->v_rip2);printf("\t|");
    }
    printf("\n---------------------------------------------------------\n");
    if (flag){
        // print extra values
        printf("\n Converter Design \n");
        printf("\n Inductor value: %.4e", active->L);
        printf("\n Capacitor value: %.4e", active->C_o);
    }
}
