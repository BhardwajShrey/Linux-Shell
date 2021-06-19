#ifndef SYMTAB_H
#define SYMTAB_H

#include "../node.h"

#define MAX_SYMTAB 256

// type of value in symbol table entry
enum symbol_type_e
{
    SYM_STR,   // shell variables
    SYM_FUNC,   // functions
};

// symbol table entry structure
struct symtab_entry_s
{
    char *name;     // name of variable or function
    enum symbol_type_e val_type;    // SYM_STR or SYM_FUNC
    char *val;      // string value for shell variables
    unsigned int flags;
    struct symtab_entry_s *next;    // pointer to next entry in symbol table (linked list)
    struct node_s *func_body;       // abstract syntax tree for function
};

// represents a single symbol table (like LL class containing nodes). 0th table is global table (for global variables)
// tables one and above are local tables. can implement variable scopes via cascading tables
struct symtab_s
{
    int level;      // level of table (0 for global. others for local)
    struct symtab_entry_s *first, *last;    // self-explanatory
};

/* values for the flags field of struct symtab_entry_s */                       
#define FLAG_EXPORT (1 << 0) /* export entry to forked commands */

// symbol table stack to cascade tables
struct symtab_stack_s
{
    int symtab_count;   // no. of tables currently in stack
    struct symtab_s *symtab_list[MAX_SYMTAB];   // array of pointers to symbol tables. 0th item is global table
    struct symtab_s *global_symtab, *local_symtab;  // for ease of access
};

struct symtab_s *new_symtab(int level);
struct symtab_s *symtab_stack_push(void);
struct symtab_s *symtab_stack_pop(void);
int rem_from_symtab(struct symtab_entry_s *entry, struct symtab_s *symtab);
struct symtab_entry_s *add_to_symtab(char *symbol);
struct symtab_entry_s *do_lookup(char *str, struct symtab_s *symtable);
struct symtab_entry_s *get_symtab_entry(char *str);
struct symtab_s *get_local_symtab(void);
struct symtab_s *get_global_symtab(void);
struct symtab_stack_s *get_symtab_stack(void);
void init_symtab(void);
void dump_local_symtab(void);
void free_symtab(struct symtab_s *symtab);
void symtab_entry_setval(struct symtab_entry_s *entry, char *val); 

#endif