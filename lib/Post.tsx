import { nmNode, parseNotableMind, wordCount } from './NotableMind';
import { triple } from './Shared';
import { doc } from './Toml';
import { config } from './Types';

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

// let postAbout =
//     (~draft=false, ~css, ~date, ~tags, ~withPic=true, ~children, ()) => {
//   Html.(
//     Css.(
//       <div
//         className={css([
//           A("color", Shared.Colors.lightText),
//           A("font-family", "Open sans"),
//           A("font-size", "14px"),
//           A("display", "flex"),
//           A("flex-direction", "row"),
//           A("align-items", "center"),
//           A("justify-content", "flex-start"),
//         ])}>
//         <showDate date />
//         {spacer(8)}
//         {tags |> List.length > 0 ? " · " : ""}
//         {spacer(8)}
//         {String.concat(
//            ", " ++ spacer(4),
//            List.map(
//              tag =>
//                <a
//                  href={"/tags/" ++ tag ++ "/"}
//                  className={css([A("text-decoration", "none")])}>
//                  tag
//                </a>,
//              tags,
//            ),
//          )}
//         {spacer(8)}
//         {draft
//            ? <span
//                style="background-color: red; padding: 4px 8px; display: inline-block; color: white; border-radius: 4px">
//                "draft"
//              </span>
//            : ""}
//       </div>
//     )
//   );
// };

// let pageWithTopAndBottom = Shared.pageWithTopAndBottom;
// let pageHead = Shared.pageHead;

// let renderBody =
//   fun
//   | Markdown(md) => MarkdownParser.process(md)
//   | Html(html) => html
//   | Nm(nodes) => NotableMind.render(nodes);

// let render =
//     (
//       posts,
//       {
//         config: {
//           Types.title: contentTitle,
//           fileName,
//           description,
//           date,
//           tags,
//           thumbnail,
//           article_image,
//           draft,
//         },
//         body: postBody,
//       },
//     ) => {
//   let (css, inlineCss) = Css.startPage();
//   open Html;
//   open Css;
//   // let isMarkdown = Filename.check_suffix(fileName, ".md");

//   let main = AboutMe.bodyWithSmallAboutMeColumn;
//   let body =
//     <main
//       css
//       toc={
//         <div
//           className={css([
//             A("padding", "0 16px"),
//             A("flex-shrink", "1"),
//             A("overflow", "auto"),
//           ])}>
//           <div className={css([])}> "Recent posts" </div>
//           {Shared.hspace(8)}
//           {List.mapi(
//              (
//                i,
//                {config: {Types.title, date: (year, month, day), fileName}},
//              ) => {
//                let href = "/" ++ Util.chopSuffix(fileName) ++ "/";
//                i < 5
//                  ? <a
//                      href
//                      className={css([
//                        A("font-size", "16px"),
//                        A("margin-bottom", "16px"),
//                        A("display", "block"),
//                        A("line-height", "20px"),
//                        ...Shared.Styles.subtleLink,
//                      ])}>
//                      <div> title </div>
//                      <div
//                        className={css([
//                          A("color", Shared.Colors.lightText),
//                          ...Shared.Styles.row,
//                        ])}>
//                        {string_of_int(year)}
//                        {Shared.monthName(month)}
//                        {string_of_int(day)}
//                      </div>
//                    </a>
//                  : "";
//              },
//              posts,
//            )
//            |> String.concat("\n")}
//           {Shared.hspace(32)}
//         </div>
//       }>
//       <h1 className={css(Shared.Styles.titleWithTopMargin)}>
//         contentTitle
//       </h1>
//       <postAbout draft css date tags />
//       {Shared.hspace(32)}
//       <div className={"post-body " ++ css(Shared.Styles.bodyText)}>
//         {renderBody(postBody)}
//       </div>
//     </main>;

//   <html>
//     <pageHead title=contentTitle ?description ?thumbnail ?article_image>
//       <style> {inlineCss()} </style>
//     </pageHead>
//     body
//   </html>;
// };

// let rss = (~title, ~urlBase, posts, tags) => {
//   Rss.wrapper(
//     ~siteTitle=title,
//     ~posts=
//       posts
//       |> List.map(({config, intro, body}) => {
//            let href =
//              urlBase ++ "/" ++ Util.chopSuffix(config.Types.fileName) ++ "/";
//            let readTime = Shared.minuteReadText(config.wordCount);
//            let description =
//              (
//                switch (intro) {
//                | None => ""
//                | Some(intro) => renderBody(intro) ++ " "
//                }
//              )
//              ++ readTime
//              ++ (config.draft ? " [DRAFT]" : "");
//            {
//              Rss.title: config.title,
//              description,
//              url: href,
//              content: renderBody(body),
//              //  descriptionHtml: description |> Rss.escapeContent,
//              date: config.date,
//              category: None,
//            };
//          }),
//   );
// };

