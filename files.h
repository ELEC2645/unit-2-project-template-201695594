#ifndef FILES_H
#define FILES_H

int read_converter(char *path, converter *target);
int write_converter(char *path, converter *data);

void print_saves();

#endif /*FILES_H*/