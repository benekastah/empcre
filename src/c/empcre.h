
#include <pcre.h>

#define MAX_MATCHES 100

typedef struct {
    pcre *re;
    pcre_extra *extra;
    const char *source;
} RE;

typedef struct {
    const char *input;
    int length;
    const char *matches[MAX_MATCHES];
} REMatch;

RE *RE_make();
//@export
void RE_destroy(RE *re);

REMatch *REMatch_make();
//@export
void REMatch_destroy(REMatch *m);

//@export
RE *RE_new(const char *pattern);

//@export
REMatch *RE_match(RE *re, const char *input, int startoffset);

//@export
int REMatch_get_length(REMatch *match);
//@export
const char *REMatch_get_match_at(REMatch *match, int i);
