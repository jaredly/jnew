/** @jsx jsx */
import { A, CssFn, Hover, jsx, Media } from './sjsx';
export const Colors = {
    text: '#333',
    lightText: '#999',
    red: '#692900',
    lightOrange: '#fff4ef',
    darkGreen: '#147429',
    green: '#1fad3e',
};

export let px = (n: number) => n + 'px';

export const Consts = {
    colPadding: 40,
    colPaddingSmall: 16,
    jaredSize: 36,
    titleSize: 24,
    statusSize: 20,
    githubSize: 16,
    updatesSize: 14,
    smallSpace: 8,
    medSpace: 16,
    // bigSpace: 32,
    bigSpace: 40,

    Media: {
        oneCol: 835,
        twoCol: 1239,
        threeCol: 1400,
        skinnyFourCol: 1650,
    },
};

export let Twitter = ({
    contentTitle,
    description,
    article_image,
    thumbnail,
}: {
    contentTitle: string;
    description: string;
    article_image?: string;
    thumbnail?: string;
}) => {
    return (
        <fragment>
            <meta
                name="twitter:card"
                content={!article_image ? 'summary' : 'summary_large_image'}
            />
            <meta name="twitter:title" content={contentTitle} />
            <meta name="twitter:description" content={description} />
            <meta
                name="twitter:image"
                content={
                    article_image ??
                    thumbnail ??
                    'https://jaredforsyth.com/images/logo/JF_black_128.png'
                }
            />
            <meta name="twitter:site" content="https://jaredforsyth.com" />
            <meta name="twitter:creator" content="@jaredforsyth" />
        </fragment>
    );
};

export let MobileMeta = () => (
    <fragment>
        <meta name="viewport" content="width=device-width, initial-scale=1.0" />
        <meta name="HandheldFriendly" content="True" />
        <meta name="MobileOptimized" content="320" />
        <meta name="apple-mobile-web-app-capable" content="yes" />
        <meta
            name="apple-mobile-web-app-status-bar-style"
            content="black-translucent"
        />
    </fragment>
);

export let MetaHead = (props: {
    title: string;
    description: string;
    thumbnail?: string;
    article_image?: string;
}) => {
    return (
        <fragment>
            <meta charset="utf8" />
            <meta name="description" content={props.description} />
            <MobileMeta />
            <Twitter
                contentTitle={props.title}
                description={props.description}
                article_image={props.article_image}
                thumbnail={props.thumbnail}
            />
            <meta property="og:type" content="article" />
            <meta property="og:title" content={props.title} />
            <meta property="og:description" content={props.description} />
            <link rel="shortcut icon" href="/images/logo/JF_black_32.png" />
            <title> {props.title} </title>
        </fragment>
    );
};

