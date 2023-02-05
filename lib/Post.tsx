/** @jsx jsx */
import { A, CssFn, Hover, jsx, Media, Sub } from './sjsx';
import * as Shared from './Shared';
import { BodyWithSmallAboutMeColumn } from './AboutMe';
import { startPage } from './Css';
import { nmNode, parseNotableMind, renderNm, wordCount } from './NotableMind';
import { PageHead, triple } from './Shared';
import { doc } from './Toml';
import { config } from './Types';
import { chopSuffix } from './Util';
import { rssWrapper } from './Rss';
import { process } from './MarkdownParser';
import MarkdownIt from 'markdown-it';

export type postBody =
    | { type: 'Html'; body: string }
    | { type: 'Markdown'; body: string }
    | { type: 'Nm'; body: nmNode[] };

export type post = {
    config: config;
    body: postBody;
    intro?: postBody | null;
};

// let (|?) = (x, y) =>
//   switch (x) {
//   | None => y
//   | Some(x) => x
//   };
// let (|?>) = (x, f) =>
//   switch (x) {
//   | None => None
//   | Some(x) => f(x)
//   };
// let (|?>>) = (x, f) =>
//   switch (x) {
//   | None => None
//   | Some(x) => Some(f(x))
//   };
// let (|!) = (x, y) =>
//   switch (x) {
//   | None => failwith(y)
//   | Some(x) => x
//   };

// let spacer = Shared.spacer;
// let showDate = Shared.showDate;

export let PostAbout = ({
    draft = false,
    css,
    date,
    tags,
    tagPrefix,
    withPic = true,
}: {
    draft?: boolean;
    css: CssFn;
    date: triple;
    tags?: string[];
    tagPrefix?: string;
    withPic?: boolean;
}) => {
    return (
        <div
            className={css([
                A('color', Shared.Colors.lightText),
                A('font-family', 'Open sans'),
                A('font-size', '14px'),
                A('display', 'flex'),
                A('flex-direction', 'row'),
                A('align-items', 'center'),
                A('justify-content', 'flex-start'),
            ])}
        >
            <Shared.showDate date={date} />
            {Shared.spacer(8)}
            {tags && tags.length > 0 ? ' · ' : ''}
            {Shared.spacer(8)}
            {tags
                ?.map((tag) => (
                    <a
                        href={(tagPrefix ?? '') + '/tags/' + tag + '/'}
                        className={css([A('text-decoration', 'none')])}
                    >
                        {tag}
                    </a>
                ))
                .join(', ' + Shared.spacer(4))}
            {Shared.spacer(8)}
            {draft ? (
                <span style="background-color: red; padding: 4px 8px; display: inline-block; color: white; border-radius: 4px">
                    draft
                </span>
            ) : (
                ''
            )}
        </div>
    );
};

// let pageWithTopAndBottom = Shared.pageWithTopAndBottom;
// let pageHead = Shared.pageHead;

export let renderBody = ({ type, body }: postBody) => {
    switch (type) {
        case 'Markdown':
            return process(body);
        case 'Html':
            return body;
        case 'Nm':
            return renderNm(body);
    }
};

// SANDBOXED
const loadComments = (
    gist: string,
    styles: {
        comment: string;
        body: string;
        avatar: string;
        top: string;
    },
) => {
    const node = document.getElementById('comments')!;

    node.textContent = 'Fetching from github...';
    const loadMarkdown = (): Promise<typeof MarkdownIt> =>
        new Promise((resolve, reject) => {
            const script = document.createElement('script');
            script.src =
                'https://cdnjs.cloudflare.com/ajax/libs/markdown-it/13.0.1/markdown-it.min.js';
            script.onload = () => resolve(window.markdownit);
            script.onerror = () =>
                reject(new Error('Unable to load markdown-it'));
            document.head.append(script);
        });
    fetch(`https://api.github.com/gists/${gist}/comments?per_page=100`, {
        headers: {
            Accept: 'application/vnd.github+json',
        },
    })
        .then((res) => res.json())
        .then((raw) => {
            const comments = raw as {
                body: string;
                id: string;
                url: string;
                created_at: string;
                user: {
                    login: string;
                    avatar_url: string;
                    html_url: string;
                };
            }[];

            if (!comments.length) {
                node.textContent = 'No comments yet!';
                return;
            }
            loadMarkdown().then((markdown) => {
                node.textContent = '';
                comments.forEach((comment) => {
                    const cm = document.createElement('div');
                    cm.className = styles.comment;
                    cm.innerHTML = `
                    <div class="${styles.top}">
                    <img src="${comment.user.avatar_url}"
                    class="${styles.avatar}" />
                    <a href="https://gist.github.com/${gist}?permalink_comment_id=${
                        comment.id
                    }#gistcomment-${comment.id}" target="_blank">
                        ${comment.user.login} on ${new Date(
                        comment.created_at,
                    ).toLocaleDateString()}
                    </a>
                    </div>
                    <div class="${styles.body}">
                        ${markdown({ linkify: true, breaks: true }).render(
                            comment.body,
                        )}
                    </div>
                    `;
                    node.append(cm);
                });
            });
        });
};

