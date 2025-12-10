/*TESTING FILE*/
/*Used for testing individual functions/features during development*/
#include "src/include/funcs.h"
#include "src/include/menu_funcs.h"
#include "src/include/colours.h"
#include "src/include/files.h"

#include "stdio.h"

int buck_test();
int boost_test();
int buckboost_test();
int cuk_test();

// template blank buck converter with all values unknown
converter test = {
    .name = "TestConverter",
    .type = 1,
    .s_type = "Buck\0",
    .V_o = -1,
    .V_i = -1,
    .I_o = -1,
    .I_i = -1,
    .F_s = -1,
    .R_l = -1,
    .i_rip = -1,
    .v_rip = -1,
    .i_rip2 = -2,
    .v_rip2 = -2,
    .k = -1,
    .C_n = -2,
    .C_o = -1,
    .L = -1,
    .L2 = -2,
};

int main(void){
    printf("Running converter compute tests...\n");
    // call each test
    int buck_res = buck_test();
    int boost_res = boost_test();
    int bboost_res = buckboost_test();
    int cuk_res = cuk_test();
    printf("\n");
    // output results
    if (buck_res) { printf(GREEN"BUCK TESTS PASSED\n"RESET);}
    else {printf(RED"BUCK TESTS FAILED\n"RESET);}
    if (boost_res) { printf(GREEN"BOOST TESTS PASSED\n"RESET);}
    else {printf(RED"BOOST TESTS FAILED\n"RESET);}
    if (bboost_res) { printf(GREEN"BUCK-BOOST TESTS PASSED\n"RESET);}
    else {printf(RED"BUCK-BOOST TESTS FAILED\n"RESET);}
    if (cuk_res) { printf(GREEN"CUK TESTS PASSED\n"RESET);}
    else {printf(RED"CUK TESTS FAILED\n"RESET);}

    /*printf("Reading test converter to json... \n");
    print_converter(&test, 1);
    int error = read_converter("./saves/test.json", &test);
    if (!error) {printf("success");}
    print_converter(&test, 1);*/
}

int buck_test(){
    // set parameters
    converter buck = test;
    buck.type = 1;
    buck.V_o = 30;
    buck.V_i = 100;
    buck.R_l = 200;
    buck.i_rip = 0.5;
    buck.v_rip = 3;
    buck.F_s = 10e3;
    // run compute option
    compute_converter(&buck);
    print_converter(&buck, 1);
    // assume pass
    int pass = 1;
    // check values against expected
    if (buck.I_o != (float)0.15) {printf("I_o failed\n"); pass = 0;}
    if (buck.I_i != (float)0.045) {printf("I_i failed\n"); pass = 0;}
    if (buck.k != (float)0.3) {printf("k failed\n"); pass = 0;}
    if (buck.L != (float)4.2e-3) {printf("L failed\n"); pass = 0;}
    if (buck.C_o != (float)6.25e-6 / 3) {printf("C_o failed\n"); pass = 0;}
    
    return pass; // result
}

int boost_test(){
    // set parameters
    converter boost = test;
    boost.type = 2;
    boost.V_o = 100;
    boost.V_i = 30;
    boost.R_l = 200;
    boost.i_rip = 0.5;
    boost.v_rip = 3;
    boost.F_s = 10e3;
    // run compute option
    compute_converter(&boost);
    print_converter(&boost, 1);
    // assume pass
    int pass = 1;
    // check values against expected
    if (boost.I_o != (float)0.5) {printf("I_o failed\n"); pass = 0;}
    if (boost.I_i != (float)0.5 / (float)0.3) {printf("I_i failed\n Expected: %.4f Got: %.4f",(float)0.35 / (float)0.3, boost.I_i); pass = 0;}
    if (boost.k != (float)0.7) {printf("k failed\n"); pass = 0;}
    if (boost.L != (float)4.2e-3) {printf("L failed\n"); pass = 0;}
    // C_o check always fails due to float division errors/difference. Values printed for mmanual comparision
    if (boost.C_o != (float)3.5e-5 / 3.0) {printf("C_o failed\n Expected: %.10e Got: %.10e\n",(float)3.5e-5 / 3.0, boost.C_o); pass = 0;}
    
    return pass; // result
}

int buckboost_test(){
    // set parameters
    converter b_boost = test;
    b_boost.type = 3;
    b_boost.V_o = 150;
    b_boost.V_i = 50;
    b_boost.R_l = 200;
    b_boost.i_rip = 0.5;
    b_boost.v_rip = 3;
    b_boost.F_s = 10e3;
    // run compute option
    compute_converter(&b_boost);
    print_converter(&b_boost, 1);
    // assume pass
    int pass = 1;
    // check values against expected
    if (b_boost.I_o != (float)0.75) {printf("I_o failed\n"); pass = 0;}
    if (b_boost.I_i != (float)2.25) {printf("I_i failed\n"); pass = 0;}
    if (b_boost.k != (float)0.75) {printf("k failed\n"); pass = 0;}
    if (b_boost.L != (float)7.5e-3) {printf("L failed\n"); pass = 0;}
    if (b_boost.C_o != (float)1.875e-5) {printf("C_o failed\n"); pass = 0;}
    
    return pass; // result

}
int cuk_test(){
    // set parameters
    converter cuk = test;
    cuk.type = 4;
    cuk.V_o = 150;
    cuk.V_i = 50;
    cuk.R_l = 200;
    cuk.i_rip = 0.5;
    cuk.v_rip = 3;
    cuk.i_rip2 = 0.4;
    cuk.v_rip2 = 2;
    cuk.F_s = 10e3;
    cuk.L2 = -1;
    cuk.C_n = -1;
    // run compute option
    compute_converter(&cuk);
    print_converter(&cuk, 1);
    // assume pass
    int pass = 1;
    // check values against expected
    if (cuk.I_o != (float)0.75) {printf("I_o failed\n"); pass = 0;}
    if (cuk.I_i != (float)2.25) {printf("I_i failed\n"); pass = 0;}
    if (cuk.k != (float)0.75) {printf("k failed\n"); pass = 0;}
    if (cuk.L != (float)7.5e-3) {printf("L failed\n"); pass = 0;}
    if (cuk.L2 != (float)9.375e-3) {printf("L2 failed\n"); pass = 0;}
    if (cuk.C_o != (float)5e-6 / 3) {printf("C_o failed\n Expected: %.10e Got: %.10e\n",(float)3.5e-5 / 3.0, cuk.C_o); pass = 0;}
    if (cuk.C_n != (float)2.8125e-5) {printf("C_n failed\n"); pass = 0;}
    
    return pass; // results
}
