#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "funcs.h"
#include "menu_funcs.h"
#include "files.h"

#include "colours.h"

// used for easily printing converter type
const char conv_types[][16] = {"Buck", "Boost", "Buck-Boost", "Cuk"};

// used by the user to set a name and type for the currently active converter design
void menu_item_1(converter *active) {
    printf(RED"\n>> Setup New Converter\n"RESET);
    char name_buf[128]; // read in buffer
    printf("\nPlease enter a filename for your new converter design: ");
    if (!fgets(name_buf, sizeof(name_buf), stdin)) { // <- here is where the string is read in
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
    printf(YELLOW"Design saved under: %s\n\n", active->name);
    // print menu of converter types
    printf(GREEN"Please select converter type:\n"RESET
        "1. Buck\n"
        "2. Boost\n"
        "3. Buck-Boost\n"
        "4. Cuk\n"
        );
    printf("Select type (1-4): ");
    // set unknown component values
    active->k = -1;
    active->C_o = -1;
    active->L = -1;    
    // assume additional parts for Cuk converter are not required
    active->C_n = -2;
    active->L2 = -2;
    // get an int input between 1 and 4
    int check_val;
    int param;
    do {
        check_val = get_int_input(&param);
        if (check_val == 0 && param <= 4){
            // edit type
            
            strncpy(active->s_type, conv_types[param -1], sizeof(active->s_type)-1);
            active->s_type[sizeof(active->s_type)-1] = '\0';
            
            active->type = param; // set type flag
            printf("Selected type: "MAGENTA "%s\n"RESET, active->s_type);
            if (param == 4){ // set addtional cuk components to unknown
                active->L2 = -1;
                active->C_n = -1;
            }
        }
        else if (check_val == 1 || param > 4){ // error msg if get_int_intput failed or param is out of scope
            printf(RED"Invalid menu selection" RESET "\nPlease select a converter type: ");
            check_val = 1;
        }
    } while (check_val != 0); // re-prompt until successful type selection
}

// called to sequentially set all of the converter's initial parameters
void menu_item_2(converter *active) {
    printf(YELLOW"\n>> New %s Converter\n"RESET, active->s_type);
    printf("\nPlease enter the prompted values for your  converter\nLeave unknown values blank\n");
    // edit each converter input field individually
    for (int i = 0; i < 10; i++){
        edit_param(active, i); // edit ith param
    }
    // print inputted specs
    printf("\n Entered converter specs:\n");
    print_converter(active, 0);
}

// called to select and edit design indiviual params
void menu_item_3(converter *active) {
    printf(GREEN"\n>> Edit design parameters\n"RESET);
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
            edit_param(active, param - 1);
        }
        else if (check_val == 1 || param > 10){
            printf("Invalid menu selection\nPlease select a field to edit: ");
        }
        else { // check_val == 2, user cancelled edit
            printf("Edit cancelled");
            break;
        }
    } while (check_val != 0);
    
    
    
}

// computes converter components and missing values
void menu_item_4(converter *active) {
    int solved; // checks if converter computed successfully
    printf(CYAN"\n>> Design computation\n"RESET);
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
    if (!solved) { // compute functions fail if the covnerter needs more information
        printf("\nInsufficent input parameters, please provide more information\n");
        printf("Redirecting to edit parameters menu...\n");
        menu_item_3(active);
    }
    else {
        print_converter(active, 1);
    }
}

void menu_item_5(converter *active) {
    printf(MAGENTA"\n>> Load/Save Menu\n\n"RESET);
    printf("Please select a file operation:\n");
    printf("\t1. Save design\n");
    printf("\t2. Load design\n");
    printf("Enter option: ");
    int check_val;
    int option;
    check_val = get_int_input(&option);
    if (check_val == 0){
        switch (option)
        {
        case 1:
            /* code */
            // ensure current design has a specified name
            if (*active->name == '\0') {printf("Please set design name\n"); break;}
            printf("Saving current design to /saves/%s.json\n", active->name);
            char path[128] = "./saves/"; // string for save file path
            strcat(path, active->name); // add filename
            strcat(path, ".json"); // append .json suffix
            write_converter(path, active);
            printf(YELLOW"Design saved successfully\n");
            break;
        
        case 2:
            printf("Currently saved designs:\n"YELLOW);
            print_saves();
            printf(RESET"Enter the save file to load: ");
            char buf[64];
            char lpath[128] = "./saves/";
            if (!fgets(buf, sizeof(buf), stdin)) { // <- here is where the string is read in
                /* EOF or error; bail out gracefully */
                puts("\nInput error. Exiting.");
                exit(1);
            }
            // strip end of line
            buf[strcspn(buf, "\r\n")] = '\0';
            // strip invalid characters
            buf[strcspn(buf, " |?!/:;,()$.")] = 0;
            buf[sizeof(buf)-1] = '\0';
            printf("Input: %s\n", buf);
            // join buf to path and load file
            strcat(lpath, buf);
            strcat(lpath, ".json");
            printf("Loading file at path: %s\n", lpath);
            if (read_converter(lpath, active)){
                printf("File opening failed\n");
                break;
            }
            print_converter(active, 0);
            break;
        
        default:
            printf("Invalid input\n");
            break;
        }
    }
    else {
        printf("Invalid input\n");
    }
}

// prints values of the fields of the passed converter struct
void print_converter(converter *active, int flag) {
    printf(RED"\n\t\tName: %s", active->name);
    printf("\n---------------------------------------------------------");
    printf(RESET"\t\nType: %s", active->s_type);
    printf("\n---------------------------------------------------------");
    printf("\n|1.\t V_o: ");print_float(active->V_o, 0);printf("\t\t 2.    \tV_i: ");print_float(active->V_i, 0);printf("\t|");
    printf("\n|3.\t I_o: ");print_float(active->I_o, 0);printf("\t\t 4.    \tI_i: ");print_float(active->I_o, 0);printf("\t|");
    printf("\n|5.\t R_l: ");print_float(active->R_l, 0);printf("\t\t 6.    \tF_s: ");print_float(active->F_s,0);printf("\t|");
    printf("\n|7.  delta_i: ");print_float(active->i_rip, 0);printf("\t\t 8. delta_v: ");print_float(active->v_rip, 0);printf("\t|");
    if (active->type == 4){
    printf("\n|9. delta_i2: ");print_float(active->i_rip2, 0);printf("\t\t10.delta_v2: ");print_float(active->v_rip2, 0);printf("\t|");
    }
    printf("\n---------------------------------------------------------\n");
    if (flag){
        // print extra values
        printf(RED"\t\tConverter Components \n"RESET);
        printf("\t Primary Inductor (L): ");print_float(active->L, 1);
        printf(" H\n \tOutput Capacitor (C_o): ");print_float(active->C_o, 1);
        if (active->type == 4){
            printf(" F\n \tSecondary Inductor (L2): ");print_float(active->L2, 1);
            printf(" H\n \tTransfer Capacitor (C_n): ");print_float(active->C_n, 1);
        }
        printf(" F\n---------------------------------------------------------\n");
    }
    printf(RED"\n---------------------------------------------------------\n"RESET);
}
