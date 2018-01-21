
let (|?) = (x, y) => switch x { | None => y | Some(x) => x };
let (|?>) = (x, f) => switch x { | None => None | Some(x) => f(x) };
let (|?>>) = (x, f) => switch x { | None => None | Some(x) => Some(f(x)) };
let (|!) = (x, y) => switch x { | None => failwith(y) | Some(x) => x };

type project = {
  title: string,
  description: string,
  longDescription: option(string),
  screenshot: option(string),
  github: option(string),
  link: option(string),
  tags: list(string),
  status: option(string),
  id: string,
  fileName: string,
  wip: bool,
  updates: list(((int, int, int), option(string), string))
};

let pageWithTopAndBottom = Shared.pageWithTopAndBottom;
let pageHead = Shared.pageHead;

let renderUpdate = (css, ((year, month, day), screenshot, content)) => {
  open Html;
  open Css;
  <div className=css([A("margin-bottom", "17px")])>
    <div className=css([
      A("font-size", "16px"),
      A("color", Shared.Colors.lightText),
      A("font-family", "Open sans"),
      /* A("font-size", "14px"), */
    ])>
    (string_of_int(year))
    (Shared.monthName(month))
    (string_of_int(day))
    </div>
    (MarkdownParser.parse(content))
  </div>
};

let render = ({title: contentTitle, description, longDescription, status, screenshot, updates}) => {
  let (css, inlineCss) = Css.startPage();
  open Html;
  open Css;

  let body = <pageWithTopAndBottom
    css
    top={ <fragment>
      <h1 className=css(Shared.Styles.titleWithTopMargin)>contentTitle</h1>
      <div className=css(Shared.Styles.bodyText)>
      (MarkdownParser.parse(description))
      (switch status {
      | None => ""
      | Some(status) => <div
          className=css([
            A("font-family", "Open sans, sans-serif")
          ])
        >status</div>
      })
      </div>
    </fragment>}
    middle=(
      <div className=css(Shared.Styles.bodyText)>
        (switch screenshot {
        | None => ""
        | Some(src) => <img src alt=(contentTitle ++ " screenshot") className=css([
            A("width", "100%")
          ]) />
        })
        (switch longDescription {
        | None => ""
        | Some(text) => <div style="padding: 16px 0">(MarkdownParser.parse(text))</div>
        })
        <div style="height: 32px"/>
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

let fullGithub = text => if (Str.string_match(Str.regexp("^[a-zA-Z0-9.]+$"), text, 0)) {
  "https://github.com/jaredly/" ++ text
} else {
  text
};

let splitTags = text => Str.split(Str.regexp(","), text) |> List.map(String.trim);

let render = (fileName, opts, rawBody) => {
  let opts = opts |! "No options for static file " ++ fileName;
  let title = Toml.string("title", opts) |! "No title for static page " ++ fileName;
  let description = Toml.string("description", opts) |! "No description for static page " ++ fileName;
  let screenshot = Toml.string("screenshot", opts);
  let status = Toml.string("status", opts);
  let github = Toml.string("github", opts) |?>> fullGithub;
  let link = Toml.string("link", opts);
  let tags = Toml.string("tags", opts) |?>> splitTags |? [];
  let wip = Toml.bool("wip", opts) |? false;

  let (longDescription, rawBody) = Util.splitFirst("\n===\n", rawBody);

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
  let config = {
    title,
    id: Filename.basename(fileName) |> Filename.chop_extension,
    github,
    link,
    status,
    tags,
    fileName,
    description,
    longDescription,
    screenshot,
    wip,
    updates
  };
  (render(config), config)
};

let renderList = (projects, contentTitle) => {
  open Html;
  let (css, inlineCss) = Css.startPage();
  let body = <pageWithTopAndBottom
    css
    backgroundImage="/images/trees.jpg"
    top=(
      <div className=css([A("padding", "1px"), A("position", "relative")])>
        (Shared.myBigFace(css))
        <h1 className=css([
          A("text-align", "center"),
          ...Shared.Styles.title
        ])>contentTitle</h1>
      </div>
    )
    middle=(
      List.map(
        ({title, description, screenshot, wip, fileName, updates}) => {
          open Types;
          let href = ("/" ++ Filename.chop_extension(fileName) ++ "/");
          let numUpdates = List.length(updates);
          <div>
            <a href className=css([
                A("color", "currentColor"),
                A("text-decoration", "none")
              ])
            >
              <h2>(title)</h2>
            </a>
            (switch screenshot {
            | None => ""
            | Some(src) => <img src alt="Screenshot" />
            })
            <div className=css([A("padding-top", "16px"), ...Shared.Styles.bodyText])>
              (Omd.to_html(Omd.of_string(description)))
            </div>
            <a className=css([A("font-size", "24px")]) href>(string_of_int(numUpdates)) (numUpdates == 1 ? "Update" : "Updates")</a>
          </div>
        },
        projects
      ) |> String.concat("\n<div style='height: 32px'></div>\n")
    )
    bottom=("This is the personal site of Jared Forsyth")
  />;
  <html>
    <pageHead
      title=contentTitle
      description="Things Jared has written about"
    >
      <style>(inlineCss())</style>
    </pageHead>
    body
  </html>
};
