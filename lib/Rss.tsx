/** @jsx jsx */
import { A, CssFn, Hover, jsx, Media, Sub } from './sjsx';
import * as Shared from './Shared';

// let channel = normal("channel", []);
// let title = normal("title", []);
// let author = normal("author", []);
// let description = normal("description", []);

import { triple } from './Shared';

// let summary = normal("summary", []);
let cdata = (text: string) => `<![CDATA[${text}]]>`;

let Rss = ({ children }: { children?: string[] }) => {
    return `<rss
    xmlns:dc="http://purl.org/dc/elements/1.1/"
    xmlns:content="http://purl.org/rss/1.0/modules/content/"
    xmlns:atom="http://www.w3.org/2005/Atom"
    version="2.0">${children?.join('\n')}</rss>`;
};

// let item = normal("item", []);
// let link = normal("link", []);
// let guid = normal("guid", []);
// let category = normal("category", []);
// let pubDate = normal("pubDate", []);
// let content = normal("content", []);

export type rssPost = {
    title: string;
    description: string;
    url: string;
    content: string;
    date: triple;
    category?: string;
};

export let escapeContent = (value: string) => {
    value
        .replaceAll('&', '&amp;')
        .replaceAll('<', '&lt;')
        .replaceAll('>', '&gt;');
    //   |> Str.global_replace(Str.regexp_string("\""), "&quot;")
};

// let force = (x) => {
//     if (!x) throw new Error('needed that');
//     return x;
// };

// let datePrinter =
//   ODate.Unix.To.generate_printer("%d %b %Y %H:%M:%S GMT") |> force;
let makeDate = ([year, month, day]: triple) => new Date(year, month - 1, day);

//   ODate.Unix.make(~day, ~month=ODate.Month.of_int(month), ~year, ());

export let rssWrapper = (siteTitle: string, posts: rssPost[]) => (
    <Rss>
        <channel>
            <title> {cdata(siteTitle)} </title>
            {posts
                .map((post) => {
                    return (
                        <item>
                            <title> {cdata(post.title)} </title>
                            <description>
                                {' '}
                                {cdata(post.description)}{' '}
                            </description>
                            <summary> {cdata(post.description)} </summary>
                            <author> "Jared Forsyth" </author>
                            <link> {post.url} </link>
                            <guid> {post.url} </guid>
                            {post.category ? (
                                <category> {cdata(post.category)} </category>
                            ) : (
                                ''
                            )}
                            <pubDate>
                                {makeDate(post.date).toUTCString()}
                            </pubDate>
                        </item>
                    );
                })
                .join('\n\n')}
        </channel>
    </Rss>
);
