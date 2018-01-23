
open Shared.Infix;
let module Consts = Shared.Consts;
let px = Shared.px;

let column = (~nav=false, ~contents=?, css) => Html.(Css.(
  <div className=css([
    Media("min-width: " ++ px(Consts.Media.oneCol), [
      ("position", "sticky"),
      ("top", px(Consts.colPadding / 2)),
      ("height", "calc(100vh - " ++ px(Consts.colPadding) ++ ")"),
      ("display", "flex"),
      ("flex-wrap", "nowrap"),
      ("flex-direction", "column"),
    ])
  ])>
    (Shared.myBigFaceStatic(css))
    (Shared.hspace(Consts.bigSpace))
    <div className=css([A("text-align", "center"), A("font-size", px(Consts.jaredSize))])>
      "I'm Jared Forsyth"
    </div>
    (Shared.hspace(Consts.bigSpace))
    <div className=css([
      A("font-size", px(Consts.titleSize)),
      A("line-height", "36px"),
      A("flex", "1"),
      A("display", "flex"),
      A("flex-direction", "column"),
      A("flex-wrap", "nowrap"),
      A("text-align", "left"),
    ])>
      (if (nav) {
        <div className=css([
          A("margin-bottom", Shared.px(Consts.bigSpace)),
          Sub("a", [
            ("margin", "0 8px")
          ]),
          A("justify-content", "center"),
          ...Shared.Styles.row
        ])>
          <a href="/" > "home" </a>
          <a href="/posts/" > "posts" </a>
          <a href="/projects/" > "projects" </a>
          <a href="/talks/" > "talks" </a>
        </div>
      } else {
        ""
      })
      (contents |?
      <fragment>
        <p>
        "I’m an idealist with strong opinions, but I'm happy to learn where I'm wrong. We all need more empathy & compassion. The most important aspect of my life is my connection to God."
        </p>
        <p>
        "I’m currently a mobile & web developer for Khan Academy, where we’re working to bring a free, world-class education to anyone anywhere. If you want to help, get in touch!"
        </p>
      </fragment>)
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
  </div>
));

let bodyWithAboutMeColumn = (~css, ~children, ()) => {
  open Html;
  open Css;

  let module Styles = Shared.Styles;

  let body = <body className=css(Styles.fullBody) lang="en">
    <div className=css(Styles.columnWrapper)>

      <div className=css([A("background-color", Shared.Colors.lightOrange), ...Styles.column])>
        (column(css))
      </div>

      (children |> String.concat("\n"))
    </div>
  </body>;

  body
};

let bodyWithSmallAboutMeColumn = (~css, ~toc, ~children, ()) => {
  open Html;
  open Css;

  let module Styles = Shared.Styles;

  let body = <body className=css(Styles.fullBody) lang="en">
    <div className=css(Styles.columnWrapper @ [
      A("justify-content", "flex-start"),
      A("flex-wrap", "nowrap"),
      Media("max-width: " ++ "1060px", [
        ("justify-content", "center"),
      ]),
    ])>

      <div className=css([
        A("background-color", Shared.Colors.lightOrange),
        A("max-width", "400px"),
        A("flex-shrink", "1"),
        A("min-width", "300px"),
        Media("max-width: " ++ "1060px", [
          ("display", "none"),
        ]),
        ...Styles.colPaddingRules
      ])>
        (column(
          ~nav=true,
          ~contents=toc, css))
      </div>

      <div
        style="flex-basis: 100px; flex-shrink: 1"
        className=css([ Media("max-width: " ++ "1060px", [ ("display", "none"), ]), ])
      />
      <div className=css([
        A("max-width", "700px")
      ])>
      (children |> String.concat("\n"))
      <div style="height: 128px"/>
      </div>
      <div
        style="flex-basis: 100px; flex-shrink: 1"
        className=css([ Media("max-width: " ++ "1060px", [ ("display", "none"), ]), ])
      />
    </div>
  </body>;

  body
};
