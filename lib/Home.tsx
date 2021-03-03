/** @jsx jsx */
import { A, CssFn, Hover, jsx, Media, Sub } from './sjsx';
import * as Shared from './Shared';
import { githubLink, project, statusSymbol, updateText } from './Project';
import { chopSuffix } from './Util';
import { process } from './MarkdownParser';

let pageHead = Shared.pageHead;

const Consts = Shared.Consts;
let px = Shared.px;

const Styles = Shared.Styles;
const Header = Shared.Header;


let projectColumn = ({css, projects}: {css: CssFn, projects: project[]}) => {
  return (
    (
      <fragment>
        <Header href="/projects/" css={css} title="Projects" />
        <div className={css(Styles.row)}>
          α - alpha
          {Shared.vspace(16)}
          β - beta
          {Shared.vspace(16)}
          🚀 - done
          {Shared.vspace(16)}
          🛌 - retired
        </div>
        {Shared.hspace(16)}
        <div>
          {projects.map(
             (
               {
                 title,
                 fileName,
                 description,
                 screenshot,
                 github,
                 updates,
                 status,
                 tags,
               },
             ) => {
               let href = "/" + chopSuffix(fileName) + "/";
               return <div>
                 <div
                   className={css([
                     A("font-size", px(Consts.titleSize)),
                     A("display", "flex"),
                     A("flex-direction", "row"),
                     A("align-items", "flex-end"),
                   ])}>
                   <a
                     href={href}
                     className={css([A("flex", "1"), ...Styles.subtleLink])}>
                     {title}
                     {status && statusSymbol[status] ? <span
                          title={status}
                          className={css([
                            A("width", "24px"),
                            A("padding-left", px(Consts.medSpace)),
                            A("display", "inline-block"),
                            ...Styles.statusText,
                          ])}>
                          {statusSymbol[status]}
                        </span> : <span style="width: 24px; display: inline-block" />
                      }
                   </a>
                   {github ? githubLink({css, href: github}) : ''}
                 </div>
                 {screenshot ? <img
                      src={screenshot}
                      alt={title + " screenshot"}
                      className={css([
                        A("margin-top", px(Consts.smallSpace)),
                        A("max-height", "200px"),
                        ...Styles.basicImage,
                      ])}
                    />
                  : ''}
                 <div
                   className={css([
                     A("padding-top", px(Consts.smallSpace)),
                     A("font-size", px(Consts.githubSize)),
                     Sub("p", [["padding-bottom", px(Consts.smallSpace)]]),
                   ])}>
                   {process(description)}
                 </div>
                 <div
                   className={css([
                     A("color", Shared.Colors.lightText),
                     A("font-family", "Open sans"),
                     A("font-size", px(Consts.updatesSize)),
                     A("display", "flex"),
                     A("flex-direction", "row"),
                   ])}>
                   {updateText({updates})}
                   {tags?.length
                      ? Shared.spacer(8) + "·" + Shared.spacer(8) : ""}
                   {tags?.map(tag =>
                         <a
                           className={css(Styles.hoverUnderline)}
                           href={"/projects/tags/" + tag + "/"}>
                           tag
                         </a>
                       )
                    .join("," + Shared.vspace(4))}
                 </div>
               </div>;
             },
           ).join("\n" + Shared.hspace(Consts.bigSpace))}
        </div>
      </fragment>
    )
  );
};

let talksColumn = ({css, talks}: {css: CssFn, talks: talk[]}) => {
  (
    (
      Styles.(
        <fragment>
          <Header href="/talks/" css title="Talks" />
          {List.map(
             ({Talk.title, image, slides, venues}) => {
               let main =
                 switch (image) {
                 | None =>
                   <div
                     className={css([
                       A("font-size", px(Consts.statusSize)),
                     ])}>
                     title
                   </div>
                 | Some(src) =>
                   <img
                     alt=title
                     src
                     className={css([
                       A("max-height", "300px"),
                       ...Styles.basicImage,
                     ])}
                   />
                 };
               <div>
                 {switch (slides) {
                  | None => main
                  | Some(href) =>
                    <a href target="_blank" className={css(subtleLink)}>
                      main
                    </a>
                  }}
                 {Shared.hspace(Consts.smallSpace)}
                 <ul
                   className={css([
                     A("font-size", px(Consts.githubSize)),
                     A("padding", "0"),
                     /* A("padding-left", "32px"), */
                     A("list-style", "none"),
                     A("margin", "0"),
                   ])}>
                   {List.map(
                      ({Talk.where, date, video}) => {
                        <li>
                          {switch (video) {
                           | None => where
                           | Some(href) =>
                             <a
                               href
                               className={css([
                                 A("font-family", "Open sans, sans-serif"),
                                 A("font-size", "14px"),
                                 ...hoverUnderline,
                               ])}>
                               where
                               "📹"
                             </a>
                           }}
                        </li>
                      },
                      venues,
                    )
                    |> String.concat("\n")}
                 </ul>
               </div>;
             },
             talks,
           )
           |> String.concat(
                "\n" + Shared.hspace(Consts.bigSpace + Consts.smallSpace),
              )}
        </fragment>
      )
    )
  );
};

let blogColumn = (css, posts) => {
  Styles.(
    Css.(
      Html.(
        <fragment>
          <Header href="/posts/" css title="Blog posts" />
          <div>
            {List.map(
               (
                 {
                   Post.config: {
                     Types.fileName,
                     date,
                     wordCount,
                     title,
                     draft,
                   },
                   intro,
                   body,
                 },
               ) => {
                 let href = "/" + Util.chopSuffix(fileName) + "/";
                 let (year, month, day) = date;
                 <a href className={css(subtleLink)}>
                   <div
                     className={css([A("font-size", px(Consts.titleSize))])}>
                     title
                   </div>
                   {Shared.hspace(Consts.smallSpace)}
                   <div
                     className={css([
                       A("display", "flex"),
                       A("align-items", "center"),
                       A("flex-direction", "row"),
                       ...metaText,
                     ])}>
                     {string_of_int(year)}
                     {Shared.monthName(month)}
                     {string_of_int(day)}
                     {draft
                        ? <span
                            style="background-color: red; margin-left: 8px; padding: 4px 8px; display: inline-block; color: white; border-radius: 4px">
                            "draft"
                          </span>
                        : ""}
                     <div style="flex: 1" />
                     {Shared.minuteReadText(wordCount)}
                   </div>
                 </a>;
               },
               posts,
             )
             |> String.concat("\n" + Shared.hspace(Consts.bigSpace))}
          </div>
        </fragment>
      )
    )
  );
};

let render = (~projects, ~posts, ~talks) => {
  open Html;
  open Css;
  let (css, inlineCss) = Css.startPage();
  open Styles;

  let main = AboutMe.bodyWithAboutMeColumn;
  let body =
    <main css>
      <div className={css(column)}> {blogColumn(css, posts)} </div>
      <div className={css(column)}> {talksColumn(css, talks)} </div>
      <div className={css([A("max-width", "700px"), ...column])}>
        {projectColumn(css, projects)}
      </div>
    </main>;

  <html>
    <pageHead title="Jared Forsyth.com" description="My website">
      <style> {inlineCss()} </style>
    </pageHead>
    body
  </html>;
};