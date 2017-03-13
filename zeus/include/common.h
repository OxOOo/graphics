#ifndef ZEUS_COMMON_H
#define ZEUS_COMMON_H

#include <plog/Log.h>
#include <cstdlib>

#define NOT_IMPLEMENTED {LOG_FATAL << "NOT_IMPLEMENTED"; exit(1);}
#define FATAL {LOG_FATAL << "FATAL ERROR"; exit(1);}
#define ASSERT(exp) {if (!(exp)) FATAL;}

#endif // ZEUS_COMMON_H