(function () {
/**
 * This file currently just contains code to test the emscripten bindings.
 * Ultimately, it will provide some sort of interface / helper functions.
 */

var re = new Module.RE('h.*o');
var m = re.match('jello', Module.Anchor.UNANCHORED);

})();
