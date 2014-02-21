(function () {

    var RE_destroy = Module.cwrap('RE_destroy', 'null', ['number']);
    var REMatch_destroy = Module.cwrap('REMatch_destroy', 'null', ['number']);
    var RE_new = Module.cwrap('RE_new', 'number', ['string']);
    var RE_match = Module.cwrap('RE_match', 'number', ['number', 'string', 'number']);
    var REMatch_get_length = Module.cwrap('REMatch_get_length', 'number', ['number']);
    var REMatch_get_match_at = Module.cwrap('REMatch_get_match_at', 'string', ['number']);

    window.RE = RE;

    function RE(pattern) {
        this.pattern = pattern;
    }

    RE.prototype.re = function () {
        if (!this._re) {
            console.warn('Allocating memory for /' + this.pattern + '/');
            this._re = RE_new(this.pattern);
        }

        // Try to manage memory automatically
        var that = this;
        clearTimeout(this._destroyTimeout);
        this._destroyTimeout = setTimeout(function () {
            that.destroy();
        }, 5000);

        return this._re;
    };

    RE.prototype.destroy = function () {
        console.warn('Cleaning up memory for /' + this.pattern + '/');
        RE_destroy(this._re);
        this._re = null;
    };

    RE.prototype.match = function (input, startOffset) {
        var matchPtr = RE_match(this.re(), input, startOffset || 0);

        var matches = {
            length: 0,
            input: input
        };

        for (var i = 0, len = REMatch_get_length(matchPtr); i < len; i++) {
            Array.prototype.push.call(matches, REMatch_get_match_at(matchPtr, i));
        }

        REMatch_destroy(matchPtr);
        return matches.length ? matches : null;
    };

    RE.prototype.executor = function (input) {
        function REExecutor() {
            this.input = input;
            this.offset = 0;
            var match = this.match;
            this.match = function () {
                var result = match.call(this, this.input, this.offset);
                if (result) {
                    result.offset += result[0].length;
                }
                return result;
            };
        };
        REExecutor.prototype = this;
        return new REExecutor;
    };

})();
