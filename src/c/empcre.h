
#include <pcre.h>

#define MAX_MATCHES 100

typedef struct {
    pcre *re;
    pcre_extra *extra;
    const char *source;
    bool ignoreCase;
    bool multiline;
    bool dotall;
    bool jsCompat;
} RE;

typedef struct {
    const char *input;
    int length;
    const char **matches;
} REMatch;

RE *RE_make();
//@export
void RE_destroy(RE *re);
//@export
RE *RE_new(const char *pattern, const char *flags);
//@export
bool RE_hasFlag(RE *re, const char *flag);

REMatch *REMatch_make();
//@export
void REMatch_destroy(REMatch *m);
//@export
REMatch *RE_match(RE *re, const char *input, int startoffset);
//@export
int REMatch_get_length(REMatch *match);
//@export
const char *REMatch_get_match_at(REMatch *match, int i);
