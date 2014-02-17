
#include <pcre.h>

#define MAX_MATCHES 30

typedef struct {
    pcre *re;
    pcre_extra *extra;
    const char *source;
} RegExp;

typedef struct {
    const char *input;
    const char *matches[MAX_MATCHES];
} RegExpMatch;

