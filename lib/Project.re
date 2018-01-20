
let (|?) = (x, y) => switch x { | None => y | Some(x) => x };
let (|?>) = (x, f) => switch x { | None => None | Some(x) => f(x) };
let (|?>>) = (x, f) => switch x { | None => None | Some(x) => Some(f(x)) };
let (|!) = (x, y) => switch x { | None => failwith(y) | Some(x) => x };

let pageWithTopAndBottom = Shared.pageWithTopAndBottom;
let pageHead = Shared.pageHead;

let renderUpdate = (css, (opts, content)) => {
  open Html;
  open Css;
  <div className=css([A("margin-bottom", "17px")])>
    (MarkdownParser.parse(content))
  </div>
};

let render = (contentTitle, description, updates) => {
  let (css, inlineCss) = Css.startPage();
  open Html;
  open Css;

  let body = <pageWithTopAndBottom
    css
    top={ <h1 className=css(Shared.Styles.titleWithTopMargin)>contentTitle</h1>}
    middle=(
      <div className=css(Shared.Styles.bodyText)>
        (MarkdownParser.parse(description))
        <div style="height: 64px"/>
        (List.map(renderUpdate(css), updates) |> String.concat("\n"))
      </div>
    )
    bottom=("This is the personal site of Jared Forsyth")
  />;

  <html>
    <pageHead title=contentTitle description>
      <style>(inlineCss())</style>
    </pageHead>
    body
  </html>
};

let render = (fileName, opts, rawBody) => {
  let opts = opts |! "No options for static file " ++ fileName;
  let title = Toml.string("title", opts) |! "No title for static page " ++ fileName;
  let description = Toml.string("description", opts) |! "No description for static page " ++ fileName;
  let updates = Util.split("\n---\n", rawBody)
  |> List.map(
    update => {
      let (opts, content) = Util.splitFirst("\n\n", update);
      let opts = opts |?>> Toml.parse;
      (opts, content)
    }
  );
  (render(title, description, updates), ())
};
