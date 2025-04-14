---
title: We can do better than semver
tags:
  - programming languages
  - package management
  - kipos
categories:
  - Programming
date: 2025-04-14
gist: 532e48dc531c57cf405218b44d5a5de0
draft: true
---

> **Validated versioning**: making the contract between library author and consumer explicit and statically verifiable.

<!-- make unannounced breaking changes unrepresentable -->

Upgrading libraries is both (a) an annoying chore, and (b) has high uncertainty about how "safe" it is to do.
We rely on semantic versioning to signpost when a library undergoes a "breaking change", but this is far from reliable.

I think a lot of the issues around versioning & dependency management come down to the fact that **the contract between library author and consumer is currently "stringly typed"**: we rely on documentation, readmes, and changelogs (none of which can be statically validated) to indicate versioning guarantees. We can do better.

<!-- more -->

## Side note on surface area

On problem closely tied to versioning is that of "API surface area". In many languages (Java, Go, NodeJS, OCaml, Elm, Haskell, Koka, Scala), there's no way to have a "private module": if you want definitions to be usable in other modules in the package, they also are made available to consumers of the package. In some languages (Python, PHP, Unison) you can't even have private declarations, so the technical "surface area" of a library is *literally all of your code*. (Rust, Swift, Haskell+Cabal, and OCaml+Dune stand out as exceptions to this, although none goes as far as I want). [^go-far]

[^go-far]: Swift is unique in providing a specific access control specifier for "visible to consumers outside of this library". Rust has the ability to have public and private modules, but no way to specify that a given declaration should be "visible within this package, but not outside of it." You can arrange your code in such a way to achieve the same effect, but I would rather there be explicit support. Haskell (GHC) and OCaml don't support private modules *per se*, but with the addition of Cabal and Dune they effectively do.

This leaves you reliant on convention and documentation to indicate "what the publicly-dependable API consists of" (e.g. "don't import the module named Internal, it may change at any time").

Wouldn't it be nice if our languages actually had affordances for limiting the API surface area of a library?

## Automatic determination of breaking changes

Of course, once we've locked down the API surface area, there's still the problem of determining whether a version includes breaking changes. Why leave that up to changelogs, when we can do better?

Here's the vision: breaking changes are statically validatable through a combination of **type compatability** (what Elm does) and **test compatability** (I don't know of any language that does this).

1. if the type of a function changes in a non-backwards-compatible way, that's a breaking change
2. if any tests of that function from the previous version fail with the new version's implementation, that's a breaking change

In this way, **the test suite of the public API represents the behavioral guarantees of the library.**

This can also allow the package manager to isolate breaking changes to the specific declaration that they impact -- if your project doesn't use that function, then the version isn't a breaking change for you.

This also means that version numbers are not an indication of breaking changes; we can instead have them be an indicator of when the version was released, e.g. 20250414.

Now let's talk about resolution. The current state of things (with Rust, NPM, Pip) is that a library will specify a range of versions that it is compatible with. In general, this relies on semantic-versioning ranges, which are both too weak (stringly typed) and too restrictive (if the breaking change applies to a method I'm not using, it shouldn't be breaking for me).

What I envision is this: a library **A** specifies a single version for a dependency **B**. This is "the version it has been developed against". (in this way, it removes some uncertainty around "what is the /recommended/ version of a thing for a given library"). But because we have validated versioning, the package manager is free to install a newer version of dependency **B**, as long as it is determined to be backwards compatible with the requested version, and as long as all of **A**'s tests continue to pass.

To add a bit more flexibility, we could have the library also specify a "minimum compatible version" in addition to the "recommended version". There are certainly cases where you would want to have a lower version than a library might have been developed against, and it would be good for our dependency metadata to make that easy to do.

## How would lockfiles & resolutions work?

(I'm still at the "idea" stages for this, so I haven't fleshed everything out, but here goes)

Libraries would have lockfiles. In the same way that it's nice to know the specific version of a dependency that the library was developed against, I would want to download whole exact tree of dependencies that the library was developed against, absent any reason not to (such as a conflicting requirement from another library).

