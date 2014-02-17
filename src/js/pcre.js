(function () {

var empcre_make_re = Module.cwrap('empcre_make_re', 'number', ['string']);
var empcre_match = Module.cwrap('empcre_match', 'number', ['number', 'string']);

window.PCRE = PCRE;

function PCRE(pattern) {
    this.rePtr = empcre_make_re(pattern);
}

PCRE.prototype.match = function (input) {
    var matchPtr = empcre_match(this.rePtr, input);
    return matchPtr;
};

})();
