
"Package-aware Programming Language Design"
So many programming languages are oblivious to the distinction between "code in this application / library" and "code in dependency libraries". This is bad.

"Your Language should be more private"

Dependency management is the worst, in large part because so many programming languages seem were designed with package management as an afterthought, or even as something completely left up to third parties.
I've been pursuing the question "what would it look like to build a language / compiler in a way that made dependency management much much better?"

Let's talk about API surface area.


https://tonsky.me/blog/alpha/

#

What about a series of blog posts:

"An overview of the problesm with dependencies"
and then links to each
- api surface area, its a problem (here's how Kipos will solve it)
- semver is stringly typed, how to do better

## Better than semver


A response to "how would it look to have no breaking changes"
https://tonsky.me/blog/alpha/
-> my response is "if you want to make code available in pre-release form, have the version be github.com/your/repo#sha" and don't release a version yet

Another thing I want to mention:
- minor versions don't make sense. If I say "I want this library, but only the bugfixes", you're unlikely to have a maintainer backport bugfixes from 1.2.1 to 1.0.1. I mean it technically could happen, but that sounds like maintenance hell.
Q:
So there's like ... "api surface area" and there's "version surface area". How many versions do we expect people to be maintaining and adding bugfixes to.

Q: How does test compatability work? "Does the package manager do it, or the publisher"?
Q: Can this be integrated into an existing language, or do you need a new one?


---

Let's talk nuts and bolts:
- who enforces this? the package registry, probably
- how is that reasonable? well it would definitely help to have a language that can enforce side-effect-free code. (if your language can target wasm, then the requirement can be that the public-facing tests need to be runnable under wasm)



Problem statements:
- as a library maintainer, I want to fix a bug once, instead of having to fix it in several different (major/minor) branches in the repo
-



With Validated Versioning, how do you deal with things like a UI library that changes some styles?
Would those be breaking, or nonbreaking? Presumably, any "exported constant" that changes would comprise a breaking change, right?
> for those libraries, do we fall back on semver?


Let's talk about dependency resolutions.
The difference between (semver major bump) and (whole new library) is: semver major bump
can be adopted by changing one line in package.json.
(whole new library) involves changing every import-site.



A list of problems with dependency management:

- requires you to (a) trust another human or (b) do a lot of manual review
- version surface area -- if you have a library with multiple major and minor versions, where to bugfixes get applied?
- monorepo madness - is that the answer? break your package into bite-sized pieces, so that version bumps from one package don't "pollute" the others? but then what are compatability guarantees between your packages, and do you reqiure users to separately install and version each individual piece?

- 'feature' or 'breaking' changes to one part of a library "pollute" the whole library, such that fixing a bug "for all users" involves a patch release all actively supported major and minor versions that are impacted by the bug
  - one way to tackle this is to break up your package into many smaller packages, so you can bump the version of one sub-part without impacting the version of the others. this however introduces a combinatorial nightmare of compatability; how is a consumer to know which versions of which sub-packages are compatible? (peer-dependencies?maybe?)
- api surface area: does your language provide access control features necessary to allow you to make internal changes without them being "breaking changes".
- migrating from one version to the next is in general a task of unknown complexity and danger
- some languages don't allow you to have multiple versions of a package in the same project
- some libraries use global mutable state which breaks if you use multiple instances of the library




##

- [-] moving this all to js!
- [x] markdown rendering -- headers need <a>s with ids folks (ok they're different, but maybe thats ok? idk)
  - [x] see how many anchors changed

- [x] DARK MODE PLS
- [x] indicate which links are internal, and which ones are external
- [ ] try this font maybe https://fonts.google.com/specimen/Spectral?preview.size=23&category=Serif&stylecount=11
- [x] and thennnnn let's do some github gisting!
  - btw don't load the comments until they come into view. for simplicityyyess
  - ok, so I want a script to just download the gists to the posts folder
    and then we can render them, kinda like normal?
  - [x] the name being the gist ID is actually annoying, lets have it be an attribute on the frontmatter.
  - [ ] also, should my script actually upload the latest data to gist.xyz? hmmm

## [x] Homepage
- [x] talks
- [x] projects
- [x] posts

## [x] Projects
- [x] list
- [x] tags pages
- [x] individual pages

## [x] Posts
- [x] list
- [x] individual pages
- [x] tags pages

## [x] Talks
- [x] list
