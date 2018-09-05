#pragma once

//
// Includes
//

#include "sfr_defs.h"
#include "portpins.h"
#include "common.h"
#include "version.h"

//
// Macros
//

#define RAMEND 2815
#define XRAMEND 65535
#define E2END 1023
#define FLASHEND 32767
#define SPM_PAGESIZE 128
#define E2PAGESIZE 4

#define _PROTECTED_WRITE(register, value) static_assert(false, "_PROTECTED_WRITE is not implemented")