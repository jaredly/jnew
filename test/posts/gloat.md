---
title: Gloat - a Self-Hosted Typechecker for Gleam
tags:
  - programming languages
categories:
  - Programming
date: 2026-01-30
draft: true
---

I was watching [this interview with Gleam Creator Louis Pilfold and Richard Feldman](https://www.youtube.com/watch?v=WC_j91qogyY) and they mentioned how few resources there are for learning how to create a type checker, and I took that as a challenge.

I had previously done a [lot of work with type inference algorithms](https://compiler.jaredforsyth.com/), but only in languages I've created myself :) but given Gleam's dedication to simplicity, I wanted to see how hard it would be to port my algorithm to work on Gleam.

Long story short, gpt-5.2-codex was able to take care of it without too much manual intervention!

- pubgrub dependency resolution
-
