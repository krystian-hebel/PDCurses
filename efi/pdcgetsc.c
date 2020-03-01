/* PDCurses */

#include "pdcefi.h"

/* return width of screen/viewport */

int PDC_get_columns(void)
{
    UINTN mode, cols, rows;

    mode = _STOI->Mode->Mode;
    uefi_call_wrapper(_STOI->QueryMode, 4,
                      _STOI, mode, &cols, &rows);

    PDC_LOG(("PDC_get_columns() - returned: cols %d\n", cols));

    return cols;
}

/* get the cursor size/shape */

int PDC_get_cursor_mode(void)
{
    PDC_LOG(("PDC_get_cursor_mode() - called\n"));

    return _STOI->Mode->Mode;
}

/* return number of screen rows */

int PDC_get_rows(void)
{
    UINTN mode, cols, rows;

    mode = _STOI->Mode->Mode;
    uefi_call_wrapper(_STOI->QueryMode, 4,
                      _STOI, mode, &cols, &rows);

    PDC_LOG(("PDC_get_rows() - returned: rows %d\n", rows));

    return rows;
}
