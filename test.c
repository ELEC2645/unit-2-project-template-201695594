/*TESTING FILE*/
#include "funcs.h"
#include "menu_funcs.h"
#include "colours.h"

#include "stdio.h"


converter test = {
    .name = "TestConverter",
    .type = 1,
    .V_o = 50,
    .V_i = 100,
    .I_o = -1,
    .I_i = -1,
    .F_s = 10e3,
    .R_l = 300,
    .i_rip = 0.05,
    .v_rip = 1,
    .i_rip2 = -2,
    .v_rip2 = -2,
    .k = -1,
    .C_n = -2,
    .C_o = -1,
    .L = -1,
    .L2 = -2,
};

void main(void){
    printf("Current test converter:\n");
    print_converter(&test, 0);
    printf("Computing as boost converter\n");
    menu_item_4(&test);
    print_converter(&test, 1);
}