
#ifndef SYMS_H
#define SYMS_H

#include "uthash.h"

typedef enum {
    SYM_ANY = 0,
    SYM_CONST = 1,
    SYM_ADDRESS = 2,
    } symboltype;

typedef struct symbol_s symbol;

struct symbol_s {
    const char    *name;
    const char    *file;
    const char    *module;
    int            address;
    symboltype     symtype;
    char           islocal;
    const char    *section;
    symbol        *next;
    UT_hash_handle hh;
};

extern symbol* symbol_find_lower(int addr, symboltype preferred_type, uint16_t* offset);
extern void      read_symbol_file(char *filename);
extern const char     *find_symbol(int addr, symboltype preferred_symtype);
extern symbol   *find_symbol_byname(const char *name);
extern int symbol_resolve(char *name);
extern char **parse_words(char *line, int *argc);

#endif