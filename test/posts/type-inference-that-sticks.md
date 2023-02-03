---
title: Type inference that sticks
tags:
  - programming languages
  - unison
  - compiler
  - jerd
categories:
  - Programming
date: 2023-02-02
draft: true
gist: 13abec33c7576c4636ca87039a999cf2
---

*What if type inference felt like a real-time conversation, instead of emailing code back and forth?*

With type inference, compilers are faced with a difficult task: intuiting what types a user *had in mind* (but didn't write down) while writing some code. It all falls apart when the types that the algorithm assigns are different from those the user expected, and especially so when the type error reported is far away from the site of mental mismatch.

<!-- more -->

## Amnesiac type inference

Current type inference algorithms perform just as well (and just as poorly) on code entered & modified over time in an IDE as they do on a text file read from a disk. Or, for that matter, on code written with pen & paper and then fed to a compiler character by character via teletype.

That is to say, they have no knowledge of, or interest in, the process by which the code was written. The only feedback loop they recognize is receiving all of the code at once, and responding with the first error they come across, or if you're lucky, several independent errors. Fixing a given error, instead of being a context-sensitive conversation ("here's the resolution to that error, please continue"), is back to a clean-slate data dump of all the code (modulo some caching for performance, but caching does not -- must not -- change the semantics of type inference).

## To annotate or not to annotate

When tracking down a hard-to-find type error, one solution is to start nailing things down: add in the types that you had in mind in the first place, and see where the compiler got tripped up. On the other hand, you could have just written in those types all along, and saved yourself the trouble.

So there's this tradeoff between brevity and verbosity of the language syntax. The more information included in the source code, the less guess-work the compiler / type checker has to do, and the less chance there is for a mismatch between the programmer's mental model and the computer's. But more verbosity results in not only more typing, but it can also disrupt the flow of writing code. In an exploratory mode, the user doesn't necessarily know the types of function arguments before they have written the body of the function. And then there's readability tradeoffs on both sides of the spectrum: for experienced programmers, having types written explicitly all over the place can be very noisy and actually inhibit code comprehension, while those same annotations are a boon for someone unfamiliar with the codebase.

There are also times when inference algorithm limitations have negative impacts on other aspects of the language. Flow, facebook's type checker for javascript, originally had global type inference, but in the interest of speed (especially on large codebases such as facebook has) recent versions require you to annotate all functions exported from a file, even when those functions have trivially inferrable types. Ocaml's type inference covers most of the language, but type-abstracted modules (known as Functors) generally cannot be inferred, and need explicitly type annotations.

Roc, on the other hand, follows Elm's admirable commitment to having all type annotations be optional, such that any correct program can have all of the annotations removed and still have the same semantics. The trade-off there is that Roc doesn't support Rank-2 polymorphism (you can't pass a generic function as a function argument), because it wouldn't be able to be fully inferred. Haskell on the other hand, never one to compromise on power, allows you to turn on "Rank-N" types, under which type inference is [undecideable in the general case](https://en.wikipedia.org/wiki/Parametric_polymorphism#Higher-rank_polymorphism).

## A sufficiently interactive compiler

With jerd, I'm trying to build a compiler that's "sufficiently interactive" instead of [sufficiently smart](http://wiki.c2.com/?SufficientlySmartCompiler). Instead of trying to reconstruct the programmer's intentions from a text-dump of source code, the compiler is involved in the entire process of development, both responding to programmer action and giving context-aware feedback in return. In this model, the source code is enriched with the compiler's inferences along the way, which the programmer can inspect & correct (but which can also be elided from display for a more compact presentation). Type inference is therefore *incremental* and *reified in the program source*, meaning that, once a function has been written, the compiler need make no guesses about types or the identities of referenced terms.

As a simple example, consider autocomplete. Isn't it funny that autocomplete can enable you to choose exactly what function you want to call, and what local variable you want to reference, but then when the compiler comes along all it has to go on is the autocompleted text? It has to duplicate all of the work autocomplete did, constructing local scopes and performing top-level name resolution, and if the term has since been renamed then it just falls over.

When typing an identifier in jerd, if you select something from autocomplete, the ID of that term is stuck onto the identifier node (e.g. `myFn#4ea8ff`). This way, the compilation pass doesn't need to do any name resolution -- it knows exactly what is being referenced. And, if you later change the name of a variable, none of the references are broken. (In jerd, references are by a hash of the definition's syntax tree instead of by name, similar to what [unison](https://www.unison-lang.org/learn/the-big-idea/) does.)

## Sticky type inference

Say you're writing a function with two arguments. Before the body is defined,
the arguments have the universal type "𝕌".

The interface *looks* like this:
```clojure
(defn movieFromLine [line idx]
    )
```
But the underlying data has reified ("sticky") type information for all identifiers, something like this:
```clojure
(defn movieFromLine [line {id: 0, type: 𝕌}
                     idx  {id: 1, type: 𝕌}]
    )
```

Then, when you use one of the arguments, the type annotation for that
argument gets updated, if possible, to the intersection of the current
annotation and the type required by the new usage.

What you see:
```clojure
(defn movieFromLine [line idx]
    (switch (split line "!")
        ))
```

What the compiler sees (again, this isn't *inferred* by the compiler, it's actually persisted in the source tree):
```clojure
(defn movieFromLine [line {id: 0, type: string}
                     idx  {id: 1, type: 𝕌}]
    (switch (split {id: '#ead31a8',
                    type: (fn [string string] (array string))}
                line {id: 0}
                "!")
        ))
```

If there is no intersection, then the new usage is marked as an error,
but you're also given the option (via dropdown) to switch the annotation to align with
your new usage, and marking any other incompatible usages as erroneous.

See how it's a conversation?

This functionality is intimitely connected to the fact that jerd is a **projectional** language,
and that there is no provision for editing source code as "flat text". Because of this,
the compiler can rely on having **much more information** persisted in the source tree, which would
otherwise be provibitively verbose if it had to be printed & parsed as syntax. Similarly, interactions
with the compiler are no longer text-first with optional enhancement by a separate IDE; the compilation process is interactive from the ground up, such that ambiguity doesn't have to be an error -- it can just be a dropdown.

## Prior Art

Lamdu's editor has [a similar interaction to this](https://www.youtube.com/watch?v=viF1bVTOO6k&t=1s), where if you add a new use of a variable that's incompatible with current uses, then the new usage is marked as the error. I don't know if they store the inferred types in the syntax tree, or if they re-compute them on every edit.

## Further reading

I'm hoping to write quite a bit more about jerd as I'm fleshing out the language and development environment, but in the meantime you can take a look at the WIP documentation for the [type system](https://github.com/jaredly/j3/blob/main/docs/Types.md) (which borrows quite a bit from [Roc](https://www.roc-lang.org/)) and the [algebraic effects](https://github.com/jaredly/j3/blob/main/docs/Algebraic%20Effects.md) setup, which is inspired by, but fairly different from, that of [unison](https://www.unison-lang.org/learn/fundamentals/abilities/) and [eff](https://www.eff-lang.org/handlers-tutorial.pdf).

This whole endeavor is quite experimental, so if you have any feedback or suggestions, I'd love to hear them 😄 leave a comment here, or message me on [mastodon](https://mastodon.social/@jaredly).
