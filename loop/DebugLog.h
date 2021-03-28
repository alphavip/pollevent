#ifndef _DEBUGLOG_H_ 
#define _DEBUGLOG_H_ 


#include <stdio.h>

#define LOG(format, ...) fprintf(stderr, "%s:%d:%s->i" format "\n", __FILE__, __LINE__, __func__, __VA_ARGS__)

#endif // _DEBUGLOG_H_
