---
title: "Papers We Love: Flow Typing, A New Lens on Linearity"
tags:
- compiler
- programming languages
- pl research
date: 2026-10-20
gist: 01a0ce48637b88c9a0b2c9f78bc8fdb0
draft: false
---

In my various efforts to develop new programming languages, I've occasionally looked sidelong at linear types, as something that looks like it could be really useful if it wasn't so incredibly cumbersome.

And now some folks have come up with [a way to make them much easier to use](https://people.cs.nott.ac.uk/pszgmh/flow-typing.pdf)[^1]! Or at least that's the hope. This past week I've been trying to implement their execution model into [a new programming language](https://github.com/jaredly/laminar), to get a feel for what it's like to actually use. Check out my [field notes](/posts/lens-on-linearity-field-notes/), if you're already familiar with the paper.

<!-- more -->

## Flow Typing: A New Lens on Linearity

I'm going to quote the abstract in full, as it does a great job of introducing the paper:

> Linear type systems often suffer from a threading problem: if a value is used more than once, then it must be explicitly threaded through each usage. Existing solutions introduce extra language features to mitigate this, but at the cost of complicating the core language and its semantics. This article introduces flow typing, a new solution to the threading problem that avoids the need to extend the core language. Flow typing is based on the theory of lenses, a functional presentation of mutable references, in which lens terms mutate parts of a stateful typing context in a controlled way. Because this approach is purely functional, it can be implemented in a standard linear lambda calculus, providing flow typing with a simple and direct semantics.

Ok I'll admit that there are a number of terms in there that likely aren't totally familiar to the average programmer, and I also want to convince myself that I know what they mean, so let's break things down a bit.

### Linear type systems

A type system is ["linear"](https://en.wikipedia.org/wiki/Substructural_type_system#Linear_type_systems) if _all values must be used exactly once_. You might be thinking (a) that sounds incredibly restrictive, I quite like using values multiple times, and (b) why would you even want that. I'll speak to the second one first.

Some reasons a linear type system might be attractive:

#### Mandatory explicit disposal

Have you ever forgotten to close a file, and had a dangling file descriptor? Have you ever forgotten to free a pointer? To put it in web dev terms, what about failing to close a database connection, or to unsubscribe a listener?

If a value (say a `DBConnection`) _must be used exactly once_, and the library offers you (a) functions that take the connection as an argument and then give it back to you in the return value, and (b) a `close()` function as the only way to "get rid of it", then you suddently have a statically enforcable mechanism for ensuring you never forget to close a db connection.

To make this concrete:

```ts
let run = () => {
    let db = sqlite.open('./my.db');
    let [db2, records] = sqlite.run_query(db, RECORD_QUERY);
    save(records);
    sqlite.close(db2);
};
```

If the final `sqlite.close(db2)` line were missing (in a fictional javascript with a linear type system), you'd get a type error `db2 must be used exactly once, but was used 0 times`. (the `save` function is a stand-in for "doing something with the records we got from the db")

In such a system, you're also statically disallowed from ignoring (or forgetting about) a return value.

#### Purity by obscurity

Pure functional languages tend to achieve purity via immutability, to the extent that you'd be forgiven for thinking that it was a necessary component of having a pure language. "Pure" here meaning that "if you give a function the same input, it is always guaranteed to return the same output"[^3]. A simple example of an impure function in javascript would be:

```js
let lie = false;
function addTwo(input) {
    if (lie) return input + 3;
    return input + 2;
}
console.log(addTwo(2)); // 4
lie = true;
console.log(addTwo(2)); // 5
```

This is a rather tame example of [action at a distance](https://en.wikipedia.org/wiki/Action_at_a_distance_%28computer_programming%29), an ill that pure functional programming saves you from.

In Haskell or Elm, the above code is impossible to represent because you can't "mutate the value that `lie` represents" after initializing the variable. All variables are immutable, giving you purity by construction.

In a linear type system, by contrast, purity is achieved by _preventing any variable from having multiple observers_. Essentially, "if mutation happens but no-one else can observe it, does it make a sound?"[^4] The above code would be disallowed because line 7 is a "second reference to the variable `lie`", and therefore a type error.

##### Rust's linear (ok affine) mutable variables

Rust is famous for its linear-inspired setup for mutable variables; if a variable is marked as mutable, there can only be one "accessor" (reader or writer) at a time. This allows them to have some pretty strong guarantees about safety, especially in the context of concurrency. However, they also want to be able to provide access-by-reference for mutable variables (as it would be terribly inconvenient to have to pass-by-value anything that's mutable), which is accomplished via the infamous "borrow checker". The linear-style requirements are represented by a system of "borrowing", where the compiler needs to track which variables are "live" at any given point in the code, and ensure there's only one live reference to a given value at a time. This is famously confusing and hard for people to learn. In spite of this, Rust is a wildly successful language.

### The Threading problem

Let's accept for the moment that a linear type system is desirable (or at least interesting), and circle back to the "terribly inconvenient to have to pass-by-value anything that's mutable" issue. This is what Sky & Wilshaw call the "threading problem". I'll demonstrate with some simple typescript code:

```ts
let person = new Person();
setName(person, 'Lucretia');
setAge(person, 51);
setSummary(person, getSummary(person));
display(person);
```

We create an object, we mutate it with a couple of functions, and we display it.

In typescript, we're passing by reference, which is how `setName` etc. are able to affect our object. Passing by reference isn't allowed in a linear type system (only one accessor is allowed per variable); so here's what our code would look like with pass-by-value[^2]:

```ts
let person = new Person();
let person2 = setName(person, 'Lucretia');
let person3 = setAge(person2, 51);
let person4 = setSummary(person3, getSummary(person3));
display(person4);
```

And `setName` would have to change, from e.g. `(person, name) => {person.name = name}` to `(person, name) => ({...person, name})`.

This is almost linear-valid, but on line 4 we're using `person3` twice, which is illegal. We'd have to change it to this:

```ts
let [person4, summary] = getSummary(person3);
let person5 = setSummary(person4, summary);
display(person5);
```

Extremely cumbersome: we're having to "thread" the `person` object into and out of every function that touches it.

### Lenses

My understanding of lenses is that they are a way to operate on (often in a functional/immutable way) a potentially deeply-nested subset of a given piece of data. Lodash's `set` and `update` are related to this, and [ramda](https://ramdajs.com/docs/#lens) has explicit support for lenses.

## Bringing it all together

The innovation of this paper is using some clever syntax sugar to give you the convenience of mutable references (potentially many layers deep into an object, that's the lens part) while retaining the simplicity and purity of a traditional linear type system. This essentially gives you something akin to a borrow-checker, but with dramatically simpler semantics and implementation, enabling things like a formal proof that it works ;).

The big question for me, though, is _what does this feel like to use?_ How hard is it to actually implement in a language, and is the resulting code easy to write, read, and understand?

And so I've started [writing a language](https://github.com/jaredly/laminar) that attempts to implement their system of "flow typing". [Here are my field notes](/posts/lens-on-linearity-field-notes/).


[^1]: I got turned onto this paper by [this tweet](https://x.com/lexi_lambda/status/2109683868869480451) from [Alexis King](https://x.com/lexi_lambda) asking for people's favorite PL papers this year.

[^2]: In truth, typescript is still doing pass-by-reference, but we're treating the values /as if/ they were pass-by-value, by doing immutable updates.

[^3]: Paraphrased [from wikipedia](https://en.wikipedia.org/wiki/Purely_functional_programming). This is of course only true within certain assumptions about the reliability of the underlying system; other actors on the same machine could corrupt your heap, or a cosmic ray could flip a bit, etc.

[^4]: Clojure takes this approach with their transients, though of course there are not any kind of static guarantees that they are correctly used.
