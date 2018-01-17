
let (|?) = (x, y) => switch x { | None => y | Some(x) => x };

let module Colors = {
  let text = "#333";
  let lightText = "#999";
};

let twitter = (~contentTitle, ~description, ~thumbnail, ~children, ()) => {
  open Html;
  <fragment>
    <meta name="twitter:card" content="summary"/>
    <meta name="twitter:title" content=contentTitle />
    <meta name="twitter:description" content=description />
    <meta name="twitter:image" content=(thumbnail |? "https://jaredforsyth.com/images/logo/JF_black_128.png")/>
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

let pageHead = (~title as contentTitle, ~description=?, ~thumbnail=?, ~extraHead="", ~children, ()) => {
  let contentTitle = contentTitle ++ " | Jared Forsyth.com";
  let description = switch (description) {
  | Some(description) => description
  | None => "Thoughts about programming, mostly"
  };
  open Html;
  <head>
    {Meta.charset("utf8")}

    <meta name="description" content=description />

    <mobileMeta />
    <twitter
      contentTitle
      description
      thumbnail
    />

    <pmeta property="og:type" content="article"/>
    <pmeta property="og:title" content=contentTitle/>
    <pmeta property="og:description" content=description/>

    <link rel="shortcut icon" href="/images/logo/JF_black_32.png"/>

    /* TODO audit these */
    <link rel="stylesheet" href="/js/styles/obsidian.css"/>
    <link rel="stylesheet" href="//brick.a.ssl.fastly.net/Linux+Libertine:400,400i,700,700i/Open+Sans:400,400i,700,700i"/>
    <link href="https://fonts.googleapis.com/css?family=Inconsolata:400,700" rel="stylesheet"/>
    <link href="//netdna.bootstrapcdn.com/font-awesome/4.1.0/css/font-awesome.min.css" rel="stylesheet"/>

    <link rel="stylesheet" media="screen" href="/css/main.css" />
    <link rel="stylesheet" media="print" href="/css/print.css" />
    <link rel="stylesheet" media="screen" href="/css/solarized-dark.css" />

    <style>
      {|
      pre {
        line-height: 18px;
        font-size: 16px;
        /* white-space: pre-wrap; */
        overflow: auto;
        max-width: 100%;
        padding: 15px;
        border: 1px solid #ddd;
        font-family: Inconsolata;
      }
      p > code {
        font-size: 20px;
        padding: 4px;
        background-color: #ffede4;
        border-radius: 4px;
        color: #692900;
      }
      a {
        color: #1fad3e;
        /* color: #28cc4c;
        text-decoration-color: #28cc4c; */
      }
      /* a:visited {
        color: #333;
      } */
      |}
    </style>

    <title>{contentTitle}</title>
    (String.concat("\n", children))
  </head>
};


let pageWithTopAndBottom = (~css, ~top, ~middle, ~bottom, ~children, ()) => {
  open Html;
  <body className=css([
    ("font-family", "Linux Libertine"),
    ("color", Colors.text),
  ])>
    <main role="main">
      <article>
        <header>
          <div className=css([
            ("max-width", "700px"),
            ("margin", "0 auto 48px")
          ])>
            top
          </div>
        </header>
        <div className=css([
          ("max-width", "700px"),
          ("margin", "0 auto")
        ])>
          middle
        </div>
      </article>
    </main>

    <footer>
      <div className=css([
        ("max-width", "700px"),
        ("margin", "100px auto 150px"),
        ("text-align", "center"),
      ])>
        bottom
      </div>
    </footer>
  </body>
};

let spacer = num => Html.(<div style=("flex-basis: " ++ string_of_int(num) ++ "px") />);

let postAbout = (~css, ~date, ~tags, ~children, ()) => {
  open Html;
  <div className=css([
    ("color", Colors.lightText),
    ("font-family", "Open sans"),
    ("font-size", "14px"),
    ("display", "flex"),
    ("flex-direction", "row"),
    ("align-items", "center"),
    ("justify-content", "flex-start"),
  ])>
    <div className=css([
      ("background-image", "url(https://www.gravatar.com/avatar/313878fc8f316fc3fe4443b13913d0a4.png?s=30)"),
      ("background-size", "cover"),
      ("display", "inline-block"),
      ("width", "30px"),
      ("height", "30px"),
      ("border-radius", "50%")
    ]) />
    (spacer(12))
    <a href="/about" className=css([
      ("color", "currentColor"),
      ("text-decoration", "none"),
      ("font-weight", "bold")
    ])>"Jared Forsyth"</a>
    (spacer(4))
    "on "
    (spacer(4))
    date
    (spacer(4))
    " tagged "
    (spacer(4))
    (String.concat(", " ++ spacer(4), List.map(tag => <a href=("/tags/" ++ tag)>tag</a>, tags)))
  </div>
};

let renderPost = (~title as contentTitle, ~description, ~date, ~tags, ~thumbnail, rawBody) => {
  let (css, inlineCss) = Html.startPage();
  open Html;

  let body = <pageWithTopAndBottom
      css
      top={
        <fragment>
          <h1 className=css([
            ("font-size", "56px"),
            ("margin-top", "150px"),
            ("margin-bottom", "16px"),
          ])>contentTitle</h1>
          <postAbout css date tags />
        </fragment>
      }
      middle=(
        <div
        className=css([
          ("font-size", "24px"),
          ("line-height", "36px"),
        ])
        >
          (Omd.to_html(Omd.of_string(rawBody)))
        </div>
      )
      bottom=("This is the personal site of Jared Forsyth")
    />;

  <html>
    <pageHead
      title=contentTitle
      ?description
      ?thumbnail
    >
      <style>(inlineCss())</style>
    </pageHead>

    body
  </html>
};
