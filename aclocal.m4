dnl pupa_ASM_USCORE checks if C symbols get an underscore after
dnl compiling to assembler.
dnl Written by Pavel Roskin. Based on grub_ASM_EXT_C written by
dnl Erich Boleyn and modified by Yoshinori K. Okuji.
AC_DEFUN(pupa_ASM_USCORE,
[AC_REQUIRE([AC_PROG_CC])
AC_MSG_CHECKING([if C symbols get an underscore after compilation])
AC_CACHE_VAL(pupa_cv_asm_uscore,
[cat > conftest.c <<\EOF
int
func (int *list)
{
  *list = 0;
  return *list;
}
EOF

if AC_TRY_COMMAND([${CC-cc} ${CFLAGS} -S conftest.c]) && test -s conftest.s; then
  true
else
  AC_MSG_ERROR([${CC-cc} failed to produce assembly code])
fi

if grep _func conftest.s >/dev/null 2>&1; then
  pupa_cv_asm_uscore=yes
else
  pupa_cv_asm_uscore=no
fi

rm -f conftest*])

if test "x$pupa_cv_asm_uscore" = xyes; then
  AC_DEFINE_UNQUOTED([HAVE_ASM_USCORE], $pupa_cv_asm_uscore,
    [Define if C symbols get an underscore after compilation])
fi

AC_MSG_RESULT([$pupa_cv_asm_uscore])
])


