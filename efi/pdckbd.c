/* PDCurses */

#include "pdcefi.h"

static short scan_codes_table[] =
{
    -1,             KEY_UP,         KEY_DOWN,       KEY_RIGHT,  // 0-3
    KEY_LEFT,       KEY_HOME,       KEY_END,        KEY_IC,     // 4-7 (7-EIC?)
    KEY_DC,         KEY_PPAGE,      KEY_NPAGE,      KEY_F(1),   // 8-b
    KEY_F(2),       KEY_F(3),       KEY_F(4),       KEY_F(5),   // c-f
    KEY_F(6),       KEY_F(7),       KEY_F(8),       KEY_F(9),   // 10-13
    KEY_F(10),      KEY_F(11),      KEY_F(12),      ALT_ESC     // 14-17
};

void PDC_set_keyboard_binary(bool on)
{
    PDC_LOG(("PDC_set_keyboard_binary() - called\n"));
}

static EFI_INPUT_KEY _read_key;
static int _read_key_valid = FALSE;
/* check if a key or mouse event is waiting */

bool PDC_check_key(void)
{
    if (_read_key_valid ||
        uefi_call_wrapper(_STII->ReadKeyStroke, 2
                          _STII, &_read_key) == EFI_SUCCESS) {
        _read_key_valid = TRUE;
    }

    return _read_key_valid;
}

/* return the next available key or mouse event */

int PDC_get_key(void)
{
    SP->key_modifiers = 0;

    if (_read_key_valid || PDC_check_key()) {
        _read_key_valid = FALSE;
        SP->key_code = !!_read_key.ScanCode;

        if (!_read_key.ScanCode)    // TODO: should be ASCII or ANSI only?
            return _read_key.UnicodeChar;

        if (_read_key.ScanCode < 0x18)
            return scan_codes_table[_read_key.ScanCode];
    }

    return -1;
}

/* discard any pending keyboard or mouse input -- this is the core
   routine for flushinp() */

void PDC_flushinp(void)
{
    PDC_LOG(("PDC_flushinp() - called\n"));

    uefi_call_wrapper(_STII->Reset, 2, _STII, FALSE);
    _read_key_valid = FALSE;
}

bool PDC_has_mouse(void)
{
    return FALSE;
}

int PDC_mouse_set(void)
{
    unsigned long mbe = SP->_trap_mbe;

    return !mbe ? OK : ERR;
}

int PDC_modifiers_set(void)
{
    return OK;
}
