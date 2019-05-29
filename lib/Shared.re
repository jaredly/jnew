
let module Infix = {
  let (|?) = (x, y) => switch x { | None => y | Some(x) => x };
  let (|??) = (x, y) => switch x { | None => y | Some(x) => Some(x) };
  let (|?>) = (x, f) => switch x { | None => None | Some(x) => f(x) };
  let (|?>>) = (x, f) => switch x { | None => None | Some(x) => Some(f(x)) };
  let (|!) = (x, y) => switch x { | None => failwith(y) | Some(x) => x };
};
open Infix;

let module Colors = {
  let text = "#333";
  let lightText = "#999";
  let red = "#692900";
  let lightOrange = "#fff4ef";
  let darkGreen = "#147429";
  let green = "#1fad3e";
};


/* let userPic = (~css, ~children, ()) => Css.(Html.(
  <div className=css([
    A("background-image", "url(https://www.gravatar.com/avatar/313878fc8f316fc3fe4443b13913d0a4.png?s=30)"),
    A("background-size", "cover"),
    A("display", "inline-block"),
    A("width", "30px"),
    A("height", "30px"),
    A("border-radius", "50%")
  ]) />
)); */

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
    let skinnyFourCol = 1650;
  };
};

let twitter = (~contentTitle, ~description, ~article_image, ~thumbnail, ~children, ()) => {
  open Html;
  <fragment>
    <meta name="twitter:card" content=(article_image == None ? "summary" : "summary_large_image")/>
    <meta name="twitter:title" content=contentTitle />
    <meta name="twitter:description" content=description />
    <meta name="twitter:image" content=(article_image |?? thumbnail |? "https://jaredforsyth.com/images/logo/JF_black_128.png")/>
    <meta name="twitter:site" content="https://jaredforsyth.com"/>
    <meta name="twitter:creator" content="@jaredforsyth"/>
  </fragment>
};

let mobileMeta = (~children, ()) => Html.(<fragment>
  <meta name="viewport" content="width=device-width, initial-scale=1.0"/>
  <meta name="HandheldFriendly" content="True" />
  <meta name="MobileOptimized" content="320" />
  <meta name="apple-mobile-web-app-capable" content="yes"/>
  <meta name="apple-mobile-web-app-status-bar-style" content="black-translucent"/>
</fragment>);

let metaHead = (~title as contentTitle, ~description, ~thumbnail=?, ~article_image=?, ~children, ()) => {
  open Html;
  <fragment>
    {Meta.charset("utf8")}

    <meta name="description" content=description />

    <mobileMeta />
    <twitter contentTitle description article_image thumbnail />

    <pmeta property="og:type" content="article"/>
    <pmeta property="og:title" content=contentTitle/>
    <pmeta property="og:description" content=description/>

    <link rel="shortcut icon" href="/images/logo/JF_black_32.png"/>

    <title>{contentTitle}</title>
  </fragment>
};

let pageHead = (~title as contentTitle, ~description=?, ~thumbnail=?, ~article_image=?, ~extraHead="", ~children, ()) => {
  let contentTitle = contentTitle ++ " | Jared Forsyth.com";
  let description = switch (description) {
  | Some(description) => description
  | None => "Thoughts about programming, mostly"
  };
  open Html;
<head>
    <metaHead title=contentTitle description ?thumbnail ?article_image />

    /* TODO audit these */
    /* <link rel="stylesheet" href="/js/styles/obsidian.css"/> */
    <link rel="stylesheet" href="//brick.a.ssl.fastly.net/Linux+Libertine:400,400i,700,700i/Open+Sans:400,400i,700,700i"/>
    <link href="https://fonts.googleapis.com/css?family=Inconsolata:400,700" rel="stylesheet"/>
    /* <link href="//netdna.bootstrapcdn.com/font-awesome/4.1.0/css/font-awesome.min.css" rel="stylesheet"/> */

    /* <link rel="stylesheet" media="screen" href="/css/main.css" />
    <link rel="stylesheet" media="print" href="/css/print.css" />
    <link rel="stylesheet" media="screen" href="/css/solarized-dark.css" /> */

    /* <asyncScript src="//platform.twitter.com/widgets.js" /> */
    <style>
      ({|
      div {
        flex-shrink: 0;
        flex-wrap: wrap;
        box-sizing: border-box;
        min-height: 0;
        min-width: 0;
      }
      span {
        box-sizing: border-box;
      }
      pre {
        line-height: 18px;
        font-size: 16px;
        /* white-space: pre-wrap; */
        overflow: auto;
        max-width: 100%;
        padding: 15px;
        /* border: 1px solid #ddd; */
        font-family: Inconsolata;
        background-color: |} ++ Colors.lightOrange ++ {|;
        color: #692900;
        border-radius: 4px;
        margin: 0;
        margin-bottom: 1em;
      }
      img {
        max-width: 100%;
      }
      li > code,
      a > code,
      p > code {
        font-size: 90%;
        padding: 4px;
        background-color: #ffede4;
        color: #692900;
        border-radius: 4px;
        hyphens: none;
      }
      blockquote {
          margin: 0;
          margin-bottom: 1em;
          padding-left: 20px;
          border-left: 5px solid |} ++ Colors.green ++ {|;
          font-style: italic;
      }
      blockquote.twitter-tweet {
        font-style: normal;
        border: 1px solid |} ++ Colors.green ++ {|;
        border-radius: 4px;
        padding: 8px 16px;
        font-size: 20px;
      }
      a {
        color: |} ++ Colors.green ++ {|;
        /* color: #28cc4c;
        text-decoration-color: #28cc4c; */
      }
      h2 {
        padding-bottom: 16px;
        font-size: 36px;
        margin: 0;
        margin-top: 1em;
      }
      p {
        margin: 0;
        padding-bottom: 1em;
      }
      blockquote > p:last-child {
        padding-bottom: 0;
      }
      h3 {
        margin-bottom: 8px;
        font-family: Open sans;
        font-weight: normal;
        margin-top: 8px;
        color: #6e6e6e;
        font-size: 30px;
      }
      h5 {
        font-family: open sans;
        margin: 0;
        margin-top: 16px;
      }
      /* a:visited {
        color: #333;
      } */
      |})
    </style>

    (String.concat("\n", children))
  </head>
};


