/** @jsx jsx */
import { A, CssFn, Hover, jsx, Media, Sub } from './sjsx';
import * as Shared from './Shared';
import { githubLink, project, statusSymbol, updateText } from './Project';
import { chopSuffix } from './Util';
import { process } from './MarkdownParser';
import { talk } from './Talk';
import { post } from './Post';
import { BodyWithAboutMeColumn } from './AboutMe';
import { startPage } from './Css';
import { PageHead } from './Shared';

const Consts = Shared.Consts;
let px = Shared.px;

const Styles = Shared.Styles;
const Header = Shared.Header;

let projectColumn = ({
    css,
    projects,
}: {
    css: CssFn;
    projects: project[];
}) => {
    return (
        <fragment>
            <Header href="/projects/" css={css} title="Projects" />
            <div className={css(Styles.row)}>
                α - alpha
                {Shared.vspace(16)}β - beta
                {Shared.vspace(16)}
                🚀 - done
                {Shared.vspace(16)}
                🛌 - retired
            </div>
            {Shared.hspace(16)}
            <div>
                {projects
                    .map(
                        ({
                            title,
                            fileName,
                            description,
                            screenshot,
                            github,
                            updates,
                            status,
                            tags,
                        }) => {
                            let href = '/' + chopSuffix(fileName) + '/';
                            return (
                                <div>
                                    <div
                                        className={css([
                                            A(
                                                'font-size',
                                                px(Consts.titleSize),
                                            ),
                                            A('display', 'flex'),
                                            A('flex-direction', 'row'),
                                            A('align-items', 'flex-end'),
                                        ])}
                                    >
                                        <a
                                            href={href}
                                            className={css([
                                                A('flex', '1'),
                                                ...Styles.subtleLink,
                                            ])}
                                        >
                                            {title}
                                            {status && statusSymbol[status] ? (
                                                <span
                                                    title={status}
                                                    className={css([
                                                        A('width', '24px'),
                                                        A(
                                                            'padding-left',
                                                            px(Consts.medSpace),
                                                        ),
                                                        A(
                                                            'display',
                                                            'inline-block',
                                                        ),
                                                        ...Styles.statusText,
                                                    ])}
                                                >
                                                    {statusSymbol[status]}
                                                </span>
                                            ) : (
                                                <span style="width: 24px; display: inline-block" />
                                            )}
                                        </a>
                                        {github
                                            ? githubLink({ css, href: github })
                                            : ''}
                                    </div>
                                    {screenshot ? (
                                        <img
                                            src={screenshot}
                                            alt={title + ' screenshot'}
                                            className={css([
                                                A(
                                                    'margin-top',
                                                    px(Consts.smallSpace),
                                                ),
                                                A('max-height', '200px'),
                                                ...Styles.basicImage,
                                            ])}
                                        />
                                    ) : (
                                        ''
                                    )}
                                    <div
                                        className={css([
                                            A(
                                                'padding-top',
                                                px(Consts.smallSpace),
                                            ),
                                            A(
                                                'font-size',
                                                px(Consts.githubSize),
                                            ),
                                            Sub('p', [
                                                [
                                                    'padding-bottom',
                                                    px(Consts.smallSpace),
                                                ],
                                            ]),
                                        ])}
                                    >
                                        {process(description)}
                                    </div>
                                    <div
                                        className={css([
                                            A('color', Shared.Colors.lightText),
                                            A('font-family', 'Open sans'),
                                            A(
                                                'font-size',
                                                px(Consts.updatesSize),
                                            ),
                                            A('display', 'flex'),
                                            A('flex-direction', 'row'),
                                        ])}
                                    >
                                        {updateText({ updates })}
                                        {tags?.length
                                            ? Shared.spacer(8) +
                                              '·' +
                                              Shared.spacer(8)
                                            : ''}
                                        {tags
                                            ?.map((tag) => (
                                                <a
                                                    className={css(
                                                        Styles.hoverUnderline,
                                                    )}
                                                    href={
                                                        '/projects/tags/' +
                                                        tag +
                                                        '/'
                                                    }
                                                >
                                                    tag
                                                </a>
                                            ))
                                            .join(',' + Shared.vspace(4))}
                                    </div>
                                </div>
                            );
                        },
                    )
                    .join('\n' + Shared.hspace(Consts.bigSpace))}
            </div>
        </fragment>
    );
};

