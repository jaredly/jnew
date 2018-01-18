
let monthName = month => switch month {
| 1 => "January"
| 2 => "February"
| 3 => "March"
| 4 => "April"
| 5 => "May"
| 6 => "June"
| 7 => "July"
| 8 => "August"
| 9 => "September"
| 10 => "October"
| 11 => "November"
| 12 => "December"
| _ => failwith("Invalid month")
};

let spacer = Shared.spacer;
let userPic = Shared.userPic;

let postAbout = (~css, ~date as (year, month, day), ~tags, ~children, ()) => {
  open Html;
  <div className=css([
    ("color", Shared.Colors.lightText),
    ("font-family", "Open sans"),
    ("font-size", "14px"),
    ("display", "flex"),
    ("flex-direction", "row"),
    ("align-items", "center"),
    ("justify-content", "flex-start"),
  ])>
    <userPic css />
    (spacer(12))
    <a href="/about" className=css([
      ("color", "currentColor"),
      ("text-decoration", "none"),
      ("font-weight", "bold")
    ])>"Jared Forsyth"</a>
    (spacer(4))
    " · "
    (spacer(4))
    (monthName(month))
    (spacer(4))
    (string_of_int(year))
    (spacer(4))
    " · "
    (spacer(4))
    " tagged "
    (spacer(4))
    (String.concat(", " ++ spacer(4), List.map(tag => <a href=("/tags/" ++ tag)>tag</a>, tags)))
  </div>
};

let pageWithTopAndBottom = Shared.pageWithTopAndBottom;
let pageHead = Shared.pageHead;

let renderPost = (~title as contentTitle, ~description, ~date, ~tags, ~thumbnail, rawBody) => {
  let (css, inlineCss) = Html.startPage();
  open Html;

  let body = <pageWithTopAndBottom
      css
      top={
        <fragment>
          <a
            href="/"
            className=css([
              ("position", "fixed"),
              ("top", "10px"),
              ("left", "10px"),
              ("padding", "8px"),
              ("background-color", "black")
            ])
          >
            <div className=css([
              ("height", "32px"),
              ("width", "32px"),
              ("background-size", "cover")
            ]) style="background-image: url(/images/logo/JF_64.png)" />
          </a>
          <h1 className=css([
            ("font-size", "56px"),
            ("margin-top", "100px"),
            ("margin-bottom", "16px"),
          ])>contentTitle</h1>
          <postAbout css date tags />
        </fragment>
      }
      middle=(
        <div
        className=css([
          ("font-size", "24px"),
          ("line-height", "36px"),
          ("hyphens", "auto"),
        ])
        >
          (MarkdownParser.parse(rawBody))
          /* (Omd.to_html(Omd.of_string(rawBody))) */
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

let postList = (posts) => {
  open Html;
  let (css, inlineCss) = Html.startPage();
  let contentTitle = "All posts";
  let body = <pageWithTopAndBottom
    css
    top=(
      <fragment>
        <h1 className=css([
          ("font-size", "56px"),
          ("margin-top", "150px"),
          ("margin-bottom", "16px"),
        ])>contentTitle</h1>
      </fragment>
    )
    middle=(
      List.map(
        ((config, teaser, _)) => {
          <div>
            <a href=(Filename.chop_extension(config.Types.fileName) ++ "/")
            >
            <h2>
              (config.Types.title)
            </h2>
            </a>
            (switch teaser {
            | None => ""
            | Some(teaser) =>
              <div className=css([
                ("hyphens", "auto")
              ])>
                (Omd.to_html(Omd.of_string(teaser)))
              </div>
            })
          </div>
        },
        posts
      ) |> String.concat("\n")
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
