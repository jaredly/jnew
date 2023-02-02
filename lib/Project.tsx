/** @jsx jsx */
import { A, CssFn, Hover, jsx, Media, Sub } from './sjsx';
import * as Shared from './Shared';
import { startPage } from './Css';
import { BodyWithSmallAboutMeColumn } from './AboutMe';
import { process } from './MarkdownParser';
import { chopSuffix, splitFirst } from './Util';
import { PageHead } from './Shared';
// import toml from 'toml';
import { parseDate } from './Post';
import path from 'path';
import { doc, parseToml } from './Toml';
import { raw } from './GithubIcon';

export let statusSymbol: { [key: string]: string } = {
    'semi-retired': '🛌',
    retired: '🛌',
    alpha: 'α',
    beta: 'β',
    finished: '🚀',
};

export type project = {
    title: string;
    description: string;
    longDescription?: string | null;
    screenshot?: string;
    github?: string;
    link?: string;
    tags?: string[];
    status?: string;
    id: string;
    fileName: string;
    wip: boolean;
    updates: [[number, number, number], string | null, string][];
};

// export let githubLink = (href: string) => {
//   return <a target="_blank" href style="font-size: 16px">"github"</a>
// };

export let githubLink = ({ css, href }: { css: CssFn; href: string }) => {
    return (
        <a
            title="github link"
            target="_blank"
            href={href}
            style="font-size: 16px"
        >
            {raw(
                css([
                    A('fill', Shared.Colors.green),
                    A('width', '20px'),
                    A('height', '20px'),
                ]),
            )}
        </a>
    );
};

export let monthDate = ([year, month, _]: Shared.triple) =>
    year + ' ' + Shared.monthName(month);

export let timeSpanMonths = (
    [year1, month1]: Shared.triple,
    [year2, month2]: Shared.triple,
) => {
    if (year1 == year2) {
        if (month1 == month2) {
            return monthDate([year1, month1, 0]);
        } else {
            return (
                year1 +
                ' ' +
                Shared.monthName(month1) +
                ' - ' +
                Shared.monthName(month2)
            );
        }
    } else {
        return (
            monthDate([year1, month1, 0]) +
            ' - ' +
            monthDate([year2, month2, 0])
        );
    }
};

export let updateText = ({
    num = true,
    updates,
}: {
    num?: boolean;
    updates: project['updates'];
}) => {
    if (!updates.length) {
        return '';
    }
    if (updates.length === 1) {
        return (num ? '1 update, ' : '') + monthDate(updates[0][0]);
    }
    let total = updates.length;
    let [date] = updates[0];
    let [date2] = updates[total - 1];
    return (num ? total + ' updates, ' : '') + timeSpanMonths(date2, date);
};

export let metaText = [
    A('display', 'flex'),
    A('flex-direction', 'row'),
    A('font-size', '16px'),
    A('color', Shared.Colors.lightText),
    A('font-family', 'Open sans'),
];

let renderUpdate = ({
    css,
    update: [[year, month, day], screenshot, content],
}: {
    css: CssFn;
    update: project['updates'][0];
}) => {
    return (
        <div className={css([A('margin-bottom', '17px')])}>
            <div className={css(metaText)}>
                {year}
                {Shared.monthName(month)}
                {day}
            </div>
            {process(content)}
        </div>
    );
};

