#ifndef FILES_H
#define FILES_H

// reads .json file at path and puts it into target converter struct
int read_converter(char *path, converter *target);
// writes passed converter into .json file at specified path
int write_converter(char *path, converter *data);

// prints file names currently in ./saves
void print_saves();

#endif /*FILES_H*/