
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "empcre.h"

RE *RE_make() {
    RE *re = malloc(sizeof(RE));
    re->source = malloc(sizeof(char *));
    re->ignoreCase = 0;
    re->multiline = 0;
    re->dotall = 0;
    re->jsCompat = 0;
    return re;
}

void RE_destroy(RE *re) {
    free((void*)re->source);
    pcre_free(re->re);
    pcre_free(re->extra);
    free(re);
}

REMatch *REMatch_make() {
    REMatch *m = malloc(sizeof(REMatch));
    m->input = malloc(sizeof(char *));
    m->length = 0;
    return m;
}

void REMatch_destroy(REMatch *m) {
    free((void*)m->input);
    pcre_free_substring_list(m->matches);
    free(m);
}

int RE_parseFlags(RE *re, const char *flags) {
    int options = 0;

    char flag;
    for (int i = 0; (flag = flags[i]); i++) {
        if (flag == 'i') {
            options |= PCRE_CASELESS;
            re->ignoreCase = true;
        } else if (flag == 'm') {
            options |= PCRE_MULTILINE;
            re->multiline = true;
        } else if (flag == 's') {
            options |= PCRE_DOTALL;
            re->dotall = true;
        } else if (flag == 'j') {
            options |= PCRE_JAVASCRIPT_COMPAT;
            re->jsCompat = true;
        }
    }

    return options;
}

RE *RE_new(const char *pattern, const char *flags) {
    const char *pcreErrorStr;
    int pcreErrorOffset;
    pcre *reCompiled;
    pcre_extra *pcreExtra;

    RE *result = RE_make();
    int options = RE_parseFlags(result, flags);

    reCompiled = pcre_compile(pattern, options, &pcreErrorStr, &pcreErrorOffset, NULL);
    if (reCompiled == NULL) {
        printf("ERROR: Could not compile '%s': %s\n", pattern, pcreErrorStr);
        exit(1);
    }

    pcreExtra = pcre_study(reCompiled, 0, &pcreErrorStr);
    if (pcreErrorStr != NULL) {
        printf("ERROR: Could not optimize '%s': %s\n'", pattern, pcreErrorStr);
        exit(1);
    }

    strcpy((char *)result->source, pattern);
    result->re = reCompiled;
    result->extra = pcreExtra;

    return result;
}

bool RE_hasFlag(RE *re, const char *flag) {
    if (strcmp(flag, "ignoreCase") == 0) {
        return re->ignoreCase;
    } else if (strcmp(flag, "multiline") == 0) {
        return re->multiline;
    } else if (strcmp(flag, "dotall") == 0) {
        return re->dotall;
    } else if (strcmp(flag, "jsCompat") == 0) {
        return re->jsCompat;
    } else {
        printf("RE has no flag '%s'\n", flag);
        exit(1);
    }
}

REMatch *RE_match(RE *re, const char *input, int startoffset) {
    int subStrVec[MAX_MATCHES];
    int result;

    result = pcre_exec(re->re,
            re->extra,
            input,
            strlen(input),
            startoffset,
            0,               // OPTIONS
            subStrVec,
            MAX_MATCHES); // Length of subStrVec

    REMatch *match = REMatch_make();

    if (result < 0) {
        switch (result) {
            case PCRE_ERROR_NOMATCH:
                return match;
                break;
            case PCRE_ERROR_NULL:
                printf("Something was null\n");
                exit(result);
                break;
            case PCRE_ERROR_BADOPTION:
                printf("A bad option was passed\n");
                exit(result);
                break;
            case PCRE_ERROR_BADMAGIC:
                printf("Magic number bad (compiled re corrupt?)\n");
                exit(result);
                break;
            case PCRE_ERROR_UNKNOWN_NODE:
                printf("Something kooky in the compiled re\n");
                exit(result);
                break;
            case PCRE_ERROR_NOMEMORY:
                printf("Ran out of memory\n");
                exit(result);
                break;
            default:
                printf("Unknown error\n");
                exit(result);
                break;
        }
    }

    match->length = result;
    strcpy((char *)match->input, input);
    pcre_get_substring_list(input, subStrVec, result, &(match->matches));

    return match;
}

int REMatch_get_length(REMatch *match) {
    return match->length;
}

const char *REMatch_get_match_at(REMatch *match, int i) {
    return match->matches[i];
}

