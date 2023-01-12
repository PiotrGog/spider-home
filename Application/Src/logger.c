#include <Application/logger.h>

#include <usart.h>

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

int logMessageVaList(const Severity severity, const char *format, va_list args);

int logMessage(const Severity severity, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    const int result = logMessageVaList(severity, format, args);
    va_end(args);

    return result;
}

int logMessageVaList(const Severity severity, const char *format, va_list args)
{
    int result = 0;
    switch (severity)
    {
    case Severity_INFO:
        result += printf("INFO: ");
        break;
    case Severity_DEBUG:
        result += printf("DEBUG: ");
        break;
    case Severity_ERROR:
        result += printf("ERROR: ");
        break;
    }
    result += vprintf(format, args);
    result += printf("\r\n");
    return result;
}

int info(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    const int result = logMessageVaList(Severity_INFO, format, args);
    va_end(args);

    return result;
}

int debug(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    const int result = logMessageVaList(Severity_DEBUG, format, args);
    va_end(args);

    return result;
}

int error(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    const int result = logMessageVaList(Severity_ERROR, format, args);
    va_end(args);

    return result;
}

size_t _write(int handle, const unsigned char *buffer, size_t size)
{
    HAL_UART_Transmit(&huart2, (uint8_t *)buffer, size, HAL_MAX_DELAY);
    return size;
}
