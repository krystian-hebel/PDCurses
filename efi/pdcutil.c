/* PDCurses */

#include "pdcefi.h"

void PDC_beep(void)
{
    PDC_LOG(("PDC_beep() - called\n"));
}

void PDC_napms(int ms)
{
    uefi_call_wrapper(BS->Stall, 1, ms * 1000);
}

const char *PDC_sysname(void)
{
    return "gnuefi";
}
