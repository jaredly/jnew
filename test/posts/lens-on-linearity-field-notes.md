---
title: "Laminar Field Nodes: Implementing Linear Flow Typing"
tags:
- compiler
- programming languages
- pl research
date: 2026-10-20
draft: false
gist: f912a640cfb24467554b7c6b194dd790
---

For an overview of the paper this is based on, [check out this post](/posts/papers-we-love-lens-on-linearity/).

Here are my notes about trying to implement a linear/flow language in typescript, compiling to javascript. It uses runtime checks to enforce lienarity; so far there is no type checker.

<!-- more -->

## Their example implementation

[zip file from Graham Hutton](https://people.cs.nott.ac.uk/pszgmh/flow-typing-supplementary.zip)

### It's in rust, targetting liquid haskell

I imagine targetting liquid haskell allows them to verify that what they're creating really does conform to linear typing guarantees.

### It's only a partial implementation

They've implemented a number of key parts of the described flow-typing system, but it's far from complete, which is frustrating.

Of the syntax described in the paper, here's what they have implemented. For the parts that they have implemented, I can check my runtime characteristics against theirs; for the rest, I have to rely on my understanding of the paper 😭.

```
pat
---

vbl
(pat, pat)
()

lin
---

// Implemented
vbl
lens := lin
unlens lens in lin
let pat = lin in lin
let pat = ~lens in lin
fn(pat) => lin
fn(pat) => ~lens
lin(lin)
lin(~lens)
(lin, lin)
()

// They've implemented, I have not
Left(lin)
Right(lin)
case lin
    Left(pat) -> lin
    Right(pat) -> lin
case ~lens
    Left(pat) -> lin
    Right(pat) -> lin

// Not implemented
false(term)
lin & lin
fst(lin)
snd(lin)
true(lin)
promote
discard
copy
derelict

lens
----

// Implemented
vbl
lin(lin)
lin(~lens)
(lens, lens)
()

// I've implemented, they have not
lens vbl <- lin in lin
let pat = lin in lens
let pat = ~lens in lens

// Not Implemented
case lin
    Left(pat) -> lens
    Right(pat) -> lens
case ~lens
    Left(pat) -> lens
    Right(pat) -> lens
false(term)
```

### Open questions not handled by the paper

What do we do about local lambdas that close over state? Their implementation treats first-class functions as exempt from linear checks (including self-recursion).

It seems like there would need to be two classes of functions, one that is re-usable and one that is not, but that would make the usage of first class functions really cumbersome. But maybe I'm misunderstanding something?

### Things I don't understand

- what does `false(term)` mean? is it like `throw/fatal`? It looks like it's able to produce any type, which is consistent with exception throwing.
- what do `promote/discard/copy/derelict` mean? Apparently they're terms from linear typing. They don't have any practical examples that make use of them, unfortunately.
- does `unlens` have any practical use? It looks like it might be a term that's just used for proving that their flow language is isomorphic to the standard linear language

### Mistakes in the paper

- There are a few places where they use lens syntax in a function body without wrapping it in a `~()`.
- There's also a `lens <-` construct whose body doesn't evaluate to `()`, which is type-invalid based on their definition of the `lens <-` form (the body must have type 1).

### Compiler bugs in their implementation

```
swap2 :: (a,b)/(a,b) -> ~(a, a)
swap2 ~(x, y) = ~(x, x)
-- turns into --
swap2 = \(x, y) -> ((x, ()), \resv -> case resv of (tres, tres0) -> (tres0, y))
```

the first part of the return value `(x, ())` is expected to have type `(a, a).`

```
callnul2 :: ~Int -> ~()
callnul2 ~inputv = ~(makenul ())
-- turns into --
callnul2 = \inputv -> case makenul () of (ab, ares) -> case ares () of () -> (ab, \resv -> case inputv resv of v -> v)
```

pretty sure you're not supposed to be calling `inputv` as a function

```
callnul :: ~Int -> ~(Int, ())
callnul ~inputv = ~(inputv, makenul ())
-- turns into --
callnul = \inputv -> case makenul () of
  (ab, ares) -> case ares () of
    () -> ((inputv, ab), \resv -> case resv of (tres, tres0) -> case () tres0 of () -> tres)
```

Can't be calling `()` as a function in `case () tres0`

## My Implementation

[repo](https://github.com/jaredly/laminar)

Currently I'm only implementing the runtime semantics, while entirely bypassing the type checking and inference steps. This is made possible by the fact that the semantics are purely derivable from the syntax; every ~ borrow is explicit, and so it is always locally knowable whether a function takes and/or returns a flow-variable, without doing any kind of analysis.

I'm using a c-style (vaguely rust/go/ts) syntax as that is what I'm most used to. I also have a ready-to-go lexer that I've been meaning to turn into a reusable library instead of just copying from project to project.

My target language is typescript, and in a critical departure from their implementation, I'm doing explicit mutable references (mediated by a global "scope" object) instead of translating to the threading-style required by liquid haskell. I'm doing this in an attempt to see if this can be implenented in a moderately performance-conscious way.

### What would come next

My real goal is to determine whether a language with linear/flow semantics as actually nice to use. Some things I can imagine wanting, to that end:

- syntax sugar for `dup(~x)`, as I would probably reach for that a lot
- mayyybe a way to have two distinct kinds of variables? "linear/flow vbls" and "immutable vbls"? That's how rust does things, broadly speaking (mut vs normal)

If/when I determine the language is fun to use, then I'll work on type checking & inference.

I also want to try targetting c, or go, or something like that; some language that has pointers. It could be cool to have malloc/free managed by flow variables to guarantee memory safety.