export let renderProject = ({
    title: contentTitle,
    tags,
    description,
    github,
    longDescription,
    status,
    screenshot,
    updates,
}: project) => {
    let { css, inlineCss } = startPage();

    let body = (
        <BodyWithSmallAboutMeColumn css={css} toc="">
            <script async src="//platform.twitter.com/widgets.js" />
            <h1 className={css(Shared.Styles.titleWithTopMargin)}>
                {contentTitle}
            </h1>
            <div className={css(Shared.Styles.bodyText)}>
                {process(description)}
                <div
                    className={css([
                        A('font-family', 'Open sans, sans-serif'),
                        A('font-size', '16px'),
                        A('display', 'flex'),
                        A('flex-direction', 'row'),
                    ])}
                >
                    {status && statusSymbol[status] && (
                        <span
                            title={status}
                            className={css([A('padding-right', '8px')])}
                        >
                            {statusSymbol[status]}
                        </span>
                    )}
                    {status ? <span className={css([])}>{status}</span> : ''}

                    {tags?.length
                        ? Shared.spacer(8) + '·' + Shared.spacer(8)
                        : ''}
                    {tags
                        ?.map((tag) => (
                            <a
                                className={css([A('text-decoration', 'none')])}
                                href={'/projects/tags/' + tag + '/'}
                            >
                                {' '}
                                {tag}{' '}
                            </a>
                        ))
                        .join(',' + Shared.vspace(4))}

                    <div style="flex: 1" />
                    {github ? (
                        <a href={github} target="_blank">
                            {github}
                        </a>
                    ) : (
                        ''
                    )}
                </div>
            </div>
            <div className={css(Shared.Styles.bodyText)}>
                {screenshot ? (
                    <img
                        src={screenshot}
                        alt={contentTitle + ' screenshot'}
                        className={css([A('width', '100%')])}
                    />
                ) : (
                    ''
                )}
                {longDescription ? (
                    <div style="padding: 16px 0">
                        {process(longDescription)}
                    </div>
                ) : (
                    ''
                )}
                <div style="height: 32px" />
                {updates
                    .map((update) => renderUpdate({ css, update }))
                    .join('\n')}
            </div>
        </BodyWithSmallAboutMeColumn>
    );

    return (
        <html>
            <PageHead title={contentTitle} description={description}>
                <style>{inlineCss()}</style>
            </PageHead>
            {body}
        </html>
    );
};

let fullGithub = (text: string) =>
    text.match('^[^/]+$') ? 'https://github.com/jaredly/' + text : text;

export let splitTags = (text: string) => text.split(/,/g).map((t) => t.trim());

export let parseProject = (
    fileName: string,
    {
        strings: { title, description, screenshot, status, github, link, tags },
        bools: { wip },
    }: doc,
    rawBody: string,
): project => {
    // let opts = opts |! "No options for static file " + fileName;
    // let title = Toml.string("title", opts) |! "No title for static page " + fileName;
    // let description = Toml.string("description", opts) |! "No description for static page " + fileName;
    // let screenshot = Toml.string("screenshot", opts);
    // let status = Toml.string("status", opts);
    let parsedTags = tags ? splitTags(tags) : [];
    // let github = Toml.string("github", opts) |?>> fullGithub;
    // let link = Toml.string("link", opts);
    // let tags = Toml.string("tags", opts) |?>> splitTags |? [];
    // let wip = Toml.bool("wip", opts) |? false;

    let longDescription;
    [longDescription, rawBody] = splitFirst('\n===\n', rawBody);

    let updates = rawBody
        .split('\n---\n')
        .map((update) => {
            let [opts, content] = splitFirst('\n\n', update);
            if (!opts) throw new Error('no opts');
            const config = parseToml(opts);

            let date = parseDate(config.strings.date);
            let screenshot: string = config.strings.screenshot;

            return [date, screenshot, content] as [
                Shared.triple,
                string,
                string,
            ];
        })
        .sort(([date], [date2]) => Shared.dateSort(date2, date));
    return {
        title,
        id: chopSuffix(path.basename(fileName)),
        github: github ? fullGithub(github) : undefined,
        link,
        status,
        tags: parsedTags,
        fileName,
        description,
        longDescription,
        screenshot,
        wip,
        updates,
    };
};

/* let render = (fileName, opts, rawBody) => {
  (render(config), config)
}; */

