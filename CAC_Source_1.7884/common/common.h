#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdarg.h>
#ifdef __BORLANDC__
typedef long intptr_t;
typedef unsigned long uintptr_t;
#else  // __BORLANDC__
#include <stdint.h>
#endif  // __BORLANDC__
#include <stdio.h>
#ifdef __BORLANDC__
#include "snprintf.h"
#endif  // __BORLANDC__
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "cactypes.h"
#include "profile.h"
#include "filename.h"
#include "vmessage.h"
#include "message.h"
#include "memblock.h"
#include "mstr.h"
#include "object.h"
#include "pointer.h"
#include "array.h"
