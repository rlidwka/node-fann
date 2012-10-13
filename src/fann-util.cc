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
	} while (*dest++ = tolower(*src++));
	while (n--)
		*dest++ = 0;
	return ret;
}

Handle<Value> NormalizeName(const char* origname, const char* prefix, int prefix_len)
{
	HandleScope scope;
	char algname[64];
	if (strncmp(origname, prefix, prefix_len) == 0) {
		origname +=	prefix_len;
	} 
//	strncpy(algname, origname, 63);
	strncpy_lower(algname, origname, 64);
	algname[63] = 0;
	return scope.Close(String::New(algname));
}

int _SeekCharArray(Local<String> value, const char* const* array, int size, const char* prefix)
{
	char name[64];
	char name2[128];
	value->WriteAscii(name, 0, 64);
	strcpy(name2, prefix);
	name[63] = 0;
	strncat(name2, name, 64);
	name2[127] = 0;
	for (int i=0; i<size; i++) {
		if (strcasecmp(name, array[i]) == 0 || strcasecmp(name2, array[i]) == 0) {
			return i;
		}
	}
	return -1;
}