dnl Some versions of `objcopy -O binary' vary their output depending
dnl on the link address.
AC_DEFUN(pupa_PROG_OBJCOPY_ABSOLUTE,
[AC_MSG_CHECKING([whether ${OBJCOPY} works for absolute addresses])
AC_CACHE_VAL(pupa_cv_prog_objcopy_absolute,
[cat > conftest.c <<\EOF
void
cmain (void)
{
   *((int *) 0x1000) = 2;
}
EOF

if AC_TRY_EVAL(ac_compile) && test -s conftest.o; then :
else
  AC_MSG_ERROR([${CC-cc} cannot compile C source code])
fi
pupa_cv_prog_objcopy_absolute=yes
for link_addr in 2000 8000 7C00; do
  if AC_TRY_COMMAND([${CC-cc} ${CFLAGS} -nostdlib -Wl,-N -Wl,-Ttext -Wl,$link_addr conftest.o -o conftest.exec]); then :
  else
    AC_MSG_ERROR([${CC-cc} cannot link at address $link_addr])
  fi
  if AC_TRY_COMMAND([${OBJCOPY-objcopy} -O binary conftest.exec conftest]); then :
  else
    AC_MSG_ERROR([${OBJCOPY-objcopy} cannot create binary files])
  fi
  if test ! -f conftest.old || AC_TRY_COMMAND([cmp -s conftest.old conftest]); then
    mv -f conftest conftest.old
  else
    pupa_cv_prog_objcopy_absolute=no
    break
  fi
done
rm -f conftest*])
AC_MSG_RESULT([$pupa_cv_prog_objcopy_absolute])

if test "x$pupa_cv_prog_objcopy_absolute" = xno; then
  AC_MSG_ERROR([PUPA requires a working absolute objcopy; upgrade your binutils])
fi
])


dnl Mass confusion!
dnl Older versions of GAS interpret `.code16' to mean ``generate 32-bit
dnl instructions, but implicitly insert addr32 and data32 bytes so
dnl that the code works in real mode''.
dnl
dnl Newer versions of GAS interpret `.code16' to mean ``generate 16-bit
dnl instructions,'' which seems right.  This requires the programmer
dnl to explicitly insert addr32 and data32 instructions when they want
dnl them.
dnl
dnl We only support the newer versions, because the old versions cause
dnl major pain, by requiring manual assembly to get 16-bit instructions into
dnl asm files.
AC_DEFUN(pupa_I386_ASM_ADDR32,
[AC_REQUIRE([AC_PROG_CC])
AC_REQUIRE([pupa_I386_ASM_PREFIX_REQUIREMENT])
AC_MSG_CHECKING([for .code16 addr32 assembler support])
AC_CACHE_VAL(pupa_cv_i386_asm_addr32,
[cat > conftest.s.in <<\EOF
	.code16
l1:	@ADDR32@	movb	%al, l1
EOF

if test "x$pupa_cv_i386_asm_prefix_requirement" = xyes; then
  sed -e s/@ADDR32@/addr32/ < conftest.s.in > conftest.s
else
  sed -e s/@ADDR32@/addr32\;/ < conftest.s.in > conftest.s
fi

if AC_TRY_COMMAND([${CC-cc} ${CFLAGS} -c conftest.s]) && test -s conftest.o; then
  pupa_cv_i386_asm_addr32=yes
else
  pupa_cv_i386_asm_addr32=no
fi

rm -f conftest*])

AC_MSG_RESULT([$pupa_cv_i386_asm_addr32])])


dnl Later versions of GAS requires that addr32 and data32 prefixes
dnl appear in the same lines as the instructions they modify, while
dnl earlier versions requires that they appear in separate lines.
AC_DEFUN(pupa_I386_ASM_PREFIX_REQUIREMENT,
[AC_REQUIRE([AC_PROG_CC])
AC_MSG_CHECKING(dnl
[whether addr32 must be in the same line as the instruction])
AC_CACHE_VAL(pupa_cv_i386_asm_prefix_requirement,
[cat > conftest.s <<\EOF
	.code16
l1:	addr32	movb	%al, l1
EOF

if AC_TRY_COMMAND([${CC-cc} ${CFLAGS} -c conftest.s]) && test -s conftest.o; then
  pupa_cv_i386_asm_prefix_requirement=yes
else
  pupa_cv_i386_asm_prefix_requirement=no
fi

rm -f conftest*])

if test "x$pupa_cv_i386_asm_prefix_requirement" = xyes; then
  pupa_tmp_addr32="addr32"
  pupa_tmp_data32="data32"
else
  pupa_tmp_addr32="addr32;"
  pupa_tmp_data32="data32;"
fi

AC_DEFINE_UNQUOTED([ADDR32], $pupa_tmp_addr32,
  [Define it to \"addr32\" or \"addr32;\" to make GAS happy])
AC_DEFINE_UNQUOTED([DATA32], $pupa_tmp_data32,
  [Define it to \"data32\" or \"data32;\" to make GAS happy])

AC_MSG_RESULT([$pupa_cv_i386_asm_prefix_requirement])])


dnl Older versions of GAS require that absolute indirect calls/jumps are
dnl not prefixed with `*', while later versions warn if not prefixed.
AC_DEFUN(pupa_I386_ASM_ABSOLUTE_WITHOUT_ASTERISK,
[AC_REQUIRE([AC_PROG_CC])
AC_MSG_CHECKING(dnl
[whether an absolute indirect call/jump must not be prefixed with an asterisk])
AC_CACHE_VAL(pupa_cv_i386_asm_absolute_without_asterisk,
[cat > conftest.s <<\EOF
	lcall	*(offset)	
offset:
	.long	0
	.word	0
EOF

if AC_TRY_COMMAND([${CC-cc} ${CFLAGS} -c conftest.s]) && test -s conftest.o; then
  pupa_cv_i386_asm_absolute_without_asterisk=no
else
  pupa_cv_i386_asm_absolute_without_asterisk=yes
fi

rm -f conftest*])

if test "x$pupa_cv_i386_asm_absolute_without_asterisk" = xyes; then
  AC_DEFINE([ABSOLUTE_WITHOUT_ASTERISK], 1,
	    [Define it if GAS requires that absolute indirect calls/jumps are not prefixed with an asterisk])
fi

AC_MSG_RESULT([$pupa_cv_i386_asm_absolute_without_asterisk])])


dnl Check what symbol is defined as a start symbol.
dnl Written by Yoshinori K. Okuji.
AC_DEFUN(pupa_CHECK_START_SYMBOL,
[AC_REQUIRE([AC_PROG_CC])
AC_MSG_CHECKING([if start is defined by the compiler])
AC_CACHE_VAL(pupa_cv_check_start_symbol,
[AC_TRY_LINK([], [asm ("incl start")],
   pupa_cv_check_start_symbol=yes,
   pupa_cv_check_start_symbol=no)])

AC_MSG_RESULT([$pupa_cv_check_start_symbol])

AC_MSG_CHECKING([if _start is defined by the compiler])
AC_CACHE_VAL(pupa_cv_check_uscore_start_symbol,
[AC_TRY_LINK([], [asm ("incl _start")],
   pupa_cv_check_uscore_start_symbol=yes,
   pupa_cv_check_uscore_start_symbol=no)])

AC_MSG_RESULT([$pupa_cv_check_uscore_start_symbol])

AH_TEMPLATE([START_SYMBOL], [Define it to either start or _start])

if test "x$pupa_cv_check_start_symbol" = xyes; then
  AC_DEFINE([START_SYMBOL], [start])
elif test "x$pupa_cv_check_uscore_start_symbol" = xyes; then
  AC_DEFINE([START_SYMBOL], [_start])
else
  AC_MSG_ERROR([neither start nor _start is defined])
fi
])