// let postList = (~urlBase, posts, tags, contentTitle) => {
//   open Html;
//   let (css, inlineCss) = Css.startPage();
//   /* let contentTitle = "All posts"; */
//   let main = AboutMe.bodyWithSmallAboutMeColumn;

//   let body =
//     <main
//       css
//       toc={
//         <div
//           className={css([
//             A("font-size", "16px"),
//             A("line-height", "20px"),
//             A("margin-bottom", "32px"),
//           ])}>
//           {List.map(
//              ((tag, count)) =>
//                Types.(
//                  <a
//                    href={"/tags/" ++ tag ++ "/"}
//                    className={css([
//                      A("color", "currentColor"),
//                      A("white-space", "nowrap"),
//                      A("margin-right", "8px"),
//                      ...Shared.Styles.hoverUnderline,
//                    ])}>
//                    tag
//                    {"" ++ string_of_int(count) ++ ""}
//                  </a>
//                ),
//              /* <postAbout css date=config.date tags=config.tags withPic=false />
//                 <a className=css([A("font-size", "24px")]) href>readTime</a> */
//              tags,
//            )
//            |> String.concat("\n")}
//         </div>
//       }>
//       <div className={css([A("flex", "3"), A("padding", "32px")])}>
//         <div
//           className={css([A("padding", "1px"), A("position", "relative")])}>
//           <h1
//             className={css([
//               A("text-align", "center"),
//               ...Shared.Styles.title,
//             ])}>
//             contentTitle
//           </h1>
//           <a href="rss.xml"> "RSS Feed" </a>
//         </div>
//         {List.map(
//            ({config, intro: teaser}) => {
//              open Types;
//              let href = "/" ++ Util.chopSuffix(config.Types.fileName) ++ "/";
//              let readTime = Shared.minuteReadText(config.wordCount);
//              <div>
//                <a
//                  href
//                  className={css([
//                    A("color", "currentColor"),
//                    A("text-decoration", "none"),
//                  ])}>
//                  <h2> {config.title} </h2>
//                </a>
//                <postAbout
//                  css
//                  date={config.date}
//                  tags={config.tags}
//                  withPic=false
//                />
//                {switch (teaser) {
//                 | None => ""
//                 | Some(teaser) =>
//                   <div
//                     className={css([
//                       A("padding-top", "16px"),
//                       ...Shared.Styles.bodyText,
//                     ])}>
//                     {renderBody(teaser)}
//                   </div>
//                 // (Omd.to_html(Omd.of_string(teaser)))
//                 }}
//                <a className={css([A("font-size", "24px")])} href>
//                  readTime
//                </a>
//              </div>;
//            },
//            posts,
//          )
//          |> String.concat("\n<div style='height: 32px'></div>\n")}
//       </div>
//     </main>;

//   let main =
//     <html>
//       <pageHead
//         title=contentTitle description="Things Jared has written about">
//         <style> {inlineCss()} </style>
//       </pageHead>
//       body
//     </html>;
//   let rss =
//     rss(~title=contentTitle ++ " | JaredForsyth.com", ~urlBase, posts, tags);
//   (main, rss);
// };

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
        strings: { title, description, date, thumbnail, article_image },
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
    // let config =
    //   check(Get.string("title", json), config, title => {...config, title});
    // let config =
    //   check(Get.string("description", json), config, description =>
    //     {...config, description: Some(description)}
    //   );
    // let config =
    //   check(Get.stringList("tags", json), config, tags => {...config, tags});
    // let config =
    //   check(Get.stringList("categories", json), config, categories =>
    //     {...config, categories}
    //   );
    // let config =
    //   check(Get.string("date", json), config, date =>
    //     {...config, date: parseDate(date)}
    //   );
    // let config =
    //   check(Get.bool("featured", json), config, featured =>
    //     {...config, featured}
    //   );
    // let config =
    //   check(Get.string("thumbnail", json), config, thumbnail =>
    //     {...config, thumbnail: Some(thumbnail)}
    //   );
    // let config =
    //   check(Get.string("article_image", json), config, article_image =>
    //     {...config, article_image: Some(article_image)}
    //   );
    // let config =
    //   check(Get.bool("draft", json), config, draft => {...config, draft});
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
    }
    console.log(nodes);
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
