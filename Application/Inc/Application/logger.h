#ifndef __LOGGER_H__
#define __LOGGER_H__

#include "severity.h"

int logMessage(const Severity severity, const char *format, ...);

int info(const char *format, ...);

int debug(const char *format, ...);

int warning(const char *format, ...);

int error(const char *format, ...);

#endif // __LOGGER_H__
