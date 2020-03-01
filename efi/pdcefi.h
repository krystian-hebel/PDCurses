/* PDCurses */

#include <curspriv.h>
#include <efi.h>
#include <efilib.h>

extern EFI_SIMPLE_TEXT_OUT_PROTOCOL *_STOI;
extern EFI_SIMPLE_TEXT_IN_PROTOCOL *_STII;

extern int main(int, char **);

#ifndef __EFIGLUE
#define __EFIGLUE

typedef UINT32 time_t;

// srand initialization only?
time_t time(time_t *t);

void srand(UINTN seed);

UINT32 rand();

static inline int abs(int x)
{
    return (x < 0) ? -x : x;
}

void *malloc(UINTN size);

void *calloc(UINTN nmemb, UINTN size);

void free(void *buffer);

void exit(int status);

char *strcpy(char *dst, char *src);

#define strlen(x)   strlena((unsigned char*)x)

#endif
