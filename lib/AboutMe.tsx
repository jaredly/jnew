/** @jsx jsx */
import { Consts, px } from './Shared';
import { A, CssFn, Hover, jsx, Media, Sub } from './sjsx';
import * as Shared from './Shared';
// open Shared.Infix;
// module Consts = Shared.Consts;
// let px = Shared.px;

export let column = ({
    nav = false,
    contents,
    css,
}: {
    css: CssFn;
    contents?: string;
    nav?: boolean;
}) => (
    <div
        className={css([
            Media('min-width: ' + px(Consts.Media.oneCol), [
                ['position', 'sticky'],
                /* ("top", px(Consts.colPadding / 2)), */
                ['height', 'calc(100vh - ' + px(Consts.colPadding * 2) + ')'],
                ['display', 'flex'],
                ['flex-wrap', 'nowrap'],
                ['flex-direction', 'column'],
            ]),
            A('top', px(Consts.colPadding)),
            Media('max-width: ' + px(Consts.Media.skinnyFourCol), [
                ['top', px(Consts.colPadding / 2)],
            ]),
            Media('max-width: ' + px(Consts.Media.oneCol), [
                ['top', px(Consts.colPaddingSmall)],
            ]),
        ])}
    >
        {Shared.myBigFaceStatic(css)}
        {Shared.hspace(Consts.bigSpace)}
        <div
            className={css([
                A('text-align', 'center'),
                A('font-size', px(Consts.jaredSize)),
            ])}
        >
            I'm Jared Forsyth
        </div>
        {Shared.hspace(Consts.bigSpace)}
        <div
            className={css([
                A('font-size', px(Consts.titleSize)),
                A('line-height', '36px'),
                A('flex', '1'),
                A('display', 'flex'),
                A('flex-direction', 'column'),
                A('flex-wrap', 'nowrap'),
                A('text-align', 'left'),
            ])}
        >
            {nav ? (
                <div
                    className={css([
                        A('margin-bottom', Shared.px(Consts.bigSpace)),
                        Sub('a', [['margin', '0 8px']]),
                        A('justify-content', 'center'),
                        ...Shared.Styles.row,
                    ])}
                >
                    <a href="/">home</a>
                    <a href="/posts/">posts</a>
                    <a href="/projects/">projects</a>
                    <a href="/talks/">talks</a>
                </div>
            ) : (
                ''
            )}
            {contents ?? (
                <div
                    className={css([
                        A('flex-shrink', '1'),
                        A('overflow', 'auto'),
                    ])}
                >
                    <p>Relationships are what it's all about.</p>
                    <p>
                        I’m currently a mobile & web developer for Khan Academy,
                        where we’re working to bring a free, world-class
                        education to anyone anywhere.
                    </p>
                </div>
            )}
            <div
                style="text-align:center"
                className={css([
                    Sub('a', [['text-decoration', 'none']]),
                    Sub('a:hover', [['text-decoration', 'underline']]),
                    A('font-family', 'Open sans, sans-serif'),
                    A('font-size', '20px'),
                ])}
            >
                <a href="https://twitter.com/jaredforsyth">
                    twitter/@jaredforsyth
                </a>
                <br />
                <a href="https://mastodon.social/@jaredly">
                    @jaredly@mastodon.social
                </a>
                <br />
                <a href="https://github.com/jaredly">github/@jaredly</a>
                <br />
                <a href="/posts/rss.xml">posts rss feed</a>
            </div>
        </div>
    </div>
);

export let BodyWithAboutMeColumn = ({
    css,
    children,
}: {
    css: CssFn;
    children?: string[];
}) => {
    const Styles = Shared.Styles;

    let body = (
        <body className={css(Styles.fullBody)} lang="en">
            <div className={css(Styles.columnWrapper)}>
                <div
                    className={css([
                        Media('min-width: ' + px(Shared.Consts.Media.oneCol), [
                            ['max-width', '400px'],
                        ]),
                        A('background-color', Shared.Colors.lightOrange),
                        ...Styles.column,
                    ])}
                >
                    {column({ css })}
                </div>
                {children?.join('\n')}
            </div>
        </body>
    );

    return body;
};

export let BodyWithSmallAboutMeColumn = ({
    css,
    toc,
    children,
}: {
    css: CssFn;
    toc: string;
    children?: string[];
}) => {
    const Styles = Shared.Styles;

    let body = (
        <body className={css(Styles.fullBody)} lang="en">
            <div
                className={css([
                    ...Styles.columnWrapper,
                    A('justify-content', 'flex-start'),
                    A('flex-wrap', 'nowrap'),
                    Media('max-width: ' + '1060px', [
                        ['justify-content', 'center'],
                    ]),
                ])}
            >
                <div
                    className={css([
                        A('background-color', Shared.Colors.lightOrange),
                        A('max-width', '400px'),
                        A('flex-shrink', '1'),
                        A('min-width', '300px'),
                        Media('max-width: ' + '1060px', [['display', 'none']]),
                        ...Styles.colPaddingRules,
                    ])}
                >
                    {column({ nav: true, contents: toc, css })}
                </div>
                <div
                    style="flex-basis: 100px; flex-shrink: 1"
                    className={css([
                        Media('max-width: ' + '1060px', [['display', 'none']]),
                    ])}
                />
                <div
                    className={css([
                        A('max-width', '700px'),
                        Media('max-width: ' + '1060px', [
                            ['flex-shrink', '1'],
                            ['min-width', '0'],
                        ]),
                    ])}
                >
                    <main role="main">
                        <article> {children?.join('\n')} </article>
                    </main>
                    <div style="height: 128px" />
                </div>
                <div
                    style="flex-basis: 100px; flex-shrink: 1"
                    className={css([
                        Media('max-width: ' + '1060px', [['display', 'none']]),
                    ])}
                />
            </div>
        </body>
    );

    return body;
};
