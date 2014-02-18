
#include <emscripten/bind.h>
#include <pcrecpp.h>

// class Match {
// public:
//     const char *input;
//     std::vector<string> matches;
//
//     Match();
//
//     bool ParseFrom(const char *str, int n) {
//         matches.insert(matches.begin() + n, str);
//         return true;
//     }
//
//     const char *getInput() {
//         return input;
//     }
//
//     std::vector<string>getMatches() {
//         return matches;
//     }
// };

struct Match {
    string input;
    std::vector<string> matches;
    bool matched;
};

#define MAX_SUBMATCHES 50
Match RE__match(pcrecpp::RE &re, const string input_, pcrecpp::RE::Anchor anchor) {
    Match match = { input_, {{}}, false };
    bool matched = false;

    int n = re.NumberOfCapturingGroups();
    if (n < 0) {
        return match;
    } else if (n > MAX_SUBMATCHES){
        fprintf(stderr, "Overflow: There are too many capturing groups\n");
        return match;
    }

    string matches[MAX_SUBMATCHES];
    const pcrecpp::Arg *args[MAX_SUBMATCHES];
    for (int z = 0; z < MAX_SUBMATCHES; z++) {
        pcrecpp::Arg arg0 = &matches[z];
        args[z] = &arg0;
    }

    pcrecpp::StringPiece input(input_);
    int consumed;
    while ((matched = re.DoMatch(input, pcrecpp::RE::UNANCHORED, &consumed, args, n))) {
        match.matched = true;
        printf("consumed: %d\n", consumed);
        input.remove_prefix(consumed);
        for (int t = 0; t < (n + 1); t++){
            printf("%s\n", matches[t].c_str());
            match.matches.push_back(matches[t]);
        }
    }
    return match;
}

EMSCRIPTEN_BINDINGS(pcrecpp) {
    // emscripten::class_<Match>("RE_Match")
    //     .constructor<>()
    //     .property("input", &Match::getInput)
    //     .property("matches", &Match::getMatches)
    //     ;

    emscripten::value_object<Match>("RE_Match")
        .field("input", &Match::input)
        .field("matches", &Match::matches)
        .field("matched", &Match::matched)
        ;

    emscripten::enum_<pcrecpp::RE::Anchor>("Anchor")
        .value("UNANCHORED", pcrecpp::RE::UNANCHORED)
        .value("ANCHOR_START", pcrecpp::RE::ANCHOR_START)
        .value("ANCHOR_BOTH", pcrecpp::RE::ANCHOR_BOTH)
        ;

    emscripten::class_<pcrecpp::RE>("RE")
        .constructor<std::string>()
        .function("match", &RE__match)
        ;

    emscripten::register_vector<string>("VectorString");
}
