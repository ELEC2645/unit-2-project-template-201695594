#ifndef MENU_FUNCS
#define MENU_FUNCS

void menu_item_1(converter *active); // set converter name & type
void menu_item_2(converter *active); // set initial converter parameters
void menu_item_3(converter *active); // edit individual converter parameters
void menu_item_4(converter *active); // compute converter
void menu_item_5(converter *active); // load/save/export converter design

void print_converter(converter *active, int flag); // print a numbered list of all the parsed converter struct fields

#endif /*MENU_FUNCS*/