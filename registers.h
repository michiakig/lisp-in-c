#ifndef registers_included
#define registers_included
#include "storage.h"

#define NUM_REGISTERS 10
extern object_t registers[NUM_REGISTERS];

#define EXP registers[0]
#define ENV registers[1]
#define VAL registers[2]
#define PROC registers[3]
#define ARGL registers[4]
#define UNEV registers[5]
#define CONTINUE registers[6]
#define RA registers[7]
#define RB registers[8]
#define RC registers[9]
#endif
