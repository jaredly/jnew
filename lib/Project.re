
let (|?) = (x, y) => switch x { | None => y | Some(x) => x };
let (|?>) = (x, f) => switch x { | None => None | Some(x) => f(x) };
let (|?>>) = (x, f) => switch x { | None => None | Some(x) => Some(f(x)) };
let (|!) = (x, y) => switch x { | None => failwith(y) | Some(x) => x };

type project = {
  title: string,
  description: string,
  screenshot: option(string),
  wip: bool,
  updates: list(((int, int, int), option(string), string))
};

let pageWithTopAndBottom = Shared.pageWithTopAndBottom;
let pageHead = Shared.pageHead;

let renderUpdate = (css, ((year, month, day), screenshot, content)) => {
  open Html;
  open Css;
  <div className=css([A("margin-bottom", "17px")])>
    (string_of_int(year))
    (Shared.monthName(month))
    (string_of_int(day))
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
  let screenshot = Toml.string("screenshot", opts);
  let wip = Toml.bool("wip", opts) |? false;
  let updates = Util.split("\n---\n", rawBody)
  |> List.map(
    update => {
      let (opts, content) = Util.splitFirst("\n\n", update);
      let opts = opts |?>> Toml.parse |! "No options for update " ++ fileName;
      let date = Toml.string("date", opts) |! "No date for project update " ++ fileName |> Post.parseDate;

      let screenshot = Toml.string("screenshot", opts);
      (date, screenshot, content)
    }
  ) |> List.sort(((date, _, _), (date2, _, _)) => Shared.dateSort(date2, date));
  (render(title, description, updates), {
    title,
    description,
    screenshot,
    wip,
    updates
  })
};
