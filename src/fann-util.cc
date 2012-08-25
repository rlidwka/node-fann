/*
 *   Utility functions
 */

#include <v8.h>
#include <node.h>
//#include <doublefann.h>
#include <fann.h>
#include <string.h>
#include <ctype.h>
#include "node-futil.h"

using namespace v8;
using namespace node;

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
	strncpy_lower(algname, origname, 63);
	return scope.Close(String::New(algname));
}

int _SeekCharArray(Local<String> value, const char* const* array, int size, const char* prefix)
{
	char name[64];
	char name2[128];
	value->WriteAscii(name, 0, 63);
	strcpy(name2, prefix);
	strcat(name2, name);
	for (int i=0; i<size; i++) {
		if (strcasecmp(name, array[i]) == 0 || strcasecmp(name2, array[i]) == 0) {
			return i;
		}
	}
	return -1;
}

