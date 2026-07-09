
We encourage potential speakers to submit more than one submission on more than one topic.
The more submissions you contribute, the more options you give us to pick you.
Make sure your talk title and abstract
- define the exact topic you want to cover and
- what you hope people will learn from the session.
In previous years, some submissions were really interesting but **not detailed enough** for them to make the final cut.



Jared thinks that programming is unreasonably complicated and difficult, and is working on research projects to make it less so. He does Mobile and Web development at Khan Academy, working to bring free education to everyone. He enjoys gardening, playing on the playground, and coaxing computers into making pretty pictures.


I've got a 2-month old, so transport & lodging for my wife would be much appreciated!




# Algebraic Effects Demystified

Algebraic Effects provide an elegant solution to two seemingly unrelated problems -- asynchronous coordination and untrusted side-effect sandboxing -- both of which have seen a lot of development in the Javascript world in recent years.
Unfortunately, really gaining a mastery over this novel language feature requires you to unlearn a lot of what you've come to expect about how programs execute.
This talk will include an interactive visualization of algebraic effects, as well as a detailed comparison to other competing models, such as Elm's and Roc's Task's, Haskell's IO monad, React's Hooks, and Javascript's promise-based async/await.

## Brief intro/background on algebraic effects
- Available in eff-lang, unison, koka, idris (fork), haskell (fork), ocaml
- Part of the inspiration for React’s Hooks

## Async in JavaScript - a progression
- fs.readFileSync to callback-passing to promises to async/await
- all of these have the unfortunate “what color is your function” property, requiring you to update syntax & control flow when adding async.
- Algebraic effects lift the information about whether a function is async to the type level, meaning that no change in invocation syntax is required!

## Regulating side-effects
- From a security & testability standpoint, allowing any code (especially untrusted library code) to execute arbitrary side-effects is undesirable.
- deno & WASI allow you to limit access to system resources (file system, network) when running a command as a whole, which is a nice step forward
- Haskell & Elm require you to pass around effects as explicit values (Tasks or IO monad), which is nice for security & testability, but has the “what color is your function” problem again
- Algebraic effects give us the best of both worlds -- fine-grained control over what side-effects can happen in what code paths, while allowing code to be written in a direct, "imperative" style, whether or not it includes side-effects. Even better, you can seamlessly spoof or mock out requested effects enabling testing and k

## Demo of algebraic effects
- developing intuition about what happens at the call-stack execution level w/ visualizations and a stepping debugger.









helping you gain an intuitive grasp of the execution model, and better understand the capacity they have to revolutionize computing.

requires you to unlearn
Unfortunately, really understanding how algebraic effects work requires diving into the call stack

JavaScript has had several different methods for managing asynchronous execution, starting with event listeners and callback functions, and then promises, and most recently async/await.

Abstract
Modern JavaScript has seen a great deal of development as far as asynchrony is concerned; starting with event listeners and callback functions, and then promises, and most recently async/await. On the other hand, we're just starting to deal with the problem of regulating  side-effects in untrusted code, with deno's permissions flags and wasi's sandboxing model blazing the trail.
Come learn how algebraic effects provide an elegant solution to both of these two areas, and see them demonstrated in a JavaScript-like language.

Shorter version of what will the attendee learn
## Brief intro/background on algebraic effects
- Available in eff-lang, unison, koka, idris, haskell, ocaml
- Part of the inspiration for React’s Hooks

## Async in JavaScript - a progression
- fs.readFileSync to callback-passing to promises to async/await
- all of these have the unfortunate “what color is your function” property, requiring you to update syntax & control flow when adding async.
- Algebraic effects lift the information about whether a function is async to the type level, meaning that no change in invocation syntax is required!

## Regulating side-effects
- From a security & testability standpoint, allowing any code (especially untrusted library code) to execute arbitrary side-effects is undesirable.
- deno & WASI allow you to limit access to system resources (file system, network) when running a command as a whole, which is a nice step forward
- Haskell & Elm require you to pass around effects as explicit values (Tasks or IO monad), which is nice for security & testability, but has the “what color is your function” problem again
- Algebraic effects give us the best of both worlds -- fine-grained control over what side-effects can happen in what code paths, while allowing code to be written in a direct, "imperative" style, whether or not it includes side-effects.

## Demo of algebraic effects
- developing intuition about what happens at the call-stack execution level w/ visualizations and a stepping debugger.




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

# The Local-first Landscape

Don't you wish the software you used worked offline, enabled seamless collaboration with your colleques and friends, and gave you full control over your data? That's the dream of local-first software! I'll give you an overview of what's happening in the world of local-first web development, including some basic definitions, what use-cases it's well-suited to, and a comparison of the current offerings in terms of data storage, authentication, and network layers.



-- Rough Outline ---

# Overview