export let renderList = ({
    tags,
    projects,
    contentTitle,
}: {
    tags: [string, number][];
    projects: project[];
    contentTitle: string;
}) => {
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
                        .map(([tag, count]) => {
                            return (
                                <a
                                    href={'/projects/tags/' + tag + '/'}
                                    className={css([
                                        A('color', 'currentColor'),
                                        A('white-space', 'nowrap'),
                                        ...Shared.Styles.hoverUnderline,
                                    ])}
                                >
                                    {tag} {count}
                                </a>
                            );
                        })
                        .join(
                            "\n<span style='display: inline-block; width: 8px'></span>\n",
                        )}
                </div>
            }
        >
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
            </div>
            {Shared.hspace(32)}
            <div
                className={css([
                    A('justify-content', 'center'),
                    A('font-size', '20px'),
                    ...Shared.Styles.row,
                ])}
            >
                α - alpha
                {Shared.vspace(16)}β - beta
                {Shared.vspace(16)}
                🚀 - done
                {Shared.vspace(16)}
                🛌 - retired
            </div>
            {Shared.hspace(64)}
            {projects
                .map(
                    ({
                        title,
                        tags,
                        status,
                        github,
                        description,
                        longDescription,
                        screenshot,
                        wip,
                        fileName,
                        updates,
                    }) => {
                        let href = '/' + chopSuffix(fileName) + '/';
                        let numUpdates = updates.length;
                        return (
                            <div>
                                <div
                                    className={css([
                                        A('align-items', 'flex-end'),
                                        ...Shared.Styles.row,
                                    ])}
                                >
                                    <a
                                        href={href}
                                        className={css([
                                            A('color', 'currentColor'),
                                            A('text-decoration', 'none'),
                                        ])}
                                    >
                                        <h2
                                            className={css([
                                                A('margin', '0'),
                                                A('padding', '0'),
                                            ])}
                                        >
                                            {title}
                                            {status && statusSymbol[status] ? (
                                                <span
                                                    title={status}
                                                    className={css([
                                                        A(
                                                            'padding-left',
                                                            '8px',
                                                        ),
                                                        A('font-size', '24px'),
                                                        A(
                                                            'color',
                                                            Shared.Colors.red,
                                                        ),
                                                        /* ...statusText */
                                                    ])}
                                                >
                                                    {statusSymbol[status]}
                                                </span>
                                            ) : (
                                                ''
                                            )}
                                        </h2>
                                    </a>
                                    <div style="flex: 1" />
                                    {github
                                        ? githubLink({ css, href: github })
                                        : ''}
                                </div>
                                {Shared.hspace(8)}
                                <div className={css(metaText)}>
                                    {updateText({ num: false, updates })}
                                    {tags?.length
                                        ? Shared.spacer(8) +
                                          '·' +
                                          Shared.spacer(8)
                                        : ''}
                                    {tags
                                        ?.map((tag) => (
                                            <a
                                                className={css([
                                                    A(
                                                        'text-decoration',
                                                        'none',
                                                    ),
                                                ])}
                                                href={
                                                    '/projects/tags/' +
                                                    tag +
                                                    '/'
                                                }
                                            >
                                                {' '}
                                                {tag}{' '}
                                            </a>
                                        ))
                                        .join(',' + Shared.vspace(4))}
                                </div>
                                {Shared.hspace(16)}
                                {screenshot ? (
                                    <a
                                        href={href}
                                        className={css([
                                            A('color', 'currentColor'),
                                            A('text-decoration', 'none'),
                                        ])}
                                    >
                                        <img
                                            src={screenshot}
                                            alt="Screenshot"
                                            className={css([
                                                A('margin-bottom', '16px'),
                                                A('max-height', '300px'),
                                                ...Shared.Styles.basicImage,
                                            ])}
                                        />
                                    </a>
                                ) : (
                                    ''
                                )}
                                <div
                                    className={css([
                                        Sub('p', [['padding-bottom', '0']]),
                                        ...Shared.Styles.bodyText,
                                    ])}
                                >
                                    {process(description)}
                                    {/* (Omd.to_html(Omd.of_string(description))) */}
                                </div>
                                {longDescription ? (
                                    <div
                                        className={css([
                                            A('padding-top', '16px'),
                                            Sub('p', [['padding-bottom', '0']]),
                                            ...Shared.Styles.bodyText,
                                        ])}
                                    >
                                        {process(longDescription)}
                                    </div>
                                ) : (
                                    ''
                                )}
                                {Shared.hspace(16)}
                                <a
                                    className={css([A('font-size', '24px')])}
                                    href={href}
                                >
                                    {numUpdates}{' '}
                                    {numUpdates == 1 ? 'Update' : 'Updates'}
                                </a>
                            </div>
                        );
                    },
                )
                .join(
                    '\n' +
                    (
                        <fragment>
                            {Shared.hspace(40)}
                            <div className={css(Shared.Styles.pinkDivider)} />
                            {Shared.hspace(40)}
                        </fragment>
                    ),
                )}
        </BodyWithSmallAboutMeColumn>
    );

    return (
        <html>
            <PageHead title={contentTitle} description="Things Jared has made">
                <style>{inlineCss()}</style>
            </PageHead>
            {body}
        </html>
    );
};
