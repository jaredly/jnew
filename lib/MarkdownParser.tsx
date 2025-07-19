import md from "markdown-it";

export let saveTwitterCache = () => {
};

import "prismjs"
import mdi from "markdown-it-anchor";
import mdf from "markdown-it-footnote";
import mdp from "markdown-it-prism";
import "prismjs/components/prism-javascript";
import "prismjs/components/prism-clojure";
import "prismjs/components/prism-reason";
import "prismjs/components/prism-ocaml";
import "prismjs/components/prism-rust";
import "prismjs/components/prism-bash";
import "prismjs/components/prism-json";
import "prismjs/components/prism-graphql";
import "prismjs/components/prism-diff";
import "prismjs/plugins/diff-highlight/prism-diff-highlight.js";
import { existsSync, readFileSync } from "fs";

const mdpoetry = md("default", { breaks: true, html: true });
export const processPoetry = (rawBody: string) => mdpoetry.render(rawBody.replace(/\n--+\n/g, "\n<br/><br/><br/><br/><br/>\n\n----\n"));

function markdownItJsEmbed(md) {
  const fence = md.renderer.rules.fence || (() => '');

  md.core.ruler.after('block', 'js-embed-eval', state => {
    const tokens = state.tokens;

    for (let i = 0; i < tokens.length; i++) {
      const token = tokens[i];

      if (
        token.type === 'fence' &&
        token.info.trim().toLowerCase() === 'js embed'
      ) {
        let html = '';
        try {
            const f = new Function('{readFile, mdToHtml}', token.content); // ⚠️ dangerous!
            const result = f({readFile: (name: string) => {
                const path = __dirname + '/test/posts/' + name
                if (existsSync(path)) {
                    return readFileSync(path, 'utf8')
                } else {
                    console.log('cant fine', path)
                    return `File not found: ${name}`
                }
            }, mdToHtml: (text) => {
                return process(text)
            }})

          html = typeof result === 'string' ? result : String(result);
        } catch (err) {
          html = `<pre><code>Error: ${md.utils.escapeHtml(err.message)}</code></pre>`;
        }

        // Replace this token with an HTML block token
        const htmlToken = new state.Token('html_block', '', 0);
        htmlToken.content = html;

        tokens.splice(i, 1, htmlToken);
      }
    }
  });
}

// function markdownItJsEmbed(md) {
//   const defaultFence = md.renderer.rules.fence || function(tokens, idx, options, env, slf) {
//     return slf.renderToken(tokens, idx, options);
//   };

//   md.renderer.rules.fence = (tokens, idx, options, env, slf) => {
//     const token = tokens[idx];

//     console.log('EVAL it', token.info.trim())
//     if (token.info.trim() === 'js embed') {
//         console.log("YES EVAL")
//       let result;
//       try {
//         const f = new Function('{readFile, mdToHtml}', token.content); // ⚠️ dangerous!
//         result = f({readFile: (name: string) => {
//             const path = __dirname + '/../test/posts/' + name
//             if (existsSync(path)) {
//                 return readFileSync(path, 'utf8')
//             } else {
//                 return `File not found: ${name}`
//             }
//         }, mdToHtml: (text) => {
//             return process(text)
//         }})
//         if (typeof result !== 'string') {
//           result = JSON.stringify(result, null, 2);
//         }
//       } catch (err) {
//         result = `Error: ${err.message}`;
//       }

//       return `<pre><code>${md.utils.escapeHtml(result)}</code></pre>\n`;
//     }

//     // fallback to default rendering
//     return defaultFence(tokens, idx, options, env, slf);
//   };
// }

const mdit = md("default", {
    linkify: true,
    typographer: true,
    html: true,
})
    .use(mdf)
    .use(mdi, {
        permalink: mdi.permalink.headerLink(),
        slugify: (text) => {
            return text
                .trim()
                .replaceAll('"', "&quot;")
                .replaceAll(/[^a-zA-Z0-9_-]+/g, "-")
                .replace(/-$/, "");
        },
    })
    .use(mdp, {
        highlightInlineCode: true,
        defaultLanguage: "javascript",
    })
    .use(markdownItJsEmbed)

export const process = (rawBody: string, ids?: { [key: string]: string }) => {
    return mdit.render(rawBody);
};
