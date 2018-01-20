
let pageHead = Shared.pageHead;

let module Header = {
  open Html;
  open Css;
  let createElement = (~css, ~href, ~title, ~children, ()) =>
    <a href className=css([
      A("text-decoration", "none"),
      A("color", "#147429")
    ])>
      <h1>title</h1>
    </a>
  ;
};

let render = (~projects, ~posts, ~tags, ~talks) => {
  open Html;
  open Css;
  let (css, inlineCss) = Css.startPage();
  open Html;
  open Css;

  let column = [A("flex", "1"), A("padding", "16px"), A("min-width", "300px")];

  let body = <body className=css([
    A("font-family", "Linux Libertine"),
    A("color", Shared.Colors.text),
  ]) lang="en">
    <div className=css([
      A("flex-direction", "row"),
      A("display", "flex"),
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
              let (year, month, _) = config.date;
              <div>
                <a href className=css([
                  A("font-size", "26px"),
                  A("text-decoration", "none"),
                  A("color", "currentColor"),
                ])>
                  (config.title)
                </a>
                (Shared.hspace(8))
                <div className=css([A("display", "flex"), A("flex-direction", "row")])>
                  (string_of_int(year))
                  (Shared.monthName(month))
                  /* (Shared.showDate(~date=config.date, ~children=[], ())) */
                  <div style="flex: 1"/>
                  (Shared.minuteReadText(config.wordCount))
                </div>
              </div>
            },
            posts
          ) |> String.concat("\n" ++ Shared.hspace(32)))
        </div>
      </div>

      <div className=css([Media("max-width: 1015px", [("order", "1")]), ...column])>
        <Header href="/projects/" css title="Projects" />
      </div>

      <div className=css([A("display", "flex"), A("flex-direction", "column"), ...column] @ [
        Media("min-width: 1341px", [("flex", "2"), ("flex-direction", "row"), ("min-width", "600px")])
      ])>
      <div className=css([Media("max-width: 1340px", [("order", "1")]), A("flex", "1")])>
        <Header href="/talks/" css title="Talks" />
      </div>

      <div className=css([Media("min-width: 1341px", [("flex", "1")])])>
        "I'm Jared Forsyth"
      </div>
      </div>
    </div>
  </body>;

  <html>
    <pageHead title="Jared Forsyh.com" description="My website">
      <style>(inlineCss())</style>
    </pageHead>
    body
  </html>
};
