import { readFileSync, writeFileSync } from 'fs';
import { basename } from 'path';
import { render } from '../lib/Home';
import { saveTwitterCache } from '../lib/MarkdownParser';
import { processBlog, processProjects, processTalks } from '../lib/Yak';
import fetch from 'node-fetch';
import { chopSuffix } from '../lib/Util';

let run = (renderDrafts: boolean) => {
    let outputDir = './test/pages/';
    let inputDir = './test';

    let projects = processProjects(inputDir, outputDir);
    let posts = processBlog(!renderDrafts, inputDir, outputDir);
    let talks = processTalks(inputDir, outputDir);

    if (process.env.UPLOAD_GISTS) {
        const token = readFileSync(
            `${process.env.HOME}/.config/gh/hosts.yml`,
            'utf-8',
        )
            .split('\n')
            .filter((t) => t.trim().startsWith('oauth_token:'))[0]
            .split('oauth_token:')[1]
            .trim();

        posts
            .filter((post) => post.config.gist)
            .forEach(async (post) => {
                if (post.body.type !== 'Markdown') {
                    throw new Error(
                        `gist post not markdown? ${post.config.fileName}`,
                    );
                }
                const current = (await fetch(
                    `https://api.github.com/gists/${post.config.gist}`,
                    {
                        headers: {
                            Accept: 'application/vnd.github+json',
                        },
                    },
                ).then((res) => res.json())) as {
                    files: { [name: string]: { content: string } };
                };
                console.log(post.config.fileName, post.body.body.length);
                const fileName = basename(post.config.fileName);
                if (!current.files[fileName]) {
                    throw new Error(`no file ${fileName}`);
                }
                const withHeader =
                    `# ${post.config.title}\n\n${
                        post.config.draft
                            ? ''
                            : `[View rendered & in context at jaredforsyth.com](https://jaredforsyth.com/posts/${chopSuffix(
                                  basename(post.config.fileName),
                              )})\n\n`
                    }` + post.body.body;
                if (current.files[fileName].content === withHeader) {
                    return console.log('- already up to date', fileName);
                }
                fetch(`https://api.github.com/gists/${post.config.gist!}`, {
                    method: 'PATCH',
                    headers: {
                        Accept: 'application/vnd.github+json',
                        Authorization: `Bearer ${token}`,
                        'X-GitHub-Api-Version': '2022-11-28',
                    },
                    body: JSON.stringify({
                        files: {
                            [fileName]: {
                                content: withHeader,
                            },
                        },
                    }),
                }).then((res) => {
                    console.log('Uploaded', post.config.fileName, res.status);
                });
            });
    }

    // setupRedirectsForOldPosts(outputDir, posts);

    /* Home page */
    writeFileSync(
        outputDir + '/index.html',
        render(projects, posts, talks),
        'utf8',
    );

    saveTwitterCache();
    console.log('Finished!');
};

run(!!process.env.DRAFTS);
