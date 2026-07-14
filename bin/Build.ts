import { readFileSync, writeFileSync } from "fs";
import { basename } from "path";
import { render } from "../lib/Home";
import { saveTwitterCache } from "../lib/MarkdownParser";
import {
    processBlog,
    processPoetry,
    processProjects,
    processTalks,
} from "../lib/Yak";
import fetch from "node-fetch";
import { chopSuffix } from "../lib/Util";
import { execSync } from "child_process";

const getToken = () => {
    try {
        const result = execSync(
            `security find-generic-password -w -s 'gh:github.com'`,
            { encoding: "utf-8" },
        ).trim();
        if (result.startsWith("go-keyring-base64:")) {
            return Buffer.from(
                result.slice("go-keyring-base64:".length),
                "base64",
            ).toString("utf-8");
        }
        return result.trim();
    } catch (err) {
        const lines = readFileSync(
            `${process.env.HOME}/.config/gh/hosts.yml`,
            "utf-8",
        )
            .split("\n")
            .filter((t) => t.trim().startsWith("oauth_token:"));
        return lines[0].split("oauth_token:")[1].trim();
    }
};

let run = (renderDrafts: boolean) => {
    let outputDir = "./test/pages/";
    let inputDir = "./test";

    let projects = processProjects(inputDir, outputDir);
    let posts = processBlog(!renderDrafts, inputDir, outputDir);
    let poetry = processPoetry(inputDir, outputDir);
    let talks = processTalks(inputDir, outputDir);

    if (process.env.UPLOAD_GISTS) {
        const token = getToken();

        posts
            .filter((post) => post.config.gist)
            .forEach(async (post) => {
                if (post.body.type !== "Markdown") {
                    throw new Error(
                        `gist post not markdown? ${post.config.fileName}`,
                    );
                }

                const fileName = basename(post.config.fileName);
                const withHeader =
                    `# ${post.config.title}\n\n${
                        post.config.draft
                            ? ""
                            : `[View rendered & in context at jaredforsyth.com](https://jaredforsyth.com/posts/${chopSuffix(
                                  basename(post.config.fileName),
                              )})\n\n`
                    }` + post.body.body;

                if (post.config.gist === true) {
                    const res = await fetch(`https://api.github.com/gists`, {
                        method: "POST",
                        headers: {
                            Accept: "application/vnd.github+json",
                            Authorization: `Bearer ${token}`,
                            "X-GitHub-Api-Version": "2022-11-28",
                        },
                        body: JSON.stringify({
                            files: { [fileName]: { content: withHeader } },
                        }),
                    });
                    const json: any = await res.json();
                    console.log(
                        "Uploaded",
                        post.config.fileName,
                        res.status,
                        json.id,
                        json.html_url,
                    );
                    const full = inputDir + "/" + post.config.fileName;
                    const text = readFileSync(full, "utf8");
                    const changed = text.replace(
                        /^gist:\s*true$/,
                        "gist: " + json.id,
                    );
                    if (text === changed) {
                        console.warn(
                            `📛 Unable to automatically update file. You must add the gist ID to ${post.config.fileName}`,
                        );
                    } else {
                        writeFileSync(full, changed, "utf8");
                    }
                    return;
                }
                const current = (await fetch(
                    `https://api.github.com/gists/${post.config.gist}`,
                    {
                        headers: {
                            Accept: "application/vnd.github+json",
                        },
                    },
                ).then((res) => res.json())) as {
                    files: { [name: string]: { content: string } };
                };
                console.log(post.config.fileName, post.body.body.length);
                if (!current.files[fileName]) {
                    throw new Error(`no file ${fileName}`);
                }
                if (current.files[fileName].content === withHeader) {
                    return console.log("- already up to date", fileName);
                }
                fetch(`https://api.github.com/gists/${post.config.gist!}`, {
                    method: "PATCH",
                    headers: {
                        Accept: "application/vnd.github+json",
                        Authorization: `Bearer ${token}`,
                        "X-GitHub-Api-Version": "2022-11-28",
                    },
                    body: JSON.stringify({
                        files: {
                            [fileName]: {
                                content: withHeader,
                            },
                        },
                    }),
                }).then((res) => {
                    console.log("Uploaded", post.config.fileName, res.status);
                });
            });
    }

    // setupRedirectsForOldPosts(outputDir, posts);

    /* Home page */
    writeFileSync(
        outputDir + "/index.html",
        render(projects, posts, talks, poetry),
        "utf8",
    );

    saveTwitterCache();
    console.log("Finished!");
};

run(!!process.env.DRAFTS);
