
let pageHead = Shared.pageHead;
let (|?>) = (x, f) => switch x { | None => None | Some(x) => f(x) };

let monthDate = ((year, month, _)) => string_of_int(year) ++ " " ++ Shared.monthName(month);

let timeSpanMonths = ((year1, month1, _), (year2, month2, _)) => {
  if (year1 == year2) {
    if (month1 == month2) {
      monthDate((year1, month1, 0))
    } else {
      string_of_int(year1) ++ " " ++ Shared.monthName(month1) ++ " - " ++ Shared.monthName(month2)
    }
  } else {
    monthDate((year1, month1, 0)) ++ " - " ++ monthDate((year2, month2, 0))
  }
};

let updateText = updates => switch updates {
| [] => ""
| [(date, screenshot, text)] => "1 update " ++ (monthDate(date))
| [(date, _, _), ...rest] => {
  let total = List.length(rest) + 1;
  let (date2, _, _) = List.nth(rest, total - 2);
  string_of_int(total) ++ " updates, " ++ timeSpanMonths(date2, date)
}
};

let render = (~projects, ~posts, ~tags, ~talks) => {
  open Html;
  open Css;
  let (css, inlineCss) = Css.startPage();
  open Html;
  open Css;

  let column = [A("flex", "1"), A("padding", "24px"), A("min-width", "300px"),
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
        A("color", "#147429"),
        Hover([("text-decoration", "underline")]),
      ])>
        <h1 className=css([A("margin-top", "0")])>title</h1>
      </a>
    ;
  };

  let body = <body className=css([
    A("font-family", "Linux Libertine"),
    A("color", Shared.Colors.text),
  ]) lang="en">
    <div className=css([
      A("flex-direction", "row"),
      A("display", "flex"),
      A("justify-content", "center"),
      A("align-items", "stretch"),
      A("flex-wrap", "wrap"),
    ])>
      <div className=css([Media("max-width: 675px", [("order", "1")]), ...column])>
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
                    A("font-size", "26px"),
                  ])
                >
                  (config.title)
                </div>
                (Shared.hspace(8))
                <div className=css([
                  A("display", "flex"),
                  A("color", Shared.Colors.lightText),
                  A("font-family", "Open sans, sans-serif"),
                  A("font-size", "14px"),
                  A("flex-direction", "row")])>
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
          ) |> String.concat("\n" ++ Shared.hspace(32)))
        </div>
      </div>

      <div className=css([Media("max-width: 1015px", [("order", "1")]),
        A("max-width", "500px"), ...column])>
        <Header href="/projects/" css title="Projects" />
        <div>
          (List.map(
            ({Project.title, fileName, description, screenshot, github, updates, status}) => {
              let href = ("/" ++ Filename.chop_extension(fileName) ++ "/");
              <div>
                <div
                  className=css([A("font-size", "26px")])
                >
                  <a href className=css(subtleLink)>title</a>
                  (switch (status, status |?> Project.status) {
                  | (Some(status), Some(text)) => <span title=status
                    className=css([
                      A("font-size", "20px"),
                      A("color", Shared.Colors.red)
                    ])
                    >text</span>
                  | _ => ""
                  })
                  (switch github {
                  | None => ""
                  | Some(href) => <a target="_blank" href className=css([
                      A("font-size", "16px")
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
                      A("margin-top", "8px"),
                      A("object-fit", "cover"),
                      A("max-height", "200px"),
                      A("box-shadow", "0 0 5px #aaa")
                    ]) />
                  })
                  <div className=css([A("padding-top", "8px"), Sub("p", [("padding-bottom", "8px")])])>
                    (MarkdownParser.parse(description))
                  </div>
                  <div className=css([A("color", Shared.Colors.lightText), A("font-family", "Open sans"), A("font-size", "14px")])>
                    (updateText(updates))
                  </div>
                </a>
              </div>
            },
            projects
          ) |> String.concat("\n" ++ Shared.hspace(16)))
        </div>
      </div>


      <div className=css([A("display", "flex"), A("flex-direction", "column"), ...column] @ [
        A("padding", "0"),
        Media("min-width: 1341px", [("flex", "2"), ("flex-direction", "row"), ("min-width", "600px")])
      ])>
        <div className=css([Media("max-width: 1340px", [("order", "1")]), A("flex", "1"), A("padding", "24px")])>
          <Header href="/" css title="Talks" />
          (List.map(
            ({Talk.title, image, slides, venues}) => {
              let main = switch image {
                | None =>
                  <div
                    className=css([A("font-size", "20px")])
                  >
                  title
                  </div>
                | Some(src) =>
                  <img alt=title src className=css([
                    A("max-width", "min(400px, 100%)"),
                    A("max-height", "300px"),
                    /* A("margin", "8px"), */
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
                <ul>
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
          ) |> String.concat("\n" ++ Shared.hspace(16)))
        </div>

        <div className=css([
          A("padding", "24px"),
          Media("min-width: 1341px", [("flex", "1")]), A("text-align", "center")])>
          (Shared.myBigFaceStatic(css))
          (Shared.hspace(32))
          <div className=css([A("font-size", "32px")])>
            "I'm Jared Forsyth"
          </div>
          (Shared.hspace(32))
          <div className=css([A("font-size", "24px"), A("line-height", "36px"), A("text-align", "left"),
          /* A("text-indent", "1.5em") */
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