When specifying dependencies (in the equivalent of a `package.json` or `cargo.toml`), you would also be able to override the resolution of a transitive dependency, if desired.

## How do you deal with "multiple versions of the same package in your tree"

I've run into this issue in JavaScript (multiple versions of React having independent global state that causes errors), Rust (two types from different versions of a library leading to the confusing `"found a::b::Foo, expected a::b::Foo"`, and Java (I don't remember why, but my `build.gradle` has a bunch of `exclude group` declarations).

I think it ought to be statically analyzeable whether a library's use of a dependency is "leaky". This would either be due to (a) global state being accessed, or (b) some types that are declared in the dependency being "visible" in the library's public API. A "leaky" dependency would therefore need to be locked down so that there's only one instance of it between all its consumers, but a "non-leaky" dependency could be free to have multiple independent versions in use.

Of course, for code size reasons it could be desirable to have more deduplication than the static analysis deems necessary, so we might want to have the "isolated dependency" behavior be opt-in instead of opt-out.

## Migrations, can we have them

We can take our "breaking changes" story one step further by introducing [migration macros](https://welltypedwitch.bearblog.dev/the-way-were-thinking-about-breaking-changes-is-really-silly/). There is a class of breaking changes that can be alleviated by making a relatively simple source code transformation of each call site. Examples include: removal of an unused function parameter, changing the type of an argument from `int` to `Option<int>`, swapping the order of two arguments of the same type, etc.

When publishing a new version of your library, you would be able to specify migration macros for public API declarations that have had such signature changes, turning what would otherwise be a breaking change into a non-breaking one.

## What if nothing ever breaks?

A further extension of these ideas that I'm considering is to just never allow breaking changes. This is in line with [Golang's ethos](https://research.swtch.com/vgo-import), where a "new major version" of a package is treated as a "new package": instead of "auth", we're using "auth/v2". There's a definite appeal to this, in that it encourages a high commitment to compatability, while also allowing for a clean break when such is necessary. (in the absence of migrations, such a policy would be needlessly restrictive, so it's a good thing we've accounted for them)

One issue this would hopefully eliminate is the need to handle "un-breaking" versions; e.g. if version 20250411 had a breaking change, and then 20250412 "reverted" that breaking change, it would be great if a package manager were able to "know" that it can just skip over 20250411 and continue with compatible upgrades. This could add quite a bit of complexity however, and so it's appealing to just "eliminate casual breaking changes" out of hand.

## FAQ

> Why are you playing around with dependency management of a single language? You should be working on whole-system (cross-language) dependency management a la Nix.

I dunno, I want to have nice things? And a whole-system package manager can only be as smart as the dumbest language. I want smarter languages.

> These ideas aren't new

You're right! I've seen similar things in various comments sections, but I have yet to see a language actually implement them.

> [Packages should be hand-curated, not managed by soulless machines](https://odin-lang.org/docs/faq/#how-do-i-manage-my-code-without-a-package-manager)

I believe that the main critiques of package management are attributable to (1) having a language that is oblivious to the notion of a package, and (2) critiques of human-curated semver, which is precisely what I'm hoping to supplant. There's also (3), "trust no one, the world will betray you", and that's a social problem that it seems the Elm community is doing a good job of combatting.

> Doesn't this require the package-manager / compiler to be able to run the tests for a library? Should we really be baking testing into the language itself?

Yes, yes it does. If you're building a language that doesn't have unit testing built into it, what are you playing at.

> Doesn't Unison solve dependency management?

Unison is doing a lot of cool things, but there are two reasons it doesn't "solve" this: 1) unison is one of the languages that doesn't even allow for private declarations, let alone private modules; the "public api" of a library is literally all of the code in the library. 2) package management in unison is still quite simplistic, and upgrading a library is a very manual process. That said, unison is the best example I've seen of trying to "think bigger" as a language, tackling version control and distributed computing in addition to what we usually see, and so it might be well-placed to adopt some of these ideas.
