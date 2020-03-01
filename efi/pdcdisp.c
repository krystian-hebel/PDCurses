/* PDCurses */

#include "pdcefi.h"
#include "../common/acsuni.h"

EFI_SIMPLE_TEXT_OUT_PROTOCOL *_STOI = NULL;
EFI_SIMPLE_TEXT_IN_PROTOCOL *_STII = NULL;

//~ static char argv_buf[800];

//~ static void conv(char **ptr, int len)
//~ {
    //~ char *dst = argv_buf;
    //~ for (int i = 0; i < len; i++) {
        //~ CHAR16 *src = (CHAR16*)ptr[i];
        //~ ptr[i] = dst;
        //~ do {
            //~ *dst = (char)*src;
            //~ dst++;
        //~ } while (*(src++));
    //~ }
//~ }

//~ static void pc(char x) {
    //~ asm volatile ("outb %0, %w1" :: "a"(x), "d"(0x3f8));
//~ }

//~ EFI_STATUS EFIAPI efi_main(EFI_HANDLE handle, EFI_SYSTEM_TABLE *systable) {
    //~ pc('e');
    //~ InitializeLib(handle, systable);
    //~ pc('i');

    //~ pc('e');
    //~ char *argv_p[20];
    //~ pc('e');
    //~ CHAR16 **argv = (CHAR16**)&argv_p;
    
    //~ int argc = GetShellArgcArgv(handle, &argv);
    //~ pc('a');

    //~ conv(argv_p, argc);
    //~ pc('c');

    //~ return main(argc, argv_p);
    //~ pc('X');
//~ }


//~ static inline void pc(char x) {
	//~ asm volatile ("outb %0, %w1" :: "a"(x), "d"(0x3f8));
//~ }

/* position hardware cursor at (y, x) */

void PDC_gotoyx(int row, int col)
{
    uefi_call_wrapper(_STOI->SetCursorPosition, 3,
                      _STOI, col, row);
}

void _new_packet(attr_t attr, int lineno, int x, int len, const chtype *srcp)
{
    int j;
    short fore, back;
    bool blink;

    // TODO: infinite loop
    if (lineno == (SP->lines - 1) && (x + len) >= SP->cols)
    {
        len--;
        if (len) {
            _new_packet(attr, lineno, x, len, srcp);
        }

        //_new_packet(attr, lineno, x + len, 1, srcp + len);
        return;
    }

    pair_content(PAIR_NUMBER(attr), &fore, &back);
    blink = (SP->termattrs & A_BLINK) && (attr & A_BLINK);

    if (blink)
        attr &= ~A_BLINK;

    if (attr & A_BOLD)
        fore |= 8;
    if (attr & A_BLINK)
        back |= 8;

    //~ if (ansi)
    {
//~ #ifdef PDC_WIDE
        CHAR16 buffer[512];
//~ #else
        //~ char buffer[512];
//~ #endif
        for (j = 0; j < len; j++)
        {
            chtype ch = srcp[j];

            if (ch & A_ALTCHARSET && !(ch & 0xff80))
                ch = acs_map[ch & 0x7f];

            buffer[j] = ch & A_CHARTEXT;
        }

        PDC_gotoyx(lineno, x);
        //~ _set_ansi_color(fore, back, attr);
        if (attr & A_REVERSE) {
            uefi_call_wrapper(_STOI->SetAttribute, 2,
                              _STOI, (fore << 4) | back);
        } else {
            uefi_call_wrapper(_STOI->SetAttribute, 2,
                              _STOI, (back << 4) | fore);
        }
//~ #ifdef PDC_WIDE
        Print(L"%*s", len, buffer);
//~ #else
        //~ WriteConsoleA(pdc_con_out, buffer, len, NULL, NULL);
//~ #endif
    }
    //~ else
    //~ {
        //~ CHAR_INFO buffer[512];
        //~ COORD bufSize, bufPos;
        //~ SMALL_RECT sr;
        //~ WORD mapped_attr;

        //~ fore = pdc_curstoreal[fore];
        //~ back = pdc_curstoreal[back];

        //~ if (attr & A_REVERSE)
            //~ mapped_attr = back | (fore << 4);
        //~ else
            //~ mapped_attr = fore | (back << 4);

        //~ if (attr & A_UNDERLINE)
            //~ mapped_attr |= 0x8000; /* COMMON_LVB_UNDERSCORE */
        //~ if (attr & A_LEFT)
            //~ mapped_attr |= 0x0800; /* COMMON_LVB_GRID_LVERTICAL */
        //~ if (attr & A_RIGHT)
            //~ mapped_attr |= 0x1000; /* COMMON_LVB_GRID_RVERTICAL */

        //~ for (j = 0; j < len; j++)
        //~ {
            //~ chtype ch = srcp[j];

            //~ if (ch & A_ALTCHARSET && !(ch & 0xff80))
                //~ ch = acs_map[ch & 0x7f];

            //~ if (blink && blinked_off)
                //~ ch = ' ';

            //~ buffer[j].Attributes = mapped_attr;
            //~ buffer[j].Char.UnicodeChar = ch & A_CHARTEXT;
        //~ }

        //~ bufPos.X = bufPos.Y = 0;
        //~ bufSize.X = len;
        //~ bufSize.Y = 1;

        //~ sr.Top = sr.Bottom = lineno;
        //~ sr.Left = x;
        //~ sr.Right = x + len - 1;

        //~ // TODO: &sr part
        //~ //WriteConsoleOutput(pdc_con_out, buffer, bufSize, bufPos, &sr);
        //~ (void)sr;
        //~ Print(L"%*s", len, buffer);
    //~ }
}

/* update the given physical line to look like the corresponding line in
   curscr */

void PDC_transform_line(int lineno, int x, int len, const chtype *srcp)
{
    attr_t old_attr, attr;
    int i, j;

    PDC_LOG(("PDC_transform_line() - called: lineno=%d\n", lineno));

    old_attr = *srcp & (A_ATTRIBUTES ^ A_ALTCHARSET);

    for (i = 1, j = 1; j < len; i++, j++)
    {
        attr = srcp[i] & (A_ATTRIBUTES ^ A_ALTCHARSET);

        if (attr != old_attr)
        {
            _new_packet(old_attr, lineno, x, i, srcp);
            old_attr = attr;
            srcp += i;
            x += i;
            i = 0;
        }
    }

    _new_packet(old_attr, lineno, x, i, srcp);
}

void PDC_doupdate(void)
{
}
