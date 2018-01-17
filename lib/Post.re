
let (|?) = (x, y) => switch x { | None => y | Some(x) => x };

let pageHead = (~title as contentTitle, ~description=?, ~thumbnail=?, ~extraHead="", ~children, ()) => {
  let contentTitle = contentTitle ++ " | Jared Forsyth.com";
  let description = switch (description) {
  | Some(description) => description
  | None => "Thoughts about programming, mostly"
  };
  open Html;
  <head>
    {Meta.charset("utf8")}
    {Meta.mobile}
    {Meta.description(description)}

    <meta name="HandheldFriendly" content="True" />
    <meta name="MobileOptimized" content="320" />
    <meta name="apple-mobile-web-app-capable" content="yes"/>
    <meta name="apple-mobile-web-app-status-bar-style" content="black-translucent"/>

    <meta name="twitter:card" content="summary"/>
    <meta name="twitter:title" content=contentTitle />
    <meta name="twitter:description" content=description />
    <meta name="twitter:image" content=(thumbnail |? "https://jaredforsyth.com/images/logo/JF_black_128.png")/>
    <meta name="twitter:site" content="https://jaredforsyth.com"/>
    <meta name="twitter:creator" content="@jaredforsyth"/>

    <meta name="google-site-verification" content=""/>
    <pmeta property="fb:admins" content=""/>
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
      code {
        font-size: 16px;
      }
      pre {
        line-height: 18px;
        /* white-space: pre-wrap; */
        overflow: auto;
        max-width: 100%;
        padding: 15px;
        border: 1px solid #ddd;
        font-family: Inconsolata;
      }
      |}
    </style>

    <title>{contentTitle}</title>
    (String.concat("\n", children))
  </head>
};


let pageWithTopAndBottom = (~css, ~top, ~middle, ~bottom, ~children, ()) => {
  open Html;
  <body className=css([
    ("font-family", "Linux Libertine")
  ])>
    <main role="main">
      <article>
        <header>
          <div className=css([
            ("max-width", "700px"),
            ("margin", "0 auto")
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
      bottom
    </footer>
  </body>
};

let renderPost = (~title as contentTitle, ~description, ~thumbnail, rawBody) => {
  let (css, inlineCss) = Html.startPage();
  open Html;

  let body = <pageWithTopAndBottom
      css
      top={
        <fragment>
          <h1 className=css([
            ("font-size", "40px")
          ])>contentTitle</h1>
          <div>
            <a href="/about">"About"</a>
          </div>
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
    /* Link to homepage, via a little logo */
    /* Title thing */


  </html>
};
