/* PDCurses */

#include "pdcefi.h"

static UINTN __seed;

void srand(UINTN seed)
{
    __seed = seed;
}

UINT32 rand()
{
    __seed *= 1578951;
    __seed += 124154;
    return __seed;
}

time_t time(time_t *t)
{
    return 17;
}

void *malloc(UINTN size)
{
    EFI_STATUS err;
    void *buffer;

    err = uefi_call_wrapper(BS->AllocatePool, 3, EfiLoaderData, size, &buffer);
    if (err != EFI_SUCCESS)
        buffer = NULL;

    return buffer;
}

void free(void *buffer)
{
    uefi_call_wrapper(BS->FreePool, 1, buffer);
}

void *calloc(UINTN s, UINTN n)
{
    void * ret = malloc(s*n);
    SetMem(ret, n*s, 0);
    return ret;
}

char *strcpy(char *dst, char *src)
{
    CopyMem(dst, src, strlena((unsigned char*)src));
    return dst;
}

void exit(int st)
{
    Print(L"exit called with status = %d", st);
    // TODO: obtain image handle for BS->Exit()
    while (1);
}

/* close the physical screen -- may restore the screen to its state
   before PDC_scr_open(); miscellaneous cleanup */

void PDC_scr_close(void)
{
    PDC_LOG(("PDC_scr_close() - called\n"));

    reset_shell_mode();

    if (SP->visibility != 1)
        curs_set(1);

    /* Position cursor to the bottom left of the screen. */

    PDC_gotoyx(PDC_get_rows() - 2, 0);
}

void PDC_scr_free(void)
{
}

/* open the physical screen -- miscellaneous initialization, may save
   the existing screen for later restoration */

int PDC_scr_open(void)
{
    PDC_LOG(("PDC_scr_open() - called\n"));

    if (!BS || !ST) {
        PDC_LOG(("UEFI lib not initialised\n"));
        return ERR;     
    }

    _STII = ST->ConIn;
    _STOI = ST->ConOut;

    SP->orig_attr = FALSE;

    SP->mouse_wait = PDC_CLICK_PERIOD;
    SP->audible = FALSE;

    SP->termattrs = (SP->mono ? A_UNDERLINE : A_COLOR) | A_REVERSE | A_BLINK;

    SP->_preserve = FALSE;

    return OK;
}

/* the core of resize_term() */

int PDC_resize_screen(int nlines, int ncols)
{
    PDC_LOG(("PDC_resize_screen() - called. Lines: %d Cols: %d\n",
             nlines, ncols));
    return ERR;
}

void PDC_reset_prog_mode(void)
{
        PDC_LOG(("PDC_reset_prog_mode() - called.\n"));
}

void PDC_reset_shell_mode(void)
{
        PDC_LOG(("PDC_reset_shell_mode() - called.\n"));
}

void PDC_restore_screen_mode(int i)
{
}

void PDC_save_screen_mode(int i)
{
}

bool PDC_can_change_color(void)
{
    return FALSE;
}

int PDC_color_content(short color, short *red, short *green, short *blue)
{
    return ERR;
}

int PDC_init_color(short color, short red, short green, short blue)
{
    return ERR;
}
