
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "empcre.h"

RegExp *MakeRegExp() {
    RegExp *re = malloc(sizeof(RegExp));
    re->source = malloc(sizeof(char *));
    return re;
}

void DestroyRegExp(RegExp *re) {
    free((void*)re->source);
    pcre_free(re->re);
    pcre_free(re->extra);
    free(re);
}

RegExpMatch *MakeRegExpMatch() {
    RegExpMatch *m = malloc(sizeof(RegExpMatch));
    m->input = malloc(sizeof(char *));
    return m;
}

void DestroyRegExpMatch(RegExpMatch *m) {
    free((void*)m->input);
    free(m);
}

RegExp *empcre_make_re(const char *pattern) {
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

    RegExp *result = MakeRegExp();
    strcpy((char *)result->source, pattern);
    result->re = reCompiled;
    result->extra = pcreExtra;

    return result;
}

RegExpMatch *empcre_match(RegExp *re, const char *input) {
    int subStrVec[MAX_MATCHES];
    int result;

    printf("Testing '%s' against the pattern '%s'...\n", input, re->source);

    result = pcre_exec(re->re,
            re->extra,
            input,
            strlen(input),   // length of string
            0,               // Start looking at this point
            0,               // OPTIONS
            subStrVec,
            MAX_MATCHES); // Length of subStrVec

    if (result < 0) {
        switch (result) {
            case PCRE_ERROR_NOMATCH:
                printf("String did not match the pattern\n");
                break;
            case PCRE_ERROR_NULL:
                printf("Something was null\n");
                break;
            case PCRE_ERROR_BADOPTION:
                printf("A bad option was passed\n");
                break;
            case PCRE_ERROR_BADMAGIC:
                printf("Magic number bad (compiled re corrupt?)\n");
                break;
            case PCRE_ERROR_UNKNOWN_NODE:
                printf("Something kooky in the compiled re\n");
                break;
            case PCRE_ERROR_NOMEMORY:
                printf("Ran out of memory\n");
                break;
            default:
                printf("Unknown error\n");
                break;
        }
        exit(1);
    }

    RegExpMatch *match = MakeRegExpMatch();
    strcpy((char *)match->input, input);

    for(int i=0; i < result; i++) {
        pcre_get_substring(input, subStrVec, result, i, &(match->matches[i]));
    }

    return match;
}

