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
import { process, processPoetry } from './MarkdownParser';
import MarkdownIt from 'markdown-it';
import { latestPosts, post, PostAbout } from './Post';

export let renderPoem = (
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
    if (postBody.type !== 'Markdown') {
        throw new Error('poems must be markdown');
    }
    const rendered = processPoetry(postBody.body);
    let body = (
        <BodyWithSmallAboutMeColumn
            css={css}
            toc={latestPosts('Latest poems', '/poems/', css, posts)}
        >
            <h1 className={css(Shared.Styles.titleWithTopMargin)}>
                {contentTitle}
            </h1>
            <PostAbout
                draft={draft}
                css={css}
                date={date}
                tags={tags}
                tagPrefix="/poems"
            />
            {Shared.hspace(32)}
            <div className={'post-body ' + css(Shared.Styles.bodyText)}>
                {rendered}
            </div>
        </BodyWithSmallAboutMeColumn>
    );

    return (
        <html>
            <PageHead
                title={contentTitle}
                description={description}
                thumbnail={thumbnail}
                article_image={article_image}
            >
                <style> {inlineCss()} </style>
            </PageHead>
            {body}
        </html>
    );
};
