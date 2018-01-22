
let pageHead = Shared.pageHead;
let (|?>) = (x, f) => switch x { | None => None | Some(x) => f(x) };

let px = n => string_of_int(n) ++ "px";

let module Consts = {
  let colPadding = 40;
  let colPaddingSmall = 16;
  let jaredSize = 36;
  let titleSize = 24;
  let statusSize = 20;
  let githubSize = 16;
  let updatesSize = 14;
  let smallSpace = 8;
  let medSpace = 16;
  let bigSpace = 32;
  let bigSpace = 40;

  let module Media = {
    let oneCol = 835;
    let twoCol = 1239;
    let threeCol = 1400;
  };
};

let aboutMeColumn = css => Html.(
  <fragment>
    (Shared.myBigFaceStatic(css))
    (Shared.hspace(Consts.bigSpace))
    <div className=css([A("text-align", "center"), A("font-size", px(Consts.jaredSize))])>
      "I'm Jared Forsyth"
    </div>
    (Shared.hspace(Consts.bigSpace))
    <div className=css([A("font-size", px(Consts.titleSize)), A("line-height", "36px"), A("text-align", "left"),
    ])>
      <p>
      "I’m an idealist with strong opinions, but I'm happy to learn where I'm wrong. We all need more empathy & compassion. The most important aspect of my life is my connection to God."
      </p>
      <p>
      "I’m currently a mobile & web developer for Khan Academy, where we’re working to bring a free, world-class education to anyone anywhere. If you want to help, get in touch!"
      </p>
      <div style="text-align:center" className=css([
        Sub("a", [(
          ("text-decoration", "none"),
        )]),
        Sub("a:hover", [(
          ("text-decoration", "underline"),
        )]),
        A("font-family", "Open sans, sans-serif"),
        A("font-size", "20px")
      ])>
      <a href="https://twitter.com/jaredforsyth">"twitter/@jaredforsyth"</a>
      "<br/>"
      <a href="https://github.com/jaredly">"github/@jaredly"</a>
      </div>
    </div>
  </fragment>
);

let module Styles = {
  open Css;

  let colPaddingRules = [
    A("padding", px(Consts.colPadding)),
    Media("max-width: " ++ px(Consts.Media.oneCol), [
      ("padding", px(Consts.colPaddingSmall))
    ])
  ];

  let column = [
    A("flex", "1"),
    A("min-width", "400px"),
    Media("max-width: " ++ px(Consts.Media.oneCol), [
      ("min-width", "300px"),
    ]),
    ...colPaddingRules
  ];

  let statusText = [
    A("font-size", px(Consts.statusSize)),
    A("color", Shared.Colors.red)
  ];

  let metaText = [
    A("font-family", "Open sans, sans-serif"),
    A("color", Shared.Colors.lightText),
    A("font-size", px(Consts.updatesSize)),
  ];

  let hoverUnderline = [
    A("text-decoration", "none"),
    Hover([("text-decoration", "underline")]),
  ];

  let subtleLink = [
    A("color", "currentColor"),
    ...hoverUnderline
  ];
};

let module Header = {
  open Html;
  open Css;
  let createElement = (~css, ~href, ~title, ~children, ()) =>
    <a href className=css([
      A("color", Shared.Colors.darkGreen),
      ...Styles.hoverUnderline
    ])>
      <h1 className=css([
        A("margin-top", "0"),
        A("font-size", px(Consts.jaredSize))
      ])>title</h1>
    </a>
  ;
};



