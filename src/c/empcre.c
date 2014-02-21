
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "empcre.h"

RE *RE_make() {
    RE *re = malloc(sizeof(RE));
    re->source = malloc(sizeof(char *));
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
    free(m);
}

RE *RE_new(const char *pattern) {
    const char *pcreErrorStr;
    int pcreErrorOffset;
    pcre *reCompiled;
    pcre_extra *pcreExtra;

    reCompiled = pcre_compile(pattern, 0, &pcreErrorStr, &pcreErrorOffset, NULL);
    if (reCompiled == NULL) {
        printf("ERROR: Could not compile '%s': %s\n", pattern, pcreErrorStr);
        exit(1);
    }

    pcreExtra = pcre_study(reCompiled, 0, &pcreErrorStr);
    if (pcreErrorStr != NULL) {
        printf("ERROR: Could not optimize '%s': %s\n'", pattern, pcreErrorStr);
        exit(1);
    }

    RE *result = RE_make();
    strcpy((char *)result->source, pattern);
    result->re = reCompiled;
    result->extra = pcreExtra;

    return result;
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

    for(int i=0; i < result; i++) {
        pcre_get_substring(input, subStrVec, result, i, &(match->matches[i]));
    }

    return match;
}

int REMatch_get_length(REMatch *match) {
    return match->length;
}

const char *REMatch_get_match_at(REMatch *match, int i) {
    return match->matches[i];
}