let TalksColumn = ({ css, talks }: { css: CssFn; talks: talk[] }) => {
    return (
        <fragment>
            <Header href="/talks/" css={css} title="Talks" />
            {talks
                .map(({ title, image, slides, venues }) => {
                    let main = image ? (
                        <img
                            alt={title}
                            src={image}
                            className={css([
                                A('max-height', '300px'),
                                ...Styles.basicImage,
                            ])}
                        />
                    ) : (
                        <div
                            className={css([
                                A('font-size', px(Consts.statusSize)),
                            ])}
                        >
                            {title}
                        </div>
                    );
                    return (
                        <div>
                            {slides ? (
                                <a
                                    href={slides}
                                    target="_blank"
                                    className={css(Styles.subtleLink)}
                                >
                                    {main}
                                </a>
                            ) : (
                                main
                            )}
                            {Shared.hspace(Consts.smallSpace)}
                            <ul
                                className={css([
                                    A('font-size', px(Consts.githubSize)),
                                    A('padding', '0'),
                                    /* A("padding-left", "32px"), */
                                    A('list-style', 'none'),
                                    A('margin', '0'),
                                ])}
                            >
                                {venues
                                    .map(({ where, date, video }) => {
                                        <li>
                                            {video ? (
                                                <a
                                                    href={video}
                                                    className={css([
                                                        A(
                                                            'font-family',
                                                            'Open sans, sans-serif',
                                                        ),
                                                        A('font-size', '14px'),
                                                        ...Styles.hoverUnderline,
                                                    ])}
                                                >
                                                    where "📹"
                                                </a>
                                            ) : (
                                                where
                                            )}
                                        </li>;
                                    })
                                    .join('\n')}
                            </ul>
                        </div>
                    );
                })
                .join(
                    '\n' + Shared.hspace(Consts.bigSpace + Consts.smallSpace),
                )}
        </fragment>
    );
};

let BlogColumn = ({ css, posts }: { css: CssFn; posts: post[] }) => {
    return (
        <fragment>
            <Header href="/posts/" css={css} title="Blog posts" />
            <div>
                {posts
                    .map(
                        ({
                            config: { fileName, date, wordCount, title, draft },
                            intro,
                            body,
                        }) => {
                            let href = '/' + chopSuffix(fileName) + '/';
                            let [year, month, day] = date;
                            <a href={href} className={css(Styles.subtleLink)}>
                                <div
                                    className={css([
                                        A('font-size', px(Consts.titleSize)),
                                    ])}
                                >
                                    title
                                </div>
                                {Shared.hspace(Consts.smallSpace)}
                                <div
                                    className={css([
                                        A('display', 'flex'),
                                        A('align-items', 'center'),
                                        A('flex-direction', 'row'),
                                        ...Styles.metaText,
                                    ])}
                                >
                                    {year}
                                    {Shared.monthName(month)}
                                    {day}
                                    {draft ? (
                                        <span style="background-color: red; margin-left: 8px; padding: 4px 8px; display: inline-block; color: white; border-radius: 4px">
                                            "draft"
                                        </span>
                                    ) : (
                                        ''
                                    )}
                                    <div style="flex: 1" />
                                    {Shared.minuteReadText(wordCount)}
                                </div>
                            </a>;
                        },
                    )
                    .join('\n' + Shared.hspace(Consts.bigSpace))}
            </div>
        </fragment>
    );
};

export let render = (projects: project[], posts: post[], talks: talk[]) => {
    let { css, inlineCss } = startPage();

    let body = (
        <BodyWithAboutMeColumn css={css}>
            <div className={css(Styles.column)}>
                {BlogColumn({ css, posts })}
            </div>
            <div className={css(Styles.column)}>
                {TalksColumn({ css, talks })}
            </div>
            <div className={css([A('max-width', '700px'), ...Styles.column])}>
                {projectColumn({ css, projects })}
            </div>
        </BodyWithAboutMeColumn>
    );

    return (
        <html>
            <PageHead title="Jared Forsyth.com" description="My website">
                <style> {inlineCss()} </style>
            </PageHead>
            {body}
        </html>
    );
};