The term "local-first" was coined by the folks at Ink & Switch, in an article where they outline [seven ideals for local-first software](https://www.inkandswitch.com/local-first/):
1. No spinners: your work at your fingertips
2. Your work is not trapped on one device
3. The network is optional
4. Seamless collaboration with your colleagues
5. The Long Now
6. Security and privacy by default
7. You retain ultimate ownership and control

# Good (& Bad) use cases for local-first software

If you have:
- a small number (<1000) of users working on any given thing
- "small" data (<100mb in a given "workspace")
- soft internal access control (anyone who can read some data is also allowed to write it)
  - this can be somewhat mitigated by client-side enforcement of access control, but it's not robust to malicious/customized clients
- eventual consistency is desirable, and you don't need to maintain whole-system invariants

so, things like:
- any app that's going to be used by just one person, but on multiple devices, is an excellent candidate
  - examples: personal to-do list manager, daily planner, journal, workout/fitness app, recipe manager, "read it later"/pinboard-style bookmarks manager
- a collaborative text editor or whiteboard where all users are "trusted" -- e.g. no need to revoke access, no view-only access

things it's not good for
- social media (lots of users, lots of read-only data)
- a multiplayer game (access control)
- payment system (can't enforce invariants e.g. total amount of money stays constant)

# Different kinds of use-cases

- people worried about privacy & security
- people who want full control over their data (e.g. if a website shuts down, I have a full backup)
- people who want independence from central infrastructure (p2p mesh networking)
- etc.

# Components of a local-first web stack

- Data storage -- probably CRDT-backed, it's the industry solution right now
- Transport layer - Websockets, or Polling, or maybe p2p direct connections
- Network topography - central server, or no? for some people not having one is a critical feature of local-first apps, but others need the system-level guarantees and access-control afforted by a central source of trurth
- Authentication
  - for very simple auth stories, you can have a "secret database ID" -- e.g. if you know the ID/URI of the database, you are allowed read & write access
  - you can have a blend of public & private data by segmenting into separate datastores; a "public" database that all users know the ID of, and for each user a "private" database that only they know about
  - You can do some public key encryption stuff to "sign" individual pieces of data in the database, to verify that a given user originated it, but there's nothing to stop another user from overwriting that data with their own, or with their own signature, claiming that they authored it
  - if you have a centralized network topography, you can have much more fine grained access control, such as refusing to integrate updates from clients if they are editing data they shouldn't. This will probably require some custom code though.





----- done with rough outline ------




Node
[ Data ]

 ^
 |
 | (lock icon = auth)
 |
 v

Node
[ Data ]


## Authentication

The very simplest auth mode is "no access limitations".
One step above that is "document-based access", where each document
(backed by a separate database) is identified by a GUID, and knowledge
of the GUID is equared with permission to access the given document.
And access, in this case, means ability to ready anything, write
anything, and of course share access with anyone else, just by giving
them the GUID to the document.
There's a way to have per-user "private" data, and that is to give each
user their own private document, with a UUID that they don't share.
Having a single logical "document" split into several databases
(one for "public" stuff, and then several others with private user
data) means that maintaining document-wide invariants becomes essentially
impossible; but it turns out that we already lost that ability when we
embraced distributed, eventual consistency with CRDTs.
CRDTs cannot, in general, maintain high-level invariants; it's hard enough
to engineer them to maintain local invariants.


Do you care about
- revoking access
- untrusted users
- read-only data
- schema migration

The simplest method of authentication is by secret ID; the "document"
in question is identified by a GUID, and if you know the ID you're
allowed full read & write access.
This means there are no system-enforced users; you can identify
yourself for a session, but you could just as easily pretend to be
someone else; there are no uniqueness guarentees.

You could do private key encryption
or signing
add a user "profile", signed with a key,
and have the key encrypted with a password a couple different ways
(once with a password, and again with some nmeumonic passwords)

BUT that would do for authentication ("I wrote this") but not for
privacy, in general.
You could do "last write wins" fields with encrypted contents,
but you couldn't, say, have a whole row that's encrypted; the
CRDT wouldn't be able to resolve conflicts if the contents
are indecipherable.

If you want truly private data, best to just have a separate,
private database.
BUT this means you can't really "share a subset" of some
database with someone. It's all or nothing.

IFF you have a central network topology,
you can enforce some ACLs on synchronization
and even maintain some more invariants.


## Data Storage

Simple last-write-wins
Collaborative text?
History recovery? (unlimited undo)
What kind of invariants can it enforce?

## Transport Layer

HTTP(S) Polling
Websockets
P2P Mesh Networking









# Let's build a local-first app

The local-first web is


We'll be building a small recipe manager; intended for use
by a handful of people, to keep track of recipes they like.

> Create a new user, or login?
  - that's not the right password. If you have a recovery phrase,
    you can enter one now.
> Overwrite a user?
  - nothing's stopping a user from overwriting another user's
    attribution
  - well, nothing except a harrowing sense of guilt

OH so when you log in, you decrypt the ID of your personal
datastore too. That's cool I think.

We could also have a server that stores keys in a retrievable way,
if we wan't "password reset" functionality.

"gentleman's agreement" security model.
Only makes sense where:
- number of users is small
- data, attribution, etc. aren't sensitive




# Recent developments

There's a local-first-web #LFW discord server with monthly meetups


So you want to make a local-first app; what are the resources available to you?

There are a number of different reasons you might be interested in "local-first" web.

Some people index high on the "low connectivity" use-case.
Some are much more concerned with data ownership and security.
Some are very interested in infrastructure-independence (Strong P2P)



## We might need to know about service workers



# The State of Local First Development




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

# Semi-structured Editing
# Why the heck are we still editing text in files
# A Brief History of Structured Editing

Have you ever wondered why we're still using plain, error-prone text files for storing and editing code in the year 2023? Or pulled your hair out over a merge conflict that the computer really *ought* to be smart enough to resolve by itself? How are programmers the only ones stuck with plain text files, when everyone else gets much richer representations?
Well, people have been trying to come up with more structured alternatives for over 40 years, and yet very few have managed to gain any widespread use. We'll look at several such attempts, common pitfalls they fell into, and what the future of structured editing might look like.






%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%










# Unison

Understanding Algebraic Effects in Unison






%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%





# Roc

Pure Functional Server-side Apps with Roc

