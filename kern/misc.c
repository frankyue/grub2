/* misc.c - definitions of misc functions */
/*
 *  PUPA  --  Preliminary Universal Programming Architecture for GRUB
 *  Copyright (C) 1999,2000,2001,2002  Free Software Foundation, Inc.
 *  Copyright (C) 2002 Yoshinori K. Okuji <okuji@enbug.org>
 *
 *  PUPA is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with PUPA; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <pupa/misc.h>
#include <pupa/err.h>
#include <pupa/mm.h>
#include <stdarg.h>
#include <pupa/term.h>

void *
pupa_memcpy (void *dest, const void *src, pupa_size_t n)
{
  char *d = (char *) dest;
  char *s = (char *) src;
  
  while (n--)
    *d++ = *s++;
  
  return dest;
}

int
pupa_printf (const char *fmt, ...)
{
  va_list ap;
  int ret;
  
  va_start (ap, fmt);
  ret = pupa_vprintf (fmt, ap);
  va_end (ap);

  return ret;
}  

int
pupa_vprintf (const char *fmt, va_list args)
{
  return pupa_vsprintf (0, fmt, args);
}

int
pupa_memcmp (const void *s1, const void *s2, pupa_size_t n)
{
  const char *t1 = s1;
  const char *t2 = s2;
  
  while (n--)
    {
      if (*t1 != *t2)
	return (int) *t1 - (int) *t2;

      t1++;
      t2++;
    }

  return 0;
}

int
pupa_strcmp (const char *s1, const char *s2)
{
  while (*s1 && *s2)
    {
      if (*s1 != *s2)
	return (int) *s1 - (int) *s2;
      
      s1++;
      s2++;
    }

  return (int) *s1 - (int) *s2;
}

char *
pupa_strchr (const char *s, int c)
{
  while (*s)
    {
      if (*s == c)
	return (char *) s;
      s++;
    }

  return 0;
}

char *
pupa_strrchr (const char *s, int c)
{
  char *p = 0;

  while (*s)
    {
      if (*s == c)
	p = (char *) s;
      s++;
    }

  return p;
}

int
pupa_isspace (int c)
{
  return (c == '\n' || c == '\r' || c == ' ' || c == '\t');
}

int
pupa_isprint (int c)
{
  return (c >= ' ' && c <= '~');
}

int
pupa_isalpha (int c)
{
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int
pupa_tolower (int c)
{
  if (c >= 'A' && c <= 'Z')
    return c - 'A' + 'a';

  return c;
}

unsigned long
pupa_strtoul (const char *str, char **end, int base)
{
  unsigned long num = 0;
  int found = 0;
  
  /* Skip white spaces.  */
  while (*str && pupa_isspace (*str))
    str++;
  
  /* Guess the base, if not specified. The prefix `0x' means 16, and
     the prefix `0' means 8.  */
  if (str[0] == '0')
    {
      if (str[1] == 'x')
	{
	  if (base == 0 || base == 16)
	    {
	      base = 16;
	      str += 2;
	    }
	}
      else if (str[1] >= '0' && str[1] <= '7')
	base = 8;
    }
  
  if (base == 0)
    base = 10;

  while (*str)
    {
      unsigned long digit;

      digit = pupa_tolower (*str) - '0';
      if (digit > 9)
	{
	  digit += '0' - 'a' + 10;
	  if (digit >= (unsigned long) base)
	    break;
	}

      found = 1;
      
      if (num > (~0UL - digit) / base)
	{
	  pupa_error (PUPA_ERR_OUT_OF_RANGE, "overflow is detected");
	  return 0;
	}

      num += num * base + digit;
      str++;
    }

  if (! found)
    {
      pupa_error (PUPA_ERR_BAD_NUMBER, "unrecognized number");
      return 0;
    }
  
  if (end)
    *end = (char *) str;

  return num;
}

char *
pupa_strdup (const char *s)
{
  pupa_size_t len;
  char *p;
  
  len = pupa_strlen (s) + 1;
  p = (char *) pupa_malloc (len);
  if (! p)
    return 0;

  return pupa_memcpy (p, s, len);
}

void *
pupa_memset (void *s, int c, pupa_size_t n)
{
  unsigned char *p = (unsigned char *) s;

  while (n--)
    *p++ = (unsigned char) c;

  return s;
}

pupa_size_t
pupa_strlen (const char *s)
{
  char *p = (char *) s;

  while (*p)
    p++;

  return p - s;
}

static inline void
pupa_reverse (char *str)
{
  char *p = str + pupa_strlen (str) - 1;

  while (str < p)
    {
      char tmp;

      tmp = *str;
      *str = *p;
      *p = tmp;
      str++;
      p--;
    }
}

char *
pupa_itoa (char *str, int c, unsigned n)
{
  unsigned base = (c == 'x') ? 16 : 10;
  char *p;
  
  if ((int) n < 0 && c == 'd')
    {
      n = (unsigned) (-((int) n));
      *str++ = '-';
    }

  p = str;
  do
    {
      unsigned d = n % base;
      *p++ = (d > 9) ? d + 'a' - 10 : d + '0';
    }
  while (n /= base);
  *p = 0;

  pupa_reverse (str);
  return p;
}

int
pupa_vsprintf (char *str, const char *fmt, va_list args)
{
  char c;
  int count = 0;
  auto void write_char (char c);
  auto void write_str (const char *s);
  
  void write_char (char c)
    {
      if (str)
	*str++ = c;
      else
	pupa_putchar (c);

      count++;
    }

  void write_str (const char *s)
    {
      while (*s)
	write_char (*s++);
    }
  
  while ((c = *fmt++) != 0)
    {
      if (c != '%')
	write_char (c);
      else
	{
	  char tmp[16];
	  char *p;
	  int n;
	  
	  c = *fmt++;
	  
	  switch (c)
	    {
	    case 'p':
	      write_str ("0x");
	      c = 'x';
	      /* fall through */
	    case 'x':
	    case 'u':
	    case 'd':
	      n = va_arg (args, int);
	      pupa_itoa (tmp, c, n);
	      write_str (tmp);
	      break;

	    case 'c':
	      n = va_arg (args, int);
	      write_char (n);
	      break;

	    case 's':
	      p = va_arg (args, char *);
	      if (p)
		write_str (p);
	      else
		write_str ("(null)");
	      break;

	    default:
	      write_char (c);
	      break;
	    }
	}
    }

  if (str)
    *str = '\0';
  
  return count;
}

int
pupa_sprintf (char *str, const char *fmt, ...)
{
  va_list ap;
  int ret;
  
  va_start (ap, fmt);
  ret = pupa_vsprintf (str, fmt, ap);
  va_end (ap);

  return ret;
}