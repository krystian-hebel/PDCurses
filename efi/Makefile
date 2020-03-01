# GNU Makefile for PDCurses - UEFI
#
# Usage: make [-f path\Makefile] [DEBUG=Y] [target]
#
# where target can be any of:
# [all|libs|demos|pdcurses.a|testcurs.exe...]

O = o
E = .EFI
RM = rm

ARCH		:= x86_64

EFIINC		:= /usr/include/efi
EFIINCS		= -I$(EFIINC) -I$(EFIINC)/$(ARCH) #-I$(EFIINC)/protocol
EFILIB		:= /usr/lib
EFI_CRT_OBJS	= $(EFILIB)/crt0-efi-$(ARCH).o
EFI_LDS		= $(EFILIB)/elf_$(ARCH)_efi.lds
CFLAGS		= -g -fno-stack-protector -fpic \
    -fshort-wchar -Wall $(EFIINCS)
ifeq ($(ARCH),x86_64)
    CFLAGS	+= -DEFI_FUNCTION_WRAPPER -mno-red-zone
endif
LDFLAGS = $(EFI_CRT_OBJS) -nostdlib -znocombreloc -T $(EFI_LDS) -shared \
    -Bsymbolic -L $(EFILIB)

ifndef PDCURSES_SRCDIR
	PDCURSES_SRCDIR = ..
endif

include $(PDCURSES_SRCDIR)/common/libobjs.mif

osdir		= $(PDCURSES_SRCDIR)/efi

PDCURSES_EFI_H	= $(osdir)/pdcefi.h

CC		= gcc

ifeq ($(DEBUG),Y)
	CFLAGS  += -g -Werror -DPDCDEBUG
else
	CFLAGS  += -O2 -Werror
endif

CFLAGS += -I$(PDCURSES_SRCDIR)

LINK		= gcc

LIBEXE		= ar
LIBFLAGS	= rcv

LIBCURSES	= pdcurses.a

.PHONY: all libs clean demos dist

all:	libs

libs:	$(LIBCURSES)

clean:
	-$(RM) *.o
	-$(RM) *.a
	-$(RM) *$E

demos:	$(DEMOS)
ifneq ($(DEBUG),Y)
	strip *$E
endif

$(LIBCURSES) : $(LIBOBJS) $(PDCOBJS)
	$(LIBEXE) $(LIBFLAGS) $@ $?

$(LIBOBJS) $(PDCOBJS) : $(PDCURSES_HEADERS)
$(PDCOBJS) : $(PDCURSES_DOS_H)
$(DEMOS) : $(PDCURSES_CURSES_H) $(LIBCURSES)
panel.o : $(PANEL_HEADER)

$(LIBOBJS) : %.o: $(srcdir)/%.c
	$(CC) -c $(CFLAGS) -lefi -lgnuefi -nostdlib -znocombreloc $<

$(PDCOBJS) : %.o: $(osdir)/%.c
	$(CC) -c $(CFLAGS) -lefi -lgnuefi -nostdlib -znocombreloc $<

#firework.exe ozdemo.exe rain.exe testcurs.exe worm.exe xmas.exe \
#ptest.exe: %.exe: $(demodir)/%.c
#	$(CC) $(CFLAGS) -o$@ $< $(LIBCURSES)

#$(demodir)/%.o: $(demodir)/%.c
#	$(CC) $(CFLAGS) -o$@ $< $(LIBCURSES)

sample_%.EFI: $(demodir)/%.o
	ld $^ $(LDFLAGS) -o $@ $(LIBCURSES) -lefi -lgnuefi
	objcopy -j .text -j .sdata -j .data -j .dynamic \
	-j .dynsym  -j .rel -j .rela -j .reloc -j .plt -j .rela.plt \
	--target=efi-app-$(ARCH) $@ $@

tuidemo.exe: tuidemo.o tui.o
	$(LINK) $(LDFLAGS) -o$@ tuidemo.o tui.o $(LIBCURSES)

tui.o: $(demodir)/tui.c $(demodir)/tui.h $(PDCURSES_CURSES_H)
	$(CC) -c $(CFLAGS) -I$(demodir) -o$@ $<

tuidemo.o: $(demodir)/tuidemo.c $(PDCURSES_CURSES_H)
	$(CC) -c $(CFLAGS) -I$(demodir) -o$@ $<
