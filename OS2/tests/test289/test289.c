#define INCL_GPIREGIONS         /* Region functions             */

#include <os2.h>

int main(VOID)
{

BOOL   fSuccess;        /* success indicator                    */

HPS    hps;             /* Presentation-space handle            */

HRGN   Hrgn;            /* handle for region                    */

POINTL pptlOffset = {3,3}; /* displacement                      */



fSuccess = GpiOffsetRegion(hps, Hrgn, &pptlOffset);

return 0;
}
