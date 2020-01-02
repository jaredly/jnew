
let (|?) = (x, y) => switch x { | None => y | Some(x) => x };
let (|?>) = (x, f) => switch x { | None => None | Some(x) => f(x) };
let (|?>>) = (x, f) => switch x { | None => None | Some(x) => Some(f(x)) };
let (|!) = (x, y) => switch x { | None => failwith(y) | Some(x) => x };


let spacer = Shared.spacer;
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

type postBody = Html(string) | Markdown(string) | Nm(list(NotableMind.nmNode))
type post = {config: Types.config, body: postBody, intro: option(postBody)};

let renderBody = fun
  | Markdown(md) => MarkdownParser.process(md)
  | Html(html) => html
  | Nm(nodes) => NotableMind.render(nodes)

let render = (posts, {config: {Types.title: contentTitle, fileName, description, date, tags, thumbnail, article_image}, body: postBody}) => {
  let (css, inlineCss) = Css.startPage();
  open Html;
  open Css;
  // let isMarkdown = Filename.check_suffix(fileName, ".md");

  let main = AboutMe.bodyWithSmallAboutMeColumn;
  let body = <main css toc=(
    <div className=css([A("padding", "0 16px"), A("flex-shrink", "1"), A("overflow", "auto")])>
      <div className=css([])>
        "Recent posts"
      </div>
      (Shared.hspace(8))
      (List.mapi(
        (i, {config: {Types.title, date: (year, month, day), fileName}}) => {
          let href = ("/" ++ Filename.chop_extension(fileName) ++ "/");
          i < 5 ? <a
            href
            className=css([
            A("font-size", "16px"),
            A("margin-bottom", "16px"),
            A("display", "block"),
            A("line-height", "20px"),
            ...Shared.Styles.subtleLink
          ])>
            <div>
            title
            </div>
            <div className=css([
              A("color", Shared.Colors.lightText),
              ...Shared.Styles.row
            ])>
            (string_of_int(year))
            (Shared.monthName(month))
            (string_of_int(day))
            </div>
          </a> : ""
        },
        posts
      ) |> String.concat("\n"))
      (Shared.hspace(32))
    </div>
  )>

    <h1 className=css(Shared.Styles.titleWithTopMargin)>contentTitle</h1>
    <postAbout css date tags />
    (Shared.hspace(32))
    <div className=css(Shared.Styles.bodyText)>
      (renderBody(postBody))
    </div>
  </main>;


  <html>
    <pageHead
      title=contentTitle
      ?description
      ?thumbnail
      ?article_image
    >
      <style>(inlineCss())</style>
    </pageHead>

    body
  </html>
};

let postList = (posts, tags, contentTitle) => {
  open Html;
  let (css, inlineCss) = Css.startPage();
  /* let contentTitle = "All posts"; */
  let main = AboutMe.bodyWithSmallAboutMeColumn;

  let body = <main css toc=(
    <div className=css([
      A("font-size", "16px"),
      A("line-height", "20px"),
      A("margin-bottom", "32px"),
    ])>
      (List.map(
        ((tag, count)) => {
          open Types;
            <a href=("/tags/" ++ tag ++ "/") className=css([
                A("color", "currentColor"),
                A("white-space", "nowrap"),
                A("margin-right", "8px"),
                ...Shared.Styles.hoverUnderline
              ])
            >
              tag ("" ++ string_of_int(count) ++ "")
            </a>
            /* <postAbout css date=config.date tags=config.tags withPic=false />
            <a className=css([A("font-size", "24px")]) href>readTime</a> */
        },
        tags
      ) |> String.concat("\n"))
    </div>
  )>
    <div className=css([A("flex", "3"), A("padding", "32px")])>
      <div className=css([A("padding", "1px"), A("position", "relative")])>
        <h1 className=css([
          A("text-align", "center"),
          ...Shared.Styles.title
        ])>contentTitle</h1>
      </div>
      (List.map(
        ({config, intro: teaser}) => {
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
              (renderBody(teaser))
                // (Omd.to_html(Omd.of_string(teaser)))
              </div>
            })
            <a className=css([A("font-size", "24px")]) href>readTime</a>
          </div>
        },
        posts
      ) |> String.concat("\n<div style='height: 32px'></div>\n"))
      </div>
  </main>;

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
  article_image: None,
  draft: false,
  featured: false,
  wordCount: None
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
  let config = check(Toml.string("thumbnail", opts), config, thumbnail => {...config, thumbnail: Some(thumbnail)});
  let config = check(Toml.string("article_image", opts), config, article_image => {...config, article_image: Some(article_image)});
  let config = check(Toml.bool("draft", opts), config, draft => {...config, draft});
  config
};

let getIntro = body => switch (Str.split(Str.regexp("<!-- more -->"), body)) {
| [] => assert(false)
| [one] => None
| [top, ...rest] => Some(top)
};

let (|?>>) = (v, f) => switch v {
  | Some(v) => Some(f(v))
  | None => None
};

let parse = (fileName, opts, content) => {
  let opts = opts |! "No options for post " ++ fileName;
  let config = parseConfig(fileName, opts);
  let intro = getIntro(content);
  let isMarkdown = Filename.check_suffix(fileName, ".md");
  let wordCount = isMarkdown ? Some(Str.split(Str.regexp("[^a-zA-Z0-9-]+"), content) |> List.length) : None;
  let config = {...config, wordCount};

  isMarkdown ? {config, intro: intro |?>> x => Markdown(x), body: Markdown(content)} : {intro: intro |?>> intro => Html(intro), config, body: Html(content)}
};

let parseJsonConfig = (config, json) => {
  open NotableMind.Json.Infix;
  module Get = NotableMind.Json.Get;
  let config = check(Get.string("title", json), config, title => {...config, title});
  let config = check(Get.string("description", json), config, description => {...config, description: Some(description)});
  let config = check(Get.stringList("tags", json), config, tags => {...config, tags});
  let config = check(Get.stringList("categories", json), config, categories => {...config, categories});
  let config = check(Get.string("date", json), config, date => {...config, date: parseDate(date)});
  let config = check(Get.bool("featured", json), config, featured => {...config, featured});
  let config = check(Get.string("thumbnail", json), config, thumbnail => {...config, thumbnail: Some(thumbnail)});
  let config = check(Get.string("article_image", json), config, article_image => {...config, article_image: Some(article_image)});
  let config = check(Get.bool("draft", json), config, draft => {...config, draft});
  config
}

let parseNm = (fileName, content) => {
  let nodes = NotableMind.parse(content);
  let config = defaultConfig(fileName);
  let (nodes, config) =
    switch (nodes) {
    | [
        {children: [{typ: "note", content: attrs}, ...otherChildren], content},
      ] => (
        otherChildren,
        parseJsonConfig({...config, title: content}, Yojson.Basic.from_string(attrs)),
      )
    | _ => (nodes, config)
    };
  {config, intro: None, body: Nm(nodes)};
};