export let PageHead = ({
    title: contentTitle,
    description,
    thumbnail,
    article_image,
    extraHead,
    children,
}: {
    title: string;
    description: string;
    thumbnail?: string;
    article_image?: string;
    extraHead?: string;
    children?: string[];
}) => {
    contentTitle = contentTitle + ' | Jared Forsyth.com';
    description = description ?? 'Thoughts about programming, mostly';
    return (
        <head>
            <MetaHead
                title={contentTitle}
                description={description}
                thumbnail={thumbnail}
                article_image={article_image}
            />
            <link
                rel="stylesheet"
                href="//brick.a.ssl.fastly.net/Linux+Libertine:400,400i,700,700i/Open+Sans:400,400i,700,700i"
            />
            <link
                href="https://fonts.googleapis.com/css?family=Inconsolata:400,700"
                rel="stylesheet"
            />
            <link
                href="https://cdnjs.cloudflare.com/ajax/libs/prism/1.17.1/themes/prism.min.css"
                rel="stylesheet"
            />
            <style>
                {`
      div {
        flex-shrink: 0;
        flex-wrap: wrap;
        box-sizing: border-box;
        min-height: 0;
        min-width: 0;
      }
      span {
        box-sizing: border-box;
      }
      pre {
        line-height: 18px;
        font-size: 16px;
        /* white-space: pre-wrap; */
        overflow: auto;
        max-width: 100%;
        padding: 15px;
        /* border: 1px solid #ddd; */
        font-family: Inconsolata;
        background-color: ${Colors.lightOrange};
        color: #692900;
        border-radius: 4px;
        margin: 0;
        margin-bottom: 1em;
      }
      img {
        max-width: 100%;
      }
      li > code,
      a > code,
      p > code {
        font-size: 90%;
        padding: 4px;
        background-color: #ffede4;
        color: #692900;
        border-radius: 4px;
        hyphens: none;
      }
      blockquote {
          margin: 0;
          margin-bottom: 1em;
          padding-left: 20px;
          border-left: 5px solid ${Colors.green};
          font-style: italic;
      }
      blockquote.twitter-tweet {
        font-style: normal;
        border: 1px solid ${Colors.green};
        border-radius: 4px;
        padding: 8px 16px;
        font-size: 20px;
      }
      div.note {
        font-style: italic;
        flex-wrap: nowrap;
        display: flex;
        align-items: flex-start;
        font-size: 90%;
      }
      div.note::before {
        margin-right: 8px;
        content: 'i';
        border: 1px solid #ccc;
        padding: 2px 10px;
        border-radius: 50px;
        font-family: arial;
        font-style: normal;
        font-size: 20px;
        display: inline;
        line-height: 1;
        margin-top: 5px;
      }
      a {
        color: ${Colors.green};
        /* color: #28cc4c;
        text-decoration-color: #28cc4c; */
      }
      h1 > a, h2 > a, h3 > a, h4 > a, h5 > a, h6 > a {
        text-decoration: none;
        color: inherit;
      }
      h1 > a:hover, h2 > a:hover, h3 > a:hover, h4 > a:hover, h5 > a:hover, h6 > a:hover,
      h1 > a:focus, h2 > a:focus, h3 > a:focus, h4 > a:focus, h5 > a:focus, h6 > a:focus {
        text-decoration: underline;
        color: ${Colors.green};
      }
      ul, ol {
        margin-top: 0;
      }
      .post-body h1 {
        font-family: Open sans;
      }
      h2 {
        padding-bottom: 16px;
        font-size: 32px;
        margin: 0;
        margin-top: 1em;
      }
      p {
        margin: 0;
        padding-bottom: 1em;
      }
      blockquote > p:last-child {
        padding-bottom: 0;
      }
      h3 {
        margin-bottom: 8px;
        font-family: Open sans;
        font-weight: normal;
        margin-top: 8px;
        color: #6e6e6e;
        font-size: 30px;
      }
      h5 {
        font-family: open sans;
        margin: 0;
        margin-top: 16px;
      }
      /* a:visited {
        color: #333;
      } */
      `}
            </style>
            {children?.join('\n')}
            {extraHead}
        </head>
    );
};

export let PageWithTopAndBottom = ({
    css,
    backgroundImage,
    top,
    middle,
    bottom,
}: {
    css: CssFn;
    backgroundImage?: string;
    top: string;
    middle: string;
    bottom: string;
}) => {
    return (
        <body
            className={css([
                A('font-family', 'Linux Libertine'),
                A('color', Colors.text),
            ])}
            lang="en"
        >
            <a
                href="/"
                className={css([
                    A('position', 'fixed'),
                    A('top', '10px'),
                    A('left', '10px'),
                    A('padding', '8px'),
                    A('background-color', 'black'),
                    Media('max-width: 600px', [['display', 'none']]),
                ])}
            >
                <div
                    className={css([
                        A('height', '32px'),
                        A('width', '32px'),
                        A('background-size', 'cover'),
                    ])}
                    style="background-image: url(/images/logo/JF_64.png)"
                />
            </a>
            <main role="main">
                <article lang="en">
                    <header>
                        {!backgroundImage ? (
                            ''
                        ) : (
                            <div
                                style={
                                    'background-image: url(' +
                                    backgroundImage +
                                    ')'
                                }
                                className={css([
                                    A('height', '400px'),
                                    A('background-size', 'cover'),
                                    A('background-position', 'center'),
                                    A('margin', '-8px -8px 0'),
                                ])}
                            />
                        )}
                        <div
                            className={css([
                                A('max-width', '700px'),
                                A('margin', '0 auto 48px'),
                            ])}
                        >
                            {top}
                        </div>
                    </header>
                    <div
                        className={css([
                            A('max-width', '700px'),
                            A('margin', '0 auto'),
                        ])}
                    >
                        {middle}
                    </div>
                </article>
            </main>
            <footer>
                <div
                    className={css([
                        A('max-width', '700px'),
                        A('margin', '100px auto 150px'),
                        A('text-align', 'center'),
                    ])}
                >
                    {bottom}
                </div>
            </footer>
        </body>
    );
};

