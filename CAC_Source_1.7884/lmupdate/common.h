#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <windows.h>

#ifdef __BORLANDC__
#include "snprintf.h"
#endif  // __BORLANDC__

#include "cactypes.h"
#include "profile.h"
#include "vmessage.h"
#include "message.h"
