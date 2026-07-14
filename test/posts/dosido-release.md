---
title: Automate typescript refactors with dosido
tags:
- javascript
- typescript
date: 2026-07-13
draft: false
gist: 67a67b9978aaf1352f73dc3a5d01cbe6
---

I don't know about you, but my personal projects often metastasize. I start going in one direction, and after a while I've learned a lot about what I really want so I start a fresh implementation in a new subfolder, and along the way I'm building tools and libraries as I discover I need them. After a while it's a tangled mess, where the production app uses maybe 30% of the code, and there are at least three reusable libraries waiting to be born, if only I could get my act together to do the cleanup.

So now I've finally gathered the will to do some refactoring, but why do the work by hand when [I can automate the process](https://xkcd.com/1319/)?

I was surprised to discover that there wasn't a ready-made cli tool for 'move this typescript file from here to here, fixing impacted imports'. So I built that (`dosido move`), as well as 'move this definition to a new/another file' (`dosido extract`).

So far these are things that can be done by drag & drop in vscode; but vscode UI interactions can't form the building blocks of other, more complex tools.

`dosido decouple <folder-a> <folder-b> <dest>` takes any definitions that are used by both `folder-a` and `folder-b` (forming dependency cycles between the two folders) and moves them to `dest`, resolving the cycles.

`dosido barrel <folder>` makes a barrel file for a given folder, rewriting all external imports into the folder to point at the directory path, to prepare the folder for being extracted as its own package (yes I know 'barrel files considered harmful' but you do actually want them at the toplevel of a library).

And then `dosido workspace extract <folder>` can take that folder and turn it into a separate package in the monorepo, rewriting imports as necessary.

There are a few `dosido` diagnostic commands that are especially helpful for planning a large scale refactor, and determining what files or definitions need to be moved around in order to enable a clean reorganization. They are also quite useful to Agents.

Available [wherever javascript packages are sold](https://www.npmjs.com/package/dosido), and [on GitHub](https://github.com/jaredly/dosido).

```bash
npm install dosido
```
