#pragma once
#include <stdint.h>

#define MAX_CALLBACKS 16

typedef void (*timer_callback_t)(void);
void set_timeout(timer_callback_t cb, uint64_t ms);
void tm_cb_update(void);