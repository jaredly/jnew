
let unwrap = (message, x) => switch x { | None => failwith(message) | Some(x) => x };

let (|?) = (x, y) => switch x { | None => y | Some(x) => x };
let (|?>) = (x, f) => switch x { | None => None | Some(x) => f(x) };
let (|?>>) = (x, f) => switch x { | None => None | Some(x) => Some(f(x)) };
let (|!) = (x, y) => switch x { | None => failwith(y) | Some(x) => x };

type venue = {
  where: string,
  video: option(string),
  date: option((int, int, int))
};

type talk = {
  title: string,
  image: option(string),
  slides: option(string),
  tweets: list(string),
  venues: list(venue)
};

let get = (name, items) => switch (List.assoc(name, items)) {
| exception Not_found => None
| v => Some(v)
};

let jsonString = item => switch item {
| `String(v) => Some(v)
| _ => None
};

let jsonArray = item => switch item {
| `List(v) => Some(v)
| _ => None
};

let parseVenue = item => switch item {
| `Assoc(items) => {
  let where = get("where", items) |?> jsonString |! "Where is required for a venue";
  let date = get("date", items) |?> jsonString |?>> Post.parseDate;
  let video = get("video", items) |?> jsonString;
  {where, date, video}
}
| _ => failwith("Bad venue json")
};

let collect = fname => {
  switch (Files.readFile(fname)) {
  | None => []
  | Some(text) => switch (Yojson.Basic.from_string(~fname, text)) {
    | exception _ => {
      failwith("Failed to parse talks json " ++ fname);
    }
    | `List(talks) => {
      List.map(
        talk => switch talk {
          | `Assoc(items) => {
            let title = get("title", items) |?> jsonString |! "Title required for talk";
            let slides = get("slides", items) |?> jsonString;
            let image = get("image", items) |?> jsonString;
            let tweets = get("tweets", items) |?> jsonArray |?>> List.map(jsonString) |?>> List.map(unwrap("Tweet must be a string")) |? [];
            let venues = get("venues", items) |?> jsonArray |?>> List.map(parseVenue) |? [];
            {title, image, slides, venues, tweets}
          }
          | _ => failwith("Bad talk json, not an assoc")
        },
        talks
      )
    }
    | _ => {
      failwith("Unexpected json in talks file " ++ fname);
    }
  }
  }
};

let renderList = (talks, contentTitle) => {
  open Html;
  open Css;
  let (css, inlineCss) = Css.startPage();
  let main = AboutMe.bodyWithSmallAboutMeColumn;
  let pageHead = Shared.pageHead;
  let body = <main
    css
    toc=(
      <div className=css([
        /* A("font-size", "16px"),
        A("line-height", "20px"), */
        A("margin-bottom", "32px"),
      ])>
        "If you want to speak at conferences, contact me on twitter and I'd be happy to give you guidance on applying."
      </div>
    )>

      <div className=css([A("padding", "1px"), A("position", "relative")])>
        <h1 className=css([
          A("text-align", "center"),
          ...Shared.Styles.title
        ])>contentTitle</h1>
      </div>
      (Shared.hspace(64))

      (List.map(
        ({title, image, slides, venues}) => {
          open Shared;
          let main = switch image {
            | None =>
              <div className=css([A("font-size", px(Consts.statusSize))])> title </div>
            | Some(src) =>
              <img alt=title src className=css([A("max-height", "300px"), ...Styles.basicImage])/>
          };
          <div>
            (switch slides {
            | None => main
            | Some(href) =>
              <a href target="_blank" className=css(Shared.Styles.subtleLink)>
                main
              </a>
            })
            (Shared.hspace(Consts.smallSpace))
            <ul className=css([
              A("font-size", px(Consts.titleSize)),
              A("padding", "0"),
              /* A("padding-left", "32px"), */
              A("list-style", "none"),
              A("margin", "0")
            ])>
            (List.map(
              ({where, date, video}) => {
                <li>
                (switch video {
                | None => where
                | Some(href) => <a href className=css([
                  A("font-family", "Open sans, sans-serif"),
                  A("font-size", Shared.px(Consts.titleSize)),
                  ...Styles.hoverUnderline
                  ])>where "📹"</a>
                })
                </li>
              },
              venues
            ) |> String.concat("\n"))
            </ul>
          </div>
        },
        talks
      ) |> String.concat("\n" ++ Shared.hspace(Shared.Consts.bigSpace + Shared.Consts.smallSpace)))

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
