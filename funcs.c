#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcs.h"
#include "converter.h"

void menu_item_1(void) {
    int fields = 0;
    printf("\n>> New Buck Converter\n");
    printf("\nPlease enter the prompted values for your buck converter:\n");
    /* you can call a function from here that handles menu 1 */
    do {
        printf("V_out:");
        float val = get_float_input();
        if (val < 0){
            printf("Invalid input, please enter a float");
        }
        else {
            // save v_out
            fields++
        }
    } while fields < 5;


}

void menu_item_2(void) {
    printf("\n>> Menu 2\n");
    printf("\nSome code here does something useful\n");
    /* you can call a function from here that handles menu 2 */
}

void menu_item_3(void) {
    printf("\n>> Menu 3\n");
    printf("\nSome code here does something useful\n");
    /* you can call a function from here that handles menu 3 */
}

void menu_item_4(void) {
    printf("\n>> Menu 4\n");
    printf("\nSome code here does something useful\n");
    /* you can call a function from here that handles menu 4 */
}

void menu_item_5(void) {
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
        printf("Conversion to float failed");
        return -1;
    }
    else {return result;}
}