/* MonCalls */

// 16-bit prototypes
#define  INCL_DOSMONITORS
#include <os2.h>

// 32-bit prototypes (EMXWRAP/SUB32 style)
USHORT APIENTRY Dos32MonClose(const HMONITOR hmon);
USHORT APIENTRY Dos32MonOpen(const PSZ a, HMONITOR * hmon);
USHORT APIENTRY Dos32MonRead(const PBYTE a, const USHORT b, const PBYTE c, const PUSHORT d);
USHORT APIENTRY Dos32MonReg(const HMONITOR hmon, const PBYTE b, const PBYTE c, const USHORT d, const USHORT e);
USHORT APIENTRY Dos32MonWrite(const PBYTE a, const PBYTE b, const USHORT c);


USHORT APIENTRY16 DOS16MONCLOSE(const HMONITOR hmon)
{
  return Dos32MonClose(hmon);
}


USHORT APIENTRY16 DOS16MONOPEN(const PSZ a, HMONITOR * hmon)
{
  return Dos32MonOpen(a, hmon);
}


USHORT APIENTRY16 DOS16MONREAD(const PBYTE a, const USHORT b, const PBYTE c, const PUSHORT d)
{
  return Dos32MonRead(a, b, c, d);
}


USHORT APIENTRY16 DOS16MONREG(const HMONITOR hmon, const PBYTE b, const PBYTE c, const USHORT d, const USHORT e)
{
  return Dos32MonReg(hmon, b, c, d, e);
}


USHORT APIENTRY16 DOS16MONWRITE(const PBYTE a, const PBYTE b, const USHORT c)
{
  return Dos32MonWrite(a, b, c);
}