let pageWithTopAndBottom = (~css, ~backgroundImage=?, ~top, ~middle, ~bottom, ~children, ()) => {
  open Html;
  open Css;
  <body className=css([
    A("font-family", "Linux Libertine"),
    A("color", Colors.text),
  ]) lang="en">
    <a
      href="/"
      className=css([
        A("position", "fixed"),
        A("top", "10px"),
        A("left", "10px"),
        A("padding", "8px"),
        A("background-color", "black"),
        Media("max-width: 600px", [
          ("display", "none"),
        ])
      ])
    >
      <div className=css([
        A("height", "32px"),
        A("width", "32px"),
        A("background-size", "cover")
      ]) style="background-image: url(/images/logo/JF_64.png)" />
    </a>
    <main role="main">
      <article lang="en">
        <header>
          (switch backgroundImage {
          | None => ""
          | Some(backgroundImage) => (
            <div style=("background-image: url(" ++ backgroundImage ++ ")")
              className=css([
                A("height", "400px"),
                A("background-size", "cover"),
                A("background-position", "center"),
                A("margin", "-8px -8px 0"),
              ])
            >
            </div>
           )
          })
          <div className=css([
            A("max-width", "700px"),
            A("margin", "0 auto 48px")
          ])>
            top
          </div>
        </header>
        <div className=css([
          A("max-width", "700px"),
          A("margin", "0 auto")
        ])>
          middle
        </div>
      </article>
    </main>

    <footer>
      <div className=css([
        A("max-width", "700px"),
        A("margin", "100px auto 150px"),
        A("text-align", "center"),
      ])>
        bottom
      </div>
    </footer>
  </body>
};

let spacer = num => Html.(<div style=("flex-basis: " ++ string_of_int(num) ++ "px") />);
let hspace = num => Html.(<div style=("height: " ++ string_of_int(num) ++ "px") />);
let vspace = num => Html.(<div style=("width: " ++ string_of_int(num) ++ "px") />);

let monthName = month => switch month {
| 1 => "January"
| 2 => "February"
| 3 => "March"
| 4 => "April"
| 5 => "May"
| 6 => "June"
| 7 => "July"
| 8 => "August"
| 9 => "September"
| 10 => "October"
| 11 => "November"
| 12 => "December"
| _ => failwith("Invalid month")
};

let showDate = (~date as (year, month, day), ~children, ()) => {
  open Html;
  <fragment>
    (monthName(month))
    (spacer(4))
    (string_of_int(day) ++ ",")
    (spacer(4))
    (string_of_int(year))
  </fragment>
};

let dateSort = ((y1, m1, d1), (y2, m2, d2)) => {
  switch (y1 - y2) {
  | 0 => switch (m1 - m2) {
    | 0 => d1 - d2
    | x => x
    }
  | x => x
  }
};

