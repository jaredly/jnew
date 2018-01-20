
let (|?) = (x, y) => switch x { | None => y | Some(x) => x };
let (|?>) = (x, f) => switch x { | None => None | Some(x) => f(x) };
let (|?>>) = (x, f) => switch x { | None => None | Some(x) => Some(f(x)) };
let (|!) = (x, y) => switch x { | None => failwith(y) | Some(x) => x };

let spacer = Shared.spacer;
let userPic = Shared.userPic;
let showDate = Shared.showDate;

let postAbout = (~css, ~date, ~tags, ~withPic=true, ~children, ()) => {
  open Html;
  open Css;
  <div className=css([
    A("color", Shared.Colors.lightText),
    A("font-family", "Open sans"),
    A("font-size", "14px"),
    A("display", "flex"),
    A("flex-direction", "row"),
    A("align-items", "center"),
    A("justify-content", "flex-start"),
  ])>
    (withPic
      ? <fragment>
        <userPic css />
        (spacer(12))
        <a href="/about" className=css([
          A("color", "currentColor"),
          A("text-decoration", "none"),
          A("font-weight", "bold")
        ])>"Jared Forsyth"</a>
        (spacer(4))
        " · "
        (spacer(4))
      </fragment>
      : "")
    <showDate date />
    (spacer(8))
    " · "
    (spacer(8))
    (String.concat(", " ++ spacer(4), List.map(tag => <a
      href=("/tags/" ++ tag ++ "/")
      className=css([A("text-decoration", "none")])
    >tag</a>, tags)))
  </div>
};

let pageWithTopAndBottom = Shared.pageWithTopAndBottom;
let pageHead = Shared.pageHead;

let renderPost = ({Types.title: contentTitle, description, date, tags, thumbnail}, rawBody) => {
  let (css, inlineCss) = Css.startPage();
  open Html;
  open Css;

  let body = <pageWithTopAndBottom
    css
    top={
      <fragment>
        <h1 className=css(Shared.Styles.titleWithTopMargin)>contentTitle</h1>
        <postAbout css date tags />
      </fragment>
    }
    middle=(
      <div className=css(Shared.Styles.bodyText)>
        (MarkdownParser.parse(rawBody))
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

let postList = (posts, contentTitle) => {
  open Html;
  let (css, inlineCss) = Css.startPage();
  /* let contentTitle = "All posts"; */
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
        ((config, teaser, _)) => {
          open Types;
          let href = ("/" ++ Filename.chop_extension(config.Types.fileName) ++ "/");
          let readTime = Shared.minuteReadText(config.wordCount);
          <div>
            <a href className=css([
                A("color", "currentColor"),
                A("text-decoration", "none")
              ])
            >
              <h2>(config.title)</h2>
            </a>
            <postAbout css date=config.date tags=config.tags withPic=false />
            (switch teaser {
            | None => ""
            | Some(teaser) =>
              <div className=css([A("padding-top", "16px"), ...Shared.Styles.bodyText])>
                (Omd.to_html(Omd.of_string(teaser)))
              </div>
            })
            <a className=css([A("font-size", "24px")]) href>readTime</a>
          </div>
        },
        posts
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



open Types;
let defaultConfig = fileName => {
  fileName,
  title: "JaredForsyth.com",
  tags: [],
  categories: [],
  date: (0, 0, 0), /* Year, Month, Day */
  description: None,
  thumbnail: None,
  featured: false,
  wordCount: 0
};

let check = (opt, base, fn) => switch opt {
| None => base
| Some(value) => fn(value)
};

let parseDate = text => {
  Scanf.sscanf(text, "%d-%d-%d", (year, month, day) => (year, month, day))
};

let parseConfig = (fileName, opts) => {
  let config = defaultConfig(fileName);
  let config = check(Toml.string("title", opts), config, title => {...config, title});
  let config = check(Toml.string("description", opts), config, description => {...config, description: Some(description)});
  let config = check(Toml.stringList("tags", opts), config, tags => {...config, tags});
  let config = check(Toml.stringList("categories", opts), config, categories => {...config, categories});
  let config = check(Toml.string("date", opts), config, date => {...config, date: parseDate(date)});
  let config = check(Toml.bool("featured", opts), config, featured => {...config, featured});
  config
};

let getIntro = body => switch (Str.split(Str.regexp("<!-- more -->"), body)) {
| [] => assert(false)
| [one] => None
| [top, ...rest] => Some(top)
};

let render = (fileName, opts, content) => {
  let opts = opts |! "No options for post " ++ fileName;
  let config = parseConfig(fileName, opts);
  let intro = getIntro(content);
  let wordCount = Str.split(Str.regexp("[^a-zA-Z0-9-]+"), content) |> List.length;
  let config = {...config, wordCount};

  (renderPost(config, content), (config, intro, content))
};