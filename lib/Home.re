
let pageHead = Shared.pageHead;

open Shared.Infix;

let module Consts = Shared.Consts;
let px = Shared.px;

let module Styles = Shared.Styles;
let module Header = Shared.Header;

let projectColumn = (css, projects) => {
  open Css;
  open Html;
  <fragment>
    <Header href="/projects/" css title="Projects" />
    <div className=css(Styles.row)>
      "α - alpha"
      (Shared.vspace(16))
      "β - beta"
      (Shared.vspace(16))
      "🚀 - done"
      (Shared.vspace(16))
      "🛌 - retired"
    </div>
    (Shared.hspace(16))
    <div>
      (List.map(
        ({Project.title, fileName, description, screenshot, github, updates, status, tags}) => {
          let href = ("/" ++ Filename.chop_extension(fileName) ++ "/");
          <div>
            <div
              className=css([
                A("font-size", px(Consts.titleSize)),
                A("display", "flex"),
                A("flex-direction", "row"),
                A("align-items", "flex-end"),
              ])
            >
              <a href className=css([A("flex", "1"), ...Styles.subtleLink])>
              title
              (switch (status, status |?> Project.statusSymbol) {
              | (Some(status), Some(text)) => <span title=status
                className=css([
                  A("width", "24px"),
                  A("padding-left", px(Consts.medSpace)),
                  A("display", "inline-block"),
                  ...Styles.statusText
                ])
                >text</span>
              | _ => <span style="width: 24px; display: inline-block"/>
              })
              </a>
              (github |?>> Project.githubLink(css) |? "")
            </div>
              (switch screenshot {
              | None => ""
              | Some(src) => <img src alt=(title ++ " screenshot") className=css([
                  A("margin-top", px(Consts.smallSpace)),
                  A("max-height", "200px"),
                  ...Styles.basicImage
                ]) />
              })
              <div className=css([
                A("padding-top", px(Consts.smallSpace)),
                A("font-size", px(Consts.githubSize)),
                Sub("p", [("padding-bottom", px(Consts.smallSpace))])])>
                (MarkdownParser.parse(description))
              </div>
              <div className=css([
                A("color", Shared.Colors.lightText),
                A("font-family", "Open sans"),
                A("font-size", px(Consts.updatesSize)),
                A("display", "flex"),
                A("flex-direction", "row")
              ])>
                (Project.updateText(updates))
                (tags != [] ? Shared.spacer(8) ++ "·" ++ Shared.spacer(8) : "")
                (tags |> List.map(tag => <a className=css(Styles.hoverUnderline) href=("/projects/tags/" ++ tag ++ "/")> tag </a>) |> String.concat("," ++ Shared.vspace(4)))
              </div>
            /* </a> */
          </div>
        },
        projects
      ) |> String.concat("\n" ++ Shared.hspace(Consts.bigSpace)))
    </div>
  </fragment>
};


let talksColumn = (css, talks) => {
  open Css;
  open Html;
  open Styles;

  <fragment>
  <Header href="/talks/" css title="Talks" />
  (List.map(
    ({Talk.title, image, slides, venues}) => {
      let main = switch image {
        | None =>
          <div className=css([A("font-size", px(Consts.statusSize))])> title </div>
        | Some(src) =>
          <img alt=title src className=css([A("max-height", "300px"), ...Styles.basicImage])/>
      };
      <div>
        (switch slides {
        | None => main
        | Some(href) =>
          <a href target="_blank" className=css(subtleLink)>
            main
          </a>
        })
        (Shared.hspace(Consts.smallSpace))
        <ul className=css([
          A("font-size", px(Consts.githubSize)),
          A("padding", "0"),
          /* A("padding-left", "32px"), */
          A("list-style", "none"),
          A("margin", "0")
        ])>
        (List.map(
          ({Talk.where, date, video}) => {
            <li>
            (switch video {
            | None => where
            | Some(href) => <a href className=css([
              A("font-family", "Open sans, sans-serif"),
              A("font-size", "14px"),
              ...hoverUnderline
              ])>where "📹"</a>
            })
            </li>
          },
          venues
        ) |> String.concat("\n"))
        </ul>
      </div>
    },
    talks
  ) |> String.concat("\n" ++ Shared.hspace(Consts.bigSpace + Consts.smallSpace)))
  </fragment>
};

let blogColumn = (css, posts) => {
  open Styles;
  open Css;
  open Html;

  <fragment>
    <Header href="/posts/" css title="Blog posts" />
    <div>
      (List.map(
        ({Post.config: {Types.fileName, date, wordCount, title}, intro, body}) => {
          let href = ("/" ++ Filename.chop_extension(fileName) ++ "/");
          let (year, month, day) = date;
          <a href className=css(subtleLink)>
            <div className=css([A("font-size", px(Consts.titleSize))])>
              (title)
            </div>
            (Shared.hspace(Consts.smallSpace))
            <div className=css([
              A("display", "flex"),
              A("flex-direction", "row"),
              ...metaText
            ])>
              (string_of_int(year))
              (Shared.monthName(month))
              (string_of_int(day))
              <div style="flex: 1"/>
              (Shared.minuteReadText(wordCount))
            </div>
          </a>
        },
        posts
      ) |> String.concat("\n" ++ Shared.hspace(Consts.bigSpace)))
    </div>
  </fragment>
};

let render = (~projects, ~posts, ~talks) => {
  open Html;
  open Css;
  let (css, inlineCss) = Css.startPage();

  open Styles;

  let main = AboutMe.bodyWithAboutMeColumn;
  let body = <main css>
    <div className=css(column)>
      (blogColumn(css, posts))
    </div>

    <div className=css(column)>
      (talksColumn(css, talks))
    </div>

    <div className=css([A("max-width", "700px"), ...column])>
      (projectColumn(css, projects))
    </div>
  </main>;

  <html>
    <pageHead title="Jared Forsyth.com" description="My website">
      <style>(inlineCss())</style>
    </pageHead>
    body
  </html>
};
