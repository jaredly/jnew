---
title: Comparing AI Coding Agents
tags:
- ai
categories:
- Programming
date: 2025-07-18
draft: true
---

I wanted to make a web site, but I didn't want to do the work. It's not a very interesting website, technically speaking, so it would be tedious to implement. Also, corporate wants me to jump into the Beautiful World of Tomorrow, and all that entails.

I have free access to copilot because of my open source work, so I started into it with GPT 4.1, because it's free, but I got to a point where I wanted a second opinion (because it was obviously missing some things), and I figured I'd do a little comparison of the models available to me.

<!-- more -->

## The Scenario

I worked with GPT4.1 to produce a somewhat detailed [`Implementation Plan.spec`](https://github.com/jaredly/artswap/blob/8f1e2d7a450309f4c270c112431fe1e473722d95/Implementation%20Plan.spec), and then a [Prisma schema](https://github.com/jaredly/artswap/blob/8f1e2d7a450309f4c270c112431fe1e473722d95/prisma/schema.prisma) defining the data model for the website.

I did a look-over of the `schema.prisma` file, and it was clear that at least one important thing was missing (the ability to mark a user as a "super admin"), so I switched over to Claude 4 Sonnet and asked:

> Can you look over the "Implenetation Plan" document, and see if we're missing anything from this prisma schema?

And it not only found & fixed the issue I had identified (the super-admin bit), but also found several other deficiencies in the data model that could be inferred from the implementation plan, but weren't necessarily obivous.

1. a core part of the website is letting artists "like" or "dislike" other artist's submissions, but there wasn't a model to represent a `Vote` entity
2. the `Event` entity had no `title` or `description` fields
3. there wasn't an entity for tracking user sessions
4. the `Event` entity has a `phase` attribute that is currently a string, with a comment indicating the phases. Changing it to a prisma `enum` is more robust

## Ways in which this scenario is "hard"

- The implementation spec has a section where it lists models that are needed, but is missing `Vote` and `Session` entities, which are implied by the rest of the spec
- My prompt is a little grammatically embiguous, and misspelled the name of the spec file
- the prisma schema has an unusual 1-to-2 relation, which confused some of the models

## The Criteria

After Claude gave me that stellar response, I thought this would be a good opportunity to evaluate the various models that Copilot gives me easy access to, along with some local models through Ollama.

Here are the criteria:

- **Accuracy:** does it say only things that are accurate? (when describing the implementation plan, or things that are missing from the spec)
  - relatedly, does it make changes that are wrong or invalid?
  - does it understand the prompt, or do I have to reword things?
- **Verbosity:** how much text does it expect me to read? (lower is better)
- **Babysitting:** do I have to prompt it to actually make the changes suggested, or does it take the initiative?

I'll also track how many correct changes it was able to make to the schema:

- Adding an entity to track `Vote`s
- Making it so an Artist can be marked as a `Super Admin` (either via a flag or a `role` enum)
- Adding an entity to track user `Session`s
- Converting the `Event`'s `phase` attribute to an `enum`

## The Results

<style>
  table td {
      border-block: 1px solid black;
      border-block: 1px solid #7e6eb2;
      padding: 0 5px;
      text-align: center;
  }
  table td:first-of-type, table th:first-of-type {
      text-align: left;
      font-size: 80%;
      white-space: pre;
  }
  table th {
      /* background-color: #373d4a;  */
      border-bottom: 2px solid black;
      border-bottom: 2px solid #7e6eb2;
      font-size: 70%;
      padding: 0 5px;
      /* max-width: 80px; */
      /* word-break: break-all; */
      white-space: pre;
  }
  table {
      border-collapse: collapse;
  }
</style>

### Github Copilot Remote Models

| Model | Accuracy | Verbosity | Babysitting | Vote | SuperAdmin | Session | Enums |
| ----- | -- | -- | -- | - | - | - | - |
| Claude 4       | 🟢  | 🟢 | 🟢 | ✅ | ✅ | ✅ | ✅ |
| GPT 4.1        | 🟡 | 🔴 | 🟡 | ✅ | ✅ | ❌ | ✅ |
| GPT 4o         | 🟢  | 🔴 | 🟡 | ✅ | ❌ | ✅ | ✅ |
| Claude 3.5     | 🟢  | 🟡 | 🟢 | ❌ | ❌ | ❌ | ✅ |
| Claude 3.7     | 🟢  | 🟡 | 🟢 | ✅ | ✅ | ✅ | ✅ |
| o4 mini        | 🟢  | 🟡 | 🟡 | ✅ | ❌ | ✅ | ✅ |
| Gemini 2.5 Pro | 🟢 | 🟢 | 🟢 | ✅ | ❌ | ❌ | ✅ |

<br/>

### Copilot local models, served by Ollama

| Model | Accuracy | Verbosity | Babysitting | Vote | SuperAdmin | Session | Enums |
| ----- | -- | -- | -- | - | - | - | - |
| Mistral        | 🔴 | 🟢 | 🟡 | ❌ | ❌ | ❌ | ❌ |
| Codellama 13b  | 🔴 | 🟡 | 🟡 | ❌ | ❌ | ❌ | ❌ |
| Codestral      | 🔴 | 🟡 | 🟡 | 😬 | ❌ | ❌ | ❌ |
| Starcoder      | 🔴 | 🟡 | 🟡 | ✅ | ❌ | ❌ | 😬 |
| Llama3         | 🔴 | 🟡 | 🟡 | ❌ | ❌ | ❌ | ❌ |
| Qwen3          | 🟢 | 🔴 | 🔴 | ❌ | ❌ | ❌ | ❌ |
| Deepseek r1 14b | 🔴 | 🔴 | 🟡 | ❌ | ❌ | ✅ | ❌ |

<br/>

### Third-party agent extensions

| Extension / Model | Accuracy | Verbosity | Babysitting | Vote | SuperAdmin | Session | Enums |
| ----- | -- | -- | -- | - | - | - | - |
| Cline / Qwen3 | 🟢 | 🟢 | 🟢 | ❌ | ❌ | ❌ | ❌ |
| Cline / Deepseek | 🟢 | 🟢 | 🟢 | ❌ | ✅ | ✅ | ❌ |
| Continue / Llama3 | 🟡 | 🟡  | 🟢 | ❌ | ❌ | ❌  | ❌ |

<br/>

### Chat Transcripts & Changes

<style>
.tabs > div.selected {
  background-color: #ddd;
}
.tab-body > div {
  display: none;
}
.tab-body > div.selected {
  display: block;
}

.tab-columns {
  display: flex;
  flex-direction: row;

  div {
    min-width: 0;
    flex: 1;
    overflow: auto;
  max-height: 600px;
  }
}

.chat-transcript {
  border: 1px solid green;
  padding: 6px 12px;
  font-size: 50%;
  line-height: 1.5;

  h2 {
    font-size: 130%;
    padding-bottom: 8px;
  }
  h3 {
    font-size: 120%;
    padding-bottom: 4px;
  }
  h4 {
    font-size: 110%;
  }
}

.chat-diff {
  font-size: 50%;

}

</style>

```js embed
const runners = {
  'copilot': ['Claude 4', 'GPT 4.1', 'GPT 4o', 'Claude 3.5', 'Claude 3.7', 'o4 mini', 'Gemini 2.5 Pro'],
  'copilot-local': ['Mistral', 'Codellama 13b', 'Codestral', 'Starcoder', 'Llama3', 'Qwen3', 'Deepseek r1 14b'],
  'cline': ['Qwen3', 'Deepseek r1 14b'],
  'continue': ['Llama3'],
}

const tabs = []

for (let runner of Object.keys(runners)) {
  for (let model of runners[runner]) {
    const fileName = `${runner} - ${model}`
    const chat = readFile(`compare-agents/${fileName}.md`)
    const diff = readFile(`compare-agents/${fileName}.diff`)
    tabs.push({name: fileName, body: `<div class="tab-columns">
<div class="chat-transcript" >${mdToHtml(chat)}</div>
<div class="chat-diff">${mdToHtml(`\`\`\`diff\n${diff}\n\`\`\``).replace(
  '<pre class="language-diff">',
  '<pre class="language-diff diff-highlight">',
)}</div>
</div>`})
  }
}

return `
<script>
  window.showTab = function(sel) {
    const name = sel.value;
    sel.parentElement.querySelectorAll('div.selected').forEach(t => t.classList.remove('selected'));
    sel.parentElement.querySelector(\`div[data-name="\${name}"]\`).classList.add('selected');
  }
</script>
<div>
<select onchange="showTab(this)">
${tabs.map(({name}, i) => (
  `<option value="${name}" ${i === 0 ? 'selected' : ''}>${name}</option>`
)).join('\n')}
</select>
<div class="tab-body">
${tabs.map(({name, body}, i) => (
  `<div data-name="${name}" ${i === 0 ? 'class="selected"' : ''}>${body}</div>`
)).join('\n')}
</div>
`

```

## Conclusions

Claude 4 is the current darling for a reason: it's really good! I was surprised to see that Claude 3.7 was essentially just as competent (just a little wordier). Behind them I would put Gemini 2.5 Pro, GPT 4o, and o4 mini; all three were helpful, but not nearly up to the standard of Claude.

GPT 4.1 and Claude 3.5 are in a tier below that, where they can do a lot for you, but will likely need much more specific instructions, and can't be expected to make rock-solid higher-level decisions about a project.

Unsurprisingly, the local models that will run on my M1 Macbook Pro are much less capable (and, with the exception of Qwen3, soooo much slower); though I was impressed with Qwen3 (again, probably needs more specific instructions to be helpful) and Deepseek, especially when it was prompted by Cline.

## Detailed notes

### Claude 4 Sonnet

Excellent work. After making the initial edit, it noticed that there were linter errors, and proceeded to fix those too.

### GPT 4.1

- I had to discard a session because it totally failed due to a syntax error that it introduced in the schema (it ended up deleting several entities from the file).
- It did well with the `Vote` entity, but when I asked it to add support for a "super admin" role, there were several issues: first, it tried to add the field to the `Artwork` entity, not the `Artist` entity, and when I corrected it, it removed the errand field, again failed to add it to the `Artist` entity, but told me that it had completed the task (when it had not)

### Gemini 2.5 Pro

Added a "Artwork Flag" model, for tragging the reason a piece was flagged. Could be useful.

### Mistral (Ollama)

Didn't actually make the change using the tool; it output the "new file version" into the chat, and I had to press a button to "adopt those changes into the current file". Perhaps needs more prompting?

While it didn't make any of the changes I was hoping for, it did manage to make /valid/ changes (adding some arguably-useful properties to 4 different entities).

### Codellama:13b (Ollama)

Had to be used in "Edit" mode, not "Agent" mode, which doubtless impacted the experience.

When it did make suggestions they were bad, and contained errors.

### Codestral (Ollama)

- totally failed to understand that the `Artist` entity was being used for user management.
- even though it recommended adding a `User` entity, when asked to implement suggestions, it skipped it
- it recognized that there was need for some "voting" support, but misunderstood the nature of the voting

### Starcoder (Ollama)

- tried to do an Event Phase enum, and made the change in the Event entity, but failed to define the actual enum
- also produced some errors (the 1-to-2 relation of Match to Artwork, it bungled it)

### Llama3 (Ollama)

- made straight up false statements about the current schema

### Qwen3 (Ollama)

- Sooo verbose
- No babysitting needed! Impressive
- the changes it suggested were useful (email verification token tracking), but it missed the things I wanted

### Cline Qwen3

- seems to have been overwhelmed by Cline's large prompt. Didn't come up with any useful changes

### Cline Deepseek R1 (14b)

- actually did a pretty decent job!

### Deepseek R1 (Copilot)

- misunderstood the prompt; had to be reprompted with clearer wording
- didn't realize that the `Artist` entity was for users, wanted to create a new `User` entity
- produced invalid prisma code (hallucinated pragmas `@oneToManyInverse` and `manyToManyInverse` instead of the actual `@relation` pragma)
- deleted the `GroupMembership` entity entirely, and removed many attributes from other entities

### Continue (llama3)

- understood the prompt, but whiffed it on making suggestions.
- claimed to add a line (the `match` attribute of the `Event` entity), when that line already existed. all it did was add a comment claiming credit