export let renderPost = (
    posts: post[],
    {
        config: {
            title: contentTitle,
            fileName,
            description,
            date,
            tags,
            thumbnail,
            article_image,
            draft,
            gist,
        },
        body: postBody,
    }: post,
) => {
    let { css, inlineCss } = startPage();
    let body = (
        <BodyWithSmallAboutMeColumn
            css={css}
            toc={latestPosts('Latest posts', '/posts/', css, posts)}
        >
            <h1 className={css(Shared.Styles.titleWithTopMargin)}>
                {contentTitle}
            </h1>
            <PostAbout draft={draft} css={css} date={date} tags={tags} />
            {Shared.hspace(32)}
            <div className={'post-body ' + css(Shared.Styles.bodyText)}>
                {renderBody(postBody)}
            </div>
            {gist ? renderComments(css, gist) : null}
        </BodyWithSmallAboutMeColumn>
    );

    return (
        <html>
            <PageHead
                title={contentTitle}
                description={description}
                thumbnail={thumbnail}
                article_image={article_image}
                extraHead={`
                `}
            >
                <style> {inlineCss()} </style>
            </PageHead>
            {body}
        </html>
    );
};

let rss = (
    title: string,
    urlBase: string,
    posts: post[],
    tags: [string, number][],
) => {
    return rssWrapper(
        title,
        posts.map(({ config, intro, body }) => {
            let href = urlBase + '/' + chopSuffix(config.fileName) + '/';
            let readTime = Shared.minuteReadText(config.wordCount);
            let description =
                (intro ? renderBody(intro) + ' ' : '') +
                readTime +
                (config.draft ? ' [DRAFT]' : '');
            return {
                title: config.title,
                description,
                url: href,
                content: renderBody(body),
                //  descriptionHtml: description |> Rss.escapeContent,
                date: config.date,
                category: undefined,
            };
        }),
    );
};

export let postList = (
    urlBase: string,
    posts: post[],
    tags: [string, number][],
    contentTitle: string,
    showReadTime = true,
    tagPrefix = '',
) => {
    let { css, inlineCss } = startPage();

    let body = (
        <BodyWithSmallAboutMeColumn
            css={css}
            toc={
                <div
                    className={css([
                        A('font-size', '16px'),
                        A('line-height', '20px'),
                        A('margin-bottom', '32px'),
                    ])}
                >
                    {tags
                        .map(([tag, count]) => (
                            <a
                                href={tagPrefix + '/tags/' + tag + '/'}
                                className={css([
                                    A('color', 'currentColor'),
                                    A('white-space', 'nowrap'),
                                    A('margin-right', '8px'),
                                    ...Shared.Styles.hoverUnderline,
                                ])}
                            >
                                {tag}
                                {'' + count + ''}
                            </a>
                        ))
                        .join('\n')}
                </div>
            }
        >
            <div className={css([A('flex', '3'), A('padding', '32px')])}>
                <div
                    className={css([
                        A('padding', '1px'),
                        A('position', 'relative'),
                    ])}
                >
                    <h1
                        className={css([
                            A('text-align', 'center'),
                            ...Shared.Styles.title,
                        ])}
                    >
                        {contentTitle}
                    </h1>
                    <a href="rss.xml">RSS Feed</a>
                </div>
                {posts
                    .map(({ config, intro: teaser }) => {
                        let href = '/' + chopSuffix(config.fileName) + '/';
                        let readTime = Shared.minuteReadText(config.wordCount);
                        return (
                            <div>
                                <a
                                    href={href}
                                    className={css([
                                        A('color', 'currentColor'),
                                        A('text-decoration', 'none'),
                                    ])}
                                >
                                    <h2> {config.title} </h2>
                                </a>
                                <PostAbout
                                    css={css}
                                    date={config.date}
                                    tags={config.tags}
                                    withPic={false}
                                    tagPrefix={tagPrefix}
                                />
                                {
                                    teaser ? (
                                        <div
                                            className={css([
                                                A('padding-top', '16px'),
                                                ...Shared.Styles.bodyText,
                                            ])}
                                        >
                                            {renderBody(teaser)}
                                        </div>
                                    ) : null
                                    // (Omd.to_html(Omd.of_string(teaser)))
                                }
                                {showReadTime ? (
                                    <a
                                        className={css([
                                            A('font-size', '24px'),
                                        ])}
                                        href={href}
                                    >
                                        {readTime}
                                    </a>
                                ) : (
                                    ''
                                )}
                            </div>
                        );
                    })
                    .join("\n<div style='height: 32px'></div>\n")}
            </div>
        </BodyWithSmallAboutMeColumn>
    );

    let main = (
        <html>
            <PageHead
                title={contentTitle}
                description="Things Jared has written about"
            >
                <style> {inlineCss()} </style>
            </PageHead>
            {body}
        </html>
    );
    return {
        main,
        rss: rss(contentTitle + ' | JaredForsyth.com', urlBase, posts, tags),
    };
};