let myBigFaceStatic = css => {
  open Css;
  open Html;
  let size = 160;
  let ssize = string_of_int(size) ++ "px";
  <a
    href="/"
    className=css([
      A("margin", "0 auto"),
      A("width", ssize),
      A("overflow", "hidden"),
      A("height", ssize),
      A("display", "block"),
      A("border-radius", "50%"),
      A("border", "5px solid white"),
      A("background-color", "white"),
      A("box-shadow", "0 1px 1px rgba(0, 0, 0, 0.3)"),
    ])
  >
    <img
      src="https://www.gravatar.com/avatar/313878fc8f316fc3fe4443b13913d0a4.png?s=200"
      alt="Jared Forsyth"
      className=css([
        A("display", "block"),
        A("width", ssize),
        A("height", ssize),
      ])
    />
  </a>
};

let myBigFace = css => Css.(Html.(
  <a
    href="/"
    style="background-image: url(https://www.gravatar.com/avatar/313878fc8f316fc3fe4443b13913d0a4.png?s=200)"
    className=css([
      A("width", "120px"),
      A("height", "120px"),
      A("position", "absolute"),
      A("top", "-60px"),
      A("right", "50%"),
      A("margin-right", "-60px"),
      A("background-size", "cover"),
      A("border-radius", "60px"),
      A("z-index", "99"),
      A("text-indent", "-9999px"),
      A("border", "3px solid white"),
      A("background-color", "white"),
      A("box-shadow", "0 1px 1px rgba(0, 0, 0, 0.3)"),
    ])
  >"Jared Forsyth"</a>
));

let module Styles = {
  open Css;
  let title = [
    A("font-size", "56px"),
    A("margin-bottom", "16px"),
    A("padding-top", "32px"),
    Media("max-width: 600px", [
      ("font-size", "32px"),
      ("margin-top", "40px"),
    ])
  ];

  let titleWithTopMargin = [
    A("margin-top", "100px"),
    Media("max-width: 600px", [
      ("margin-top", "40px"),
    ]),
    ...title
  ];

  let bodyText = [
    A("font-size", "24px"),
    A("line-height", "36px"),
    A("hyphens", "auto"),
    Media("max-width: 600px", [
      ("font-size", "20px"),
      ("line-height", "30px"),
    ])
  ];

  let colPaddingRules = [
    A("padding", px(Consts.colPadding)),
    Media("max-width: " ++ px(Consts.Media.skinnyFourCol), [
      ("padding", px(Consts.colPadding / 2))
    ]),
    Media("max-width: " ++ px(Consts.Media.oneCol), [
      ("padding", px(Consts.colPaddingSmall))
    ])
  ];

  let row = [
    A("display", "flex"),
    A("flex-direction", "row"),
  ];

  let column = [
    A("flex", "1"),
    A("min-width", "300px"),
    Media("max-width: " ++ px(Consts.Media.oneCol), [
      ("min-width", "300px"),
    ]),
    ...colPaddingRules
  ];

  let statusText = [
    A("font-size", px(Consts.statusSize)),
    A("color", Colors.red)
  ];

  let metaText = [
    A("font-family", "Open sans, sans-serif"),
    A("color", Colors.lightText),
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

  let basicImage = [
    /* A("box-shadow", "0 0 5px #aaa"), */
    A("border", "1px solid #ddd"),
    A("width", "100%"),
    A("object-fit", "cover"),
  ];

  let columnWrapper = [
    A("flex-direction", "row"),
    A("padding", "0 " ++ px(Consts.colPadding / 2)),
    A("display", "flex"),
    A("justify-content", "center"),
    A("align-items", "stretch"),
    A("flex-wrap", "wrap"),
    Media("max-width: " ++ px(Consts.Media.oneCol), [
      ("padding", px(Consts.colPaddingSmall / 2))
    ])
  ];

  let fullBody = [
    A("font-family", "Linux Libertine"),
    A("color", Colors.text),
    A("margin", "0"),
    A("padding", "0")
  ];

  let pinkDivider = [
    A("height", "3px"),
    A("background-color", Colors.lightOrange),
    A("box-shadow", "0 0.5px 1px #c2c2c2"),
  ];
};

let module Header = {
  open Html;
  open Css;
  let createElement = (~css, ~href, ~title, ~children, ()) =>
    <a href className=css([
      A("color", Colors.darkGreen),
      ...Styles.hoverUnderline
    ])>
      <h1 className=css([
        A("margin-top", "0"),
        A("font-size", px(Consts.jaredSize))
      ])>title</h1>
    </a>
  ;
};

let minuteRead = wordCount => wordCount / 180;

let minuteReadText = wordCount => switch wordCount {
| Some(wordCount) => string_of_int(minuteRead(wordCount)) ++ " minute read"
| None => "Read more"
};