export let spacer = (num: number) => (
    <div style={'flex-basis: ' + num + 'px'} />
);
export let hspace = (num: number) => <div style={'height: ' + num + 'px'} />;
export let vspace = (num: number) => <div style={'width: ' + num + 'px'} />;

export let monthName = (month: number) => {
    switch (month) {
        case 1:
            return 'January';
        case 2:
            return 'February';
        case 3:
            return 'March';
        case 4:
            return 'April';
        case 5:
            return 'May';
        case 6:
            return 'June';
        case 7:
            return 'July';
        case 8:
            return 'August';
        case 9:
            return 'September';
        case 10:
            return 'October';
        case 11:
            return 'November';
        case 12:
            return 'December';
        default:
            return 'NOTAMONTH';
        // throw new Error('Invalid month ' + month);
    }
};

export let shortMonth = (month: number) => {
    switch (month) {
        case 1:
            return 'Jan';
        case 2:
            return 'Feb';
        case 3:
            return 'Mar';
        case 4:
            return 'Apr';
        case 5:
            return 'May';
        case 6:
            return 'Jun';
        case 7:
            return 'Jul';
        case 8:
            return 'Aug';
        case 9:
            return 'Sep';
        case 10:
            return 'Oct';
        case 11:
            return 'Nov';
        case 12:
            return 'Dec';
        default:
            throw new Error('Invalid month ' + month);
    }
};

export let showDate = ({ date: [year, month, day] }: { date: triple }) => {
    <fragment>
        {monthName(month)}
        {spacer(4)}
        {day + ','}
        {spacer(4)}
        {year}
    </fragment>;
};
export type triple = [number, number, number];

export let rfcDate = ([year, month, day]: triple) => {
    day + ' ' + shortMonth(month) + ' ' + year;
};

export let dateSort = ([y1, m1, d1]: triple, [y2, m2, d2]: triple) => {
    switch (y1 - y2) {
        case 0:
            switch (m1 - m2) {
                case 0:
                    return d1 - d2;
                default:
                    return m1 - m2;
            }
        default:
            return y1 - y2;
    }
};

export let myBigFaceStatic = (css: CssFn) => {
    let size = 160;
    let ssize = size + 'px';
    return (
        <a
            href="/"
            className={css([
                A('margin', '0 auto'),
                A('width', ssize),
                A('overflow', 'hidden'),
                A('height', ssize),
                A('display', 'block'),
                A('border-radius', '50%'),
                A('border', '5px solid white'),
                A('background-color', 'white'),
                A('box-shadow', '0 1px 1px rgba(0, 0, 0, 0.3)'),
            ])}
        >
            <img
                src="https://www.gravatar.com/avatar/313878fc8f316fc3fe4443b13913d0a4.png?s=200"
                alt="Jared Forsyth"
                className={css([
                    A('display', 'block'),
                    A('width', ssize),
                    A('height', ssize),
                ])}
            />
        </a>
    );
};