dnl Check what symbol is defined as a bss start symbol.
dnl Written by Michael Hohmoth and Yoshinori K. Okuji.
AC_DEFUN(pupa_CHECK_BSS_START_SYMBOL,
[AC_REQUIRE([AC_PROG_CC])
AC_MSG_CHECKING([if __bss_start is defined by the compiler])
AC_CACHE_VAL(pupa_cv_check_uscore_uscore_bss_start_symbol,
[AC_TRY_LINK([], [asm ("incl __bss_start")],
   pupa_cv_check_uscore_uscore_bss_start_symbol=yes,
   pupa_cv_check_uscore_uscore_bss_start_symbol=no)])

AC_MSG_RESULT([$pupa_cv_check_uscore_uscore_bss_start_symbol])

AC_MSG_CHECKING([if edata is defined by the compiler])
AC_CACHE_VAL(pupa_cv_check_edata_symbol,
[AC_TRY_LINK([], [asm ("incl edata")],
   pupa_cv_check_edata_symbol=yes,
   pupa_cv_check_edata_symbol=no)])

AC_MSG_RESULT([$pupa_cv_check_edata_symbol])

AC_MSG_CHECKING([if _edata is defined by the compiler])
AC_CACHE_VAL(pupa_cv_check_uscore_edata_symbol,
[AC_TRY_LINK([], [asm ("incl _edata")],
   pupa_cv_check_uscore_edata_symbol=yes,
   pupa_cv_check_uscore_edata_symbol=no)])

AC_MSG_RESULT([$pupa_cv_check_uscore_edata_symbol])

AH_TEMPLATE([BSS_START_SYMBOL], [Define it to one of __bss_start, edata and _edata])

if test "x$pupa_cv_check_uscore_uscore_bss_start_symbol" = xyes; then
  AC_DEFINE([BSS_START_SYMBOL], [__bss_start])
elif test "x$pupa_cv_check_edata_symbol" = xyes; then
  AC_DEFINE([BSS_START_SYMBOL], [edata])
elif test "x$pupa_cv_check_uscore_edata_symbol" = xyes; then
  AC_DEFINE([BSS_START_SYMBOL], [_edata])
else
  AC_MSG_ERROR([none of __bss_start, edata or _edata is defined])
fi
])

dnl Check what symbol is defined as an end symbol.
dnl Written by Yoshinori K. Okuji.
AC_DEFUN(pupa_CHECK_END_SYMBOL,
[AC_REQUIRE([AC_PROG_CC])
AC_MSG_CHECKING([if end is defined by the compiler])
AC_CACHE_VAL(pupa_cv_check_end_symbol,
[AC_TRY_LINK([], [asm ("incl end")],
   pupa_cv_check_end_symbol=yes,
   pupa_cv_check_end_symbol=no)])

AC_MSG_RESULT([$pupa_cv_check_end_symbol])

AC_MSG_CHECKING([if _end is defined by the compiler])
AC_CACHE_VAL(pupa_cv_check_uscore_end_symbol,
[AC_TRY_LINK([], [asm ("incl _end")],
   pupa_cv_check_uscore_end_symbol=yes,
   pupa_cv_check_uscore_end_symbol=no)])

AC_MSG_RESULT([$pupa_cv_check_uscore_end_symbol])

AH_TEMPLATE([END_SYMBOL], [Define it to either end or _end])

if test "x$pupa_cv_check_end_symbol" = xyes; then
  AC_DEFINE([END_SYMBOL], [end])
elif test "x$pupa_cv_check_uscore_end_symbol" = xyes; then
  AC_DEFINE([END_SYMBOL], [_end])
else
  AC_MSG_ERROR([neither end nor _end is defined])
fi
])