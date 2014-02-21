Empcre
======

[PCRE](http://www.pcre.org/) via javascript (compiled with
[emscripten](https://github.com/kripken/emscripten/wiki)).

This is just a proof-of-concept that PCRE can work in the browser. I may flesh
out the API as time goes on. The idea to make this came from a conversation on
the [Jison](http://zaach.github.io/jison/) mailing list about the missing
"sticky" flag in many javascript RegExp implementations (I think only Firefox
has it?). The presence of this flag could greatly speed up parsing large files
in javascript. I have not yet determined if that will be possible through PCRE
and will look into that soon.

You don't have to build this yourself; for convenience I have included the
build output in the `dist` directory.

If you're curious how to use this, feel free to look at the `example`
directory.
