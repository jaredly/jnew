
let module Consts = Shared.Consts;
let px = Shared.px;

let column = css => Html.(Css.(
  <div className=css([
    Media("min-width: " ++ px(Consts.Media.oneCol), [
      ("position", "sticky"),
      ("top", px(Consts.colPadding / 2))
    ])
  ])>
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
  </div>
));