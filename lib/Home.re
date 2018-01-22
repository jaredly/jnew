
let pageHead = Shared.pageHead;
let (|?>) = (x, f) => switch x { | None => None | Some(x) => f(x) };

let px = n => string_of_int(n) ++ "px";

let module Consts = {
  let colPadding = 32;
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
    let oneCol = 675;
    let twoCol = 1015;
    let threeCol = 1340;
  };
};


let render = (~projects, ~posts, ~tags, ~talks) => {
  open Html;
  open Css;
  let (css, inlineCss) = Css.startPage();
  open Html;
  open Css;

  let colPaddingRules = [
    A("padding", px(Consts.colPadding)),
    Media("max-width: " ++ px(Consts.Media.oneCol), [
      ("padding", px(Consts.colPadding / 2))
    ])
  ];

  let column = [
    A("flex", "1"),
    A("min-width", "300px"),
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

  let subtleLink = [
    A("text-decoration", "none"),
    A("color", "currentColor"),
    Hover([("text-decoration", "underline")]),
  ];

  let module Header = {
    open Html;
    open Css;
    let createElement = (~css, ~href, ~title, ~children, ()) =>
      <a href className=css([
        A("text-decoration", "none"),
        A("color", Shared.Colors.green),
        Hover([("text-decoration", "underline")]),
      ])>
        <h1 className=css([
          A("margin-top", "0"),
          A("font-size", px(Consts.jaredSize))
        ])>title</h1>
      </a>
    ;
  };

  let body = <body className=css([
    A("font-family", "Linux Libertine"),
    A("color", Shared.Colors.text),
    A("margin", "0"),
    A("padding", "0")
  ]) lang="en">
    <div className=css([
      A("flex-direction", "row"),
      A("padding", px(Consts.colPadding / 2)),
      A("display", "flex"),
      A("justify-content", "center"),
      A("align-items", "stretch"),
      A("flex-wrap", "wrap"),
      Media("max-width: " ++ px(Consts.Media.oneCol), [
        ("padding", px(Consts.colPadding / 4))
      ])
    ])>
      <div className=css([Media("max-width: " ++ px(Consts.Media.oneCol), [("order", "1")]), ...column])>
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
      </div>

      <div className=css([Media("max-width: " ++ px(Consts.Media.twoCol), [("order", "1")]),
        A("max-width", "500px"), ...column])>
        <Header href="/projects/" css title="Projects" />
        <div>
          (List.map(
            ({Project.title, fileName, description, screenshot, github, updates, status}) => {
              let href = ("/" ++ Filename.chop_extension(fileName) ++ "/");
              <div>
                <div
                  className=css([A("font-size", px(Consts.titleSize))])
                >
                  <a href className=css(subtleLink)>title</a>
                  (switch (status, status |?> Project.status) {
                  | (Some(status), Some(text)) => <span title=status
                    className=css(statusText)
                    >text</span>
                  | _ => ""
                  })
                  (switch github {
                  | None => ""
                  | Some(href) => <a target="_blank" href className=css([
                      A("font-size", px(Consts.githubSize))
                    ])>"github"</a>
                  })
                </div>
                <a
                  href
                  className=css([
                    A("display", "block"),
                    ...subtleLink
                  ])
                >
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
                    A("font-size", px(Consts.updatesSize))
                  ])>
                    (Project.updateText(updates))
                  </div>
                </a>
              </div>
            },
            projects
          ) |> String.concat("\n" ++ Shared.hspace(Consts.bigSpace)))
        </div>
      </div>


      <div className=css([A("display", "flex"), A("flex-direction", "column"), ...column] @ [
        A("padding", "0"),
        Media("min-width: " ++ px(Consts.Media.threeCol + 1), [("flex", "2"), ("flex-direction", "row"), ("min-width", "600px")])
      ])>
        <div className=css([Media("max-width: " ++ px(Consts.Media.threeCol), [("order", "1")]), A("flex", "1"), ...colPaddingRules])>
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
                    | Some(href) => <a href>where "📹"</a>
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
        </div>

        <div className=css([
          Media("min-width: " ++ px(Consts.Media.threeCol + 1), [("flex", "1")]),
          A("text-align", "center"),
          ...colPaddingRules
        ])>
          (Shared.myBigFaceStatic(css))
          (Shared.hspace(Consts.bigSpace))
          <div className=css([A("font-size", px(Consts.jaredSize))])>
            "I'm Jared Forsyth"
          </div>
          (Shared.hspace(Consts.bigSpace))
          <div className=css([A("font-size", px(Consts.titleSize)), A("line-height", "36px"), A("text-align", "left"),
          ])>
            <p>
            "I’m an idealist with strong opinions, but I'm happy to learn where I'm wrong. We all need more empathy & compassion. The most important aspect of my life is my connection to God."
            </p>
            <p>
            "I’m currently a mobile & web developer for Khan Academy, where we’re working to bring a free, world-class education to anyone anywhere. If you want to help, send me an email!"
            </p>
          </div>
        </div>
      </div>
    </div>
  </body>;

  <html>
    <pageHead title="Jared Forsyth.com" description="My website">
      <style>(inlineCss())</style>
    </pageHead>
    body
  </html>
};
