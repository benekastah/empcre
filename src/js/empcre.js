(function (Module) {

    var method = function (cls, ptrProp, methodName, ret, args) {
        var fn = Module.cwrap(cls.name + methodName, ret, args);
        cls.prototype[methodName] = function () {
            var ptr = this[ptrProp];
            var args = [ptr && ptr.call ? ptr.call(this) : ptr];
            args.push.apply(args, arguments);
            return fn.apply(null, args);
        };
    };

    var REMatch_destroy = Module.cwrap('REMatch_destroy', 'null', ['number']);
    var REMatch_get_length = Module.cwrap('REMatch_get_length', 'number', ['number']);
    var REMatch_get_match_at = Module.cwrap('REMatch_get_match_at', 'string', ['number']);

    window.RE = RE;

    function RE(pattern, flags) {
        this.pattern = pattern;
        this.flags = flags || '';
        this.buildRe();
    }

    RE.method = function (methodName, ret, args) {
        method(this, '_re', methodName, ret, args);
    };

    RE._new = Module.cwrap('RE_new', 'number', ['string', 'string']);
    RE.method('_destroy', 'null', ['number']);
    RE.method('_new', 'number', ['string', 'string']);
    RE.method('_match', 'number', ['number', 'string', 'number']);
    RE.method('_hasFlag', 'number', ['number', 'string']);

    RE.prototype.toString = function () {
        return '/' + this.pattern + '/' + this.flags;
    };

    RE.prototype.buildRe = function () {
        if (!this._re) {
            console.warn('Allocating memory for ' + this);
            this._re = RE._new(this.pattern, this.flags);
            this.ignoreCase = this._hasFlag('ignoreCase');
            this.multiline = this._hasFlag('multiline');
            this.dotall = this._hasFlag('dotall');
            this.jsCompat = this._hasFlag('jsCompat');
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
        console.warn('Cleaning up memory for ' + this);
        this._destroy();
        this._re = null;
    };

    RE.prototype.match = function (input, startOffset) {
        this.buildRe();
        var matchPtr = this._match(input, startOffset || 0);

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

})(Module);
