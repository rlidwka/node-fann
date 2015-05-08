/*
 *   Utility functions
 */

#include <string.h>
#include <ctype.h>
#include "node-futil.h"

char* strncpy_lower(char *dest, const char *src, size_t n)
{
  char *ret = dest;
  do {
    if (!n--)
      return ret;
  } while ((*dest++ = tolower(*src++)) != 0);
  while (n--)
    *dest++ = 0;
  return ret;
}

Handle<Value> NormalizeName(const char* origname, const char* prefix, int prefix_len)
{
  char algname[64];
  if (strncmp(origname, prefix, prefix_len) == 0) {
    origname += prefix_len;
  }
//  strncpy(algname, origname, 63);
  strncpy_lower(algname, origname, 64);
  algname[63] = 0;
  return NanNew<String>(algname);
}

int _SeekCharArray(Local<String> _value, const char* const* array, int size, const char* prefix)
{
  String::Utf8Value value(_value);
  char name[128];
  strncpy(name, prefix, 64);
  name[63] = 0;
  strncat(name, *value, 64);
  name[127] = 0;
  for (int i=0; i<size; i++) {
    if (strcasecmp(*value, array[i]) == 0 || strcasecmp(name, array[i]) == 0) {
      return i;
    }
  }
  return -1;
}