export let myBigFace = (css: CssFn) => (
    <a
        href="/"
        style="background-image: url(https://www.gravatar.com/avatar/313878fc8f316fc3fe4443b13913d0a4.png?s=200)"
        className={css([
            A('width', '120px'),
            A('height', '120px'),
            A('position', 'absolute'),
            A('top', '-60px'),
            A('right', '50%'),
            A('margin-right', '-60px'),
            A('background-size', 'cover'),
            A('border-radius', '60px'),
            A('z-index', '99'),
            A('text-indent', '-9999px'),
            A('border', '3px solid white'),
            A('background-color', 'white'),
            A('box-shadow', '0 1px 1px rgba(0, 0, 0, 0.3)'),
        ])}
    >
        "Jared Forsyth"
    </a>
);

export const colPaddingRules = [
    A('padding', px(Consts.colPadding)),
    Media('max-width: ' + px(Consts.Media.skinnyFourCol), [
        ['padding', px(Consts.colPadding / 2)],
    ]),
    Media('max-width: ' + px(Consts.Media.oneCol), [
        ['padding', px(Consts.colPaddingSmall)],
    ]),
];
export const title = [
    A('font-size', '56px'),
    A('margin-bottom', '16px'),
    A('padding-top', '32px'),
    Media('max-width: 600px', [
        ['font-size', '32px'],
        ['margin-top', '40px'],
    ]),
];
export const Styles = {
    title: title,

    titleWithTopMargin: [
        A('margin-top', '100px'),
        Media('max-width: 600px', [['margin-top', '40px']]),
        ...title,
    ],

    bodyText: [
        A('font-size', '24px'),
        A('line-height', '36px'),
        A('hyphens', 'auto'),
        Media('max-width: 600px', [
            ['font-size', '20px'],
            ['line-height', '30px'],
        ]),
    ],

    colPaddingRules: colPaddingRules,

    row: [A('display', 'flex'), A('flex-direction', 'row')],

    column: [
        A('flex', '1'),
        A('min-width', '300px'),
        Media('max-width: ' + px(Consts.Media.oneCol), [
            ['min-width', '300px'],
        ]),
        ...colPaddingRules,
    ],

    statusText: [A('font-size', px(Consts.statusSize)), A('color', Colors.red)],

    metaText: [
        A('font-family', 'Open sans, sans-serif'),
        A('color', Colors.lightText),
        A('font-size', px(Consts.updatesSize)),
    ],

    hoverUnderline: [
        A('text-decoration', 'none'),
        Hover([['text-decoration', 'underline']]),
    ],

    subtleLink: [
        A('color', 'currentColor'),
        A('text-decoration', 'none'),
        Hover([['text-decoration', 'underline']]),
    ],

    basicImage: [
        /* A("box-shadow", "0 0 5px #aaa"), */
        A('border', '1px solid #ddd'),
        A('width', '100%'),
        A('object-fit', 'cover'),
    ],

    columnWrapper: [
        A('flex-direction', 'row'),
        A('padding', '0 ' + px(Consts.colPadding / 2)),
        A('display', 'flex'),
        A('justify-content', 'center'),
        A('align-items', 'stretch'),
        A('flex-wrap', 'wrap'),
        Media('max-width: ' + px(Consts.Media.oneCol), [
            ['padding', px(Consts.colPaddingSmall / 2)],
        ]),
    ],

    fullBody: [
        A('font-family', 'Linux Libertine'),
        A('color', Colors.text),
        A('margin', '0'),
        A('padding', '0'),
    ],

    pinkDivider: [
        A('height', '3px'),
        A('background-color', Colors.lightOrange),
        A('box-shadow', '0 0.5px 1px #c2c2c2'),
    ],
};

export const Header = ({
    css,
    href,
    title,
}: {
    css: CssFn;
    href: string;
    title: string;
}) => (
    <a
        href={href}
        className={css([
            A('color', Colors.darkGreen),
            ...Styles.hoverUnderline,
        ])}
    >
        <h1
            className={css([
                A('margin-top', '0'),
                A('font-size', px(Consts.jaredSize)),
            ])}
        >
            {title}
        </h1>
    </a>
);

export let minuteRead = (wordCount: number) => Math.round(wordCount / 180);

export let minuteReadText = (wordCount?: number) =>
    wordCount ? minuteRead(wordCount) + ' minute read' : 'Read more';