// open Types;
let defaultConfig = (fileName: string): config => ({
    fileName,
    title: 'JaredForsyth.com',
    tags: [],
    categories: [],
    date: [0, 0, 0] /* Year, Month, Day */,
    draft: false,
    featured: false,
});

// let check = (opt, base, fn) =>
//   switch (opt) {
//   | None => base
//   | Some(value) => fn(value)
//   };

export let parseDate = (text: string): triple => {
    return text.split('-').map((m) => parseInt(m)) as triple;
};

export let parseConfig = (
    fileName: string,
    {
        strings: { title, description, date, thumbnail, article_image, gist },
        stringLists: { tags, categories },
        bools: { featured, draft },
    }: doc,
): config => {
    let config = defaultConfig(fileName);
    return {
        ...config,
        ...{
            title,
            description,
            gist,
            date: date ? parseDate(date) : config.date,
            thumbnail,
            article_image,
            tags,
            categories,
            featured,
            draft,
        },
    };
};

let parseJsonConfig = (
    config: config,
    {
        title,
        description,
        tags,
        categories,
        date,
        featured,
        thumnbail,
        article_image,
        draft,
    }: any,
): config => {
    return {
        ...config,
        ...{
            title,
            description,
            tags,
            categories,
            date: date ? parseDate(date) : undefined,
            featured,
            thumnbail,
            article_image,
            draft,
        },
    } as config;
};

let getIntro = (body: string) => {
    const parts = body.split('<!-- more -->');
    if (parts.length > 1) {
        return parts[0];
    }
    return null;
};

// let (|?>>) = (v, f) =>
//   switch (v) {
//   | Some(v) => Some(f(v))
//   | None => None
//   };

export let parsePost = (fileName: string, opts: doc, content: string): post => {
    let config = parseConfig(fileName, opts);
    let intro = getIntro(content);
    let isMarkdown = fileName.endsWith('.md');
    let wordCount = isMarkdown
        ? content.split(/[^a-zA-Z0-9-]+/).length
        : undefined;
    config.wordCount = wordCount;

    return isMarkdown
        ? {
              config,
              intro: intro ? { type: 'Markdown', body: intro } : undefined,
              body: { type: 'Markdown', body: content },
          }
        : {
              intro: intro ? { type: 'Html', body: intro } : undefined,
              config,
              body: { type: 'Html', body: content },
          };
};

let getNmIntro = (nodes: nmNode[]) => {
    // let found = [];
    // console.log(nodes.slice(0, 10).map((n) => n.content));
    let loop = (coll: nmNode[], nodes: nmNode[]): postBody | null => {
        if (nodes.length && nodes[0].content === '<!-- more -->') {
            return { type: 'Nm', body: coll.reverse() };
        }
        if (nodes.length) {
            return loop([nodes[0], ...coll], nodes.slice(1));
        }
        // switch (nodes) {
        // | [{content: "<!-- more -->"}, ..._] =>
        //   Some(Nm(List.rev(coll)))
        // | [one, ...rest] => loop([one, ...coll], rest)
        // | [] => None
        // };
        return null;
    };
    return loop([], nodes);
    // return null;
};

