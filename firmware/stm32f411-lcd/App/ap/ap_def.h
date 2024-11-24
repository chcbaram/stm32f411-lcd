#ifndef AP_DEF_H_
#define AP_DEF_H_


#include "hw.h"




typedef struct
{
  char name[32];
  void (*init)(void);
  void (*run_func)(void);
} app_info_t;

#endif