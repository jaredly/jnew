
let (|?) = (x, y) => switch x { | None => y | Some(x) => x };
let (|!) = (x, y) => switch x { | None => failwith(y) | Some(x) => x };

let pageWithTopAndBottom = Shared.pageWithTopAndBottom;
let pageHead = Shared.pageHead;

let render = (contentTitle, description, rawBody) => {
  let (css, inlineCss) = Css.startPage();
  open Html;
  open Css;

  let body = <pageWithTopAndBottom
    css
    top={ <h1 className=css(Shared.Styles.titleWithTopMargin)>contentTitle</h1>}
    middle=(
      <div className=css(Shared.Styles.bodyText)> (MarkdownParser.process(rawBody)) </div>
    )
    bottom=("This is the personal site of Jared Forsyth")
  />;

  <html>
    <pageHead title=contentTitle ?description>
      <style>(inlineCss())</style>
    </pageHead>
    body
  </html>
};

let render = (fileName, opts, rawBody) => {
  let opts = opts |! "No options for static file " ++ fileName;
  let title = Toml.string("title", opts) |! "No title for static page " ++ fileName;
  let description = Toml.string("description", opts);
  (render(title, description, rawBody), ())
};