let projectColumn = (css, projects) => {
  open Css;
  open Html;
  <fragment>
    <Header href="/projects/" css title="Projects" />
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
                  A("padding-left", px(Consts.smallSpace)),
                  ...Styles.statusText
                ])
                >text</span>
              | _ => ""
              })
              </a>
              /* <div style="flex: 1"/> */
              (switch github {
              | None => ""
              | Some(href) => Project.githubLink(css, href)
              /* <a target="_blank" href className=css([
                  A("font-size", px(Consts.githubSize))
                ])>"github"</a> */
              })
            </div>
            /* <a
              href
              className=css([
                A("display", "block"),
                ...hiddenLink
              ])
            > */
              (switch screenshot {
              | None => ""
              | Some(src) => <img src alt=(title ++ " screenshot") className=css([
                  A("width", "100%"),
                  A("margin-top", px(Consts.smallSpace)),
                  A("object-fit", "cover"),
                  A("max-height", "200px"),
                  A("box-shadow", "0 0 5px #aaa")
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
  <Header href="/" css title="Talks" />
  (List.map(
    ({Talk.title, image, slides, venues}) => {
      let main = switch image {
        | None =>
          <div
            className=css([A("font-size", px(Consts.statusSize))])
          >
          title
          </div>
        | Some(src) =>
          <img alt=title src className=css([
            A("max-width", "min(400px, 100%)"),
            A("max-height", "300px"),
            A("box-shadow", "0 0 5px #aaa"),
          ])/>
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
        ((config, intro, body)) => {
          open Types;
          let href = ("/" ++ Filename.chop_extension(config.fileName) ++ "/");
          let (year, month, day) = config.date;
          <a href className=css(subtleLink)>
            <div
              className=css([
                A("font-size", px(Consts.titleSize)),
              ])
            >
              (config.title)
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
              /* (Shared.showDate(~date=config.date, ~children=[], ())) */
              <div style="flex: 1"/>
              (Shared.minuteReadText(config.wordCount))
            </div>
          </a>
        },
        posts
      ) |> String.concat("\n" ++ Shared.hspace(Consts.bigSpace)))
    </div>
  </fragment>
};

let render = (~projects, ~posts, ~tags, ~talks) => {
  open Html;
  open Css;
  let (css, inlineCss) = Css.startPage();

  open Styles;

  let body = <body className=css([
    A("font-family", "Linux Libertine"),
    A("color", Shared.Colors.text),
    A("margin", "0"),
    A("padding", "0")
  ]) lang="en">
    <div className=css([
      A("flex-direction", "row"),
      A("padding", px(Consts.colPadding / 2)),
      A("padding-top", "0"),
      A("display", "flex"),
      A("justify-content", "center"),
      A("align-items", "stretch"),
      A("flex-wrap", "wrap"),
      Media("max-width: " ++ px(Consts.Media.oneCol), [
        ("padding", px(Consts.colPaddingSmall / 2))
      ])
    ])>

      /* <div className=css([Media("max-width: " ++ px(Consts.Media.oneCol), [("order", "1")]), ...column])> */
      <div className=css([A("background-color", Shared.Colors.lightOrange), ...column])>
        (aboutMeColumn(css))
      </div>

      /* <div className=css([Media("max-width: " ++ px(Consts.Media.oneCol), [("order", "1")]), ...column])> */
      <div className=css(column)>
        (blogColumn(css, posts))
      </div>

      <div className=css(column)>
        (talksColumn(css, talks))
      </div>

      /* <div className=css([Media("max-width: " ++ px(Consts.Media.twoCol), [("order", "1")]),
        A("max-width", "500px"), ...column])> */
      <div className=css([A("max-width", "700px"), ...column])>
        (projectColumn(css, projects))
      </div>

      /* <div className=css([A("display", "flex"), A("flex-direction", "column"), ...column] @ [
        A("padding", "0"),
        Media("min-width: " ++ px(Consts.Media.threeCol + 1), [("flex", "2"), ("flex-direction", "row"), ("min-width", "600px")])
      ])>
        <div className=css([Media("max-width: " ++ px(Consts.Media.threeCol), [("order", "1")]), A("flex", "1"), ...colPaddingRules])>
            (talksColumn(css, talks))
        </div>

        <div className=css([
          Media("min-width: " ++ px(Consts.Media.threeCol + 1), [("flex", "1")]),
          A("text-align", "center"),
          ...colPaddingRules
        ])>
            (aboutMeColumn(css))
        </div>
      </div> */

    </div>
  </body>;

  <html>
    <pageHead title="Jared Forsyth.com" description="My website">
      <style>(inlineCss())</style>
    </pageHead>
    body
  </html>
};
