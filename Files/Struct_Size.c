#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct
{
    uint16_t AC_Voltage;
    uint16_t AC_Current;
    uint16_t AC_Power;
    uint16_t AC_Freq;

    uint16_t DC_Voltage;
    uint16_t DC_Current;
    uint16_t DC_Power;
    uint16_t DC_Freq;

    uint16_t Extras[];
} InputRegMap;

#define Extra (64 - sizeof(InputRegMap))
int main()
{
    int x = Extra;
    printf("%d", x);

    return 0;
}