export let parseNm = (fileName: string, content: string): post => {
    let nodes = parseNotableMind(content);
    let config = defaultConfig(fileName);
    if (
        nodes.length &&
        nodes[0].children.length &&
        nodes[0].children[0].typ === 'note'
    ) {
        config = parseJsonConfig(
            config,
            JSON.parse(nodes[0].children[0].content),
        );
        nodes = nodes[0].children.slice(1);
    }
    // console.log(nodes);
    // fail;
    // let (nodes, config) =
    //   switch (nodes) {
    //   | [
    //       {
    //         children: [{typ: "note", content: attrs}, ...otherChildren],
    //         content,
    //       },
    //     ] => (
    //       otherChildren,
    //       parseJsonConfig(
    //         {...config, title: content},
    //         Yojson.Basic.from_string(attrs),
    //       ),
    //     )
    //   | _ => (nodes, config)
    //   };
    return {
        config: {
            ...config,
            wordCount: nodes.reduce((a, b) => a + wordCount(b), 0),
        },
        intro: getNmIntro(nodes),
        body: { type: 'Nm', body: nodes },
    };
};

export function latestPosts(
    title: string,
    base: string,
    css: CssFn,
    posts: post[],
): string {
    return (
        <div
            className={css([
                A('padding', '0 16px'),
                A('flex-shrink', '1'),
                A('overflow', 'auto'),
            ])}
        >
            <a
                href={base}
                className={css([
                    A('text-decoration', 'none'),
                    A('color', 'inherit'),
                    Hover([['text-decoration', 'underline']]),
                ])}
            >
                <div className={css([])}>{title}</div>
            </a>
            {Shared.hspace(8)}
            {posts
                .slice(0, 5)
                .map(
                    ({
                        config: {
                            title,
                            date: [year, month, day],
                            fileName,
                        },
                    }) => {
                        let href = '/' + chopSuffix(fileName) + '/';
                        return (
                            <a
                                href={href}
                                className={css([
                                    A('font-size', '16px'),
                                    A('margin-bottom', '16px'),
                                    A('display', 'block'),
                                    A('line-height', '20px'),
                                    ...Shared.Styles.subtleLink,
                                ])}
                            >
                                <div> {title} </div>
                                <div
                                    className={css([
                                        A('color', Shared.Colors.lightText),
                                        ...Shared.Styles.row,
                                    ])}
                                >
                                    {year}
                                    {Shared.monthName(month)}
                                    {day}
                                </div>
                            </a>
                        );
                    },
                )
                .join('\n')}
            {Shared.hspace(32)}
        </div>
    );
}

function renderComments(
    css: (
        items: import('/Users/jared/clone/sites/jnew/lib/sjsx').CssAttr[],
    ) => string,
    gist: string,
) {
    return (
        <section className={css(Shared.Styles.bodyText)}>
            <div
                className={css([
                    A('font-size', '150%'),
                    A('margin-top', '32px'),
                ])}
            >
                Comments
            </div>
            <div
                className={css([
                    A('font-size', '80%'),
                    A('margin-bottom', '32px'),
                ])}
            >
                courtesy of{' '}
                <a href={'https://gist.github.com/' + gist} target="_blank">
                    github gists
                </a>
            </div>
            <div id="comments" data-gist={gist}>
                Loading comments...
            </div>
            <script async>
                {`const loadComments = ${loadComments.toString()};
                        const styles = ${JSON.stringify({
                            top: css([
                                A('display', 'flex'),
                                A('align-items', 'center'),
                            ]),
                            comment: css([A('padding', '8px')]),
                            avatar: css([
                                A('width', '40px'),
                                A('height', '40px'),
                                A('border-radius', '10px'),
                                A('margin-right', '16px'),
                            ]),
                            body: css([
                                A('padding', '16px'),
                                A('margin-top', '16px'),
                                A('border-radius', '10px'),

                                A('background', 'var(--color-lightOrange)'),
                            ]),
                        })};
                        let observer = new IntersectionObserver((entries) => {
                            if (!entries[0].isIntersecting) return;
                            observer.disconnect();
                            loadComments(${JSON.stringify(gist)}, styles);
                        }, {rootMargin: '200px'});
                        observer.observe(document.getElementById('comments'));
                        `}
            </script>
            <a
                href={`https://gist.github.com/${gist}#comments`}
                target="_blank"
                className={css([
                    A('margin-top', '32px'),
                    A('display', 'block'),
                ])}
            >
                Add a comment on github
            </a>
        </section>
    );
}
