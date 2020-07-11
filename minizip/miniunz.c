#include <direct.h>

#define mkdir _mkdirthunk
#define chdir _chdirthunk

int
_mkdirthunk(
	nwn2dev__in const char * dirname
	);

int
_chdirthunk(
	nwn2dev__in const char * dirname
	);

#include "../zlib/contrib/minizip/miniunz.c"
