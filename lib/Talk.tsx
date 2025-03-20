/** @jsx jsx */
import { A, CssFn, Hover, jsx, Media } from './sjsx';
import { BodyWithSmallAboutMeColumn } from './AboutMe';
import { startPage } from './Css';
import { Consts, PageHead, px, Styles, triple } from './Shared';
import * as Shared from './Shared';
import { readFileSync } from 'fs';

export type venue = {
    where: string;
    video?: string;
    date?: triple;
};

export type talk = {
    title: string;
    image?: string;
    slides?: string;
    tweets: string[];
    venues: venue[];
};

// let jsonString = item => switch item {
// | String(v) => Some(v)
// | _ => None
// };

// let jsonArray = item => switch item {
// | `List(v) => Some(v)
// | _ => None
// };

// let parseVenue = item => switch item {
// | Assoc(items) => {
//   let where = get("where", items) |?> jsonString |! "Where is required for a venue";
//   let date = get("date", items) |?> jsonString |?>> Post.parseDate;
//   let video = get("video", items) |?> jsonString;
//   {where, date, video}
// }
// | _ => failwith("Bad venue json")
// };

export let collectTalks = (fname: string) => {
    const text = readFileSync(fname, 'utf-8');
    const talks = JSON.parse(text);
    return talks;
};

export let renderTalkList = (talks: talk[], contentTitle: string) => {
    let { css, inlineCss } = startPage();
    let body = (
        <BodyWithSmallAboutMeColumn
            css={css}
            toc={
                <div
                    className={css([
                        /* A("font-size", "16px"),
        A("line-height", "20px"), */
                        A('margin-bottom', '32px'),
                    ])}
                >
                    If you want to speak at conferences, contact me on twitter
                    and I'd be happy to give you guidance on applying 👋
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
            {Shared.hspace(64)}
            {talks
                .map(({ title, image, slides, venues }) => {
                    let main = image ? (
                        <img
                            alt={title}
                            src={image}
                            className={css([
                                A('max-height', '300px'),
                                ...Shared.Styles.basicImage,
                                A('object-fit', 'contain'),
                                A('background', 'white')
                            ])}
                        />
                    ) : (
                        <div
                            className={css([
                                A(
                                    'font-size',
                                    Shared.px(Shared.Consts.jaredSize),
                                ),
                            ])}
                        >
                            {' '}
                            {title}{' '}
                        </div>
                    );
                    return (
                        <div>
                            {slides ? (
                                <a
                                    href={slides}
                                    target="_blank"
                                    className={css(Shared.Styles.subtleLink)}
                                >
                                    {main}
                                </a>
                            ) : (
                                ''
                            )}
                            {Shared.hspace(Shared.Consts.smallSpace)}
                            <ul
                                className={css([
                                    A('font-size', px(Consts.titleSize)),
                                    A('padding', '0'),
                                    /* A("padding-left", "32px"), */
                                    A('list-style', 'none'),
                                    A('margin', '0'),
                                ])}
                            >
                                {venues
                                    .map(({ where, date, video }) => {
                                        return (
                                            <li>
                                                {video ? (
                                                    <a
                                                        href={video}
                                                        className={css([
                                                            A(
                                                                'font-family',
                                                                'Open sans, sans-serif',
                                                            ),
                                                            A(
                                                                'font-size',
                                                                Shared.px(
                                                                    Consts.titleSize,
                                                                ),
                                                            ),
                                                            ...Styles.hoverUnderline,
                                                        ])}
                                                    >
                                                        {where} 📹
                                                    </a>
                                                ) : (
                                                    where
                                                )}
                                            </li>
                                        );
                                    }, venues)
                                    .join('\n')}
                            </ul>
                        </div>
                    );
                })
                .join(
                    '\n' +
                        Shared.hspace(
                            Shared.Consts.bigSpace + Shared.Consts.smallSpace,
                        ),
                )}
        </BodyWithSmallAboutMeColumn>
    );

    return (
        <html>
            <PageHead title={contentTitle} description="Talks Jared has given">
                <style>{inlineCss()}</style>
            </PageHead>
            {body}
        </html>
    );
};
