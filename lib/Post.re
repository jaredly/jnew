
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

let showDate = (~date as (year, month, _), ~children, ()) => {
  open Html;
  <fragment>
    (monthName(month))
    (spacer(4))
    (string_of_int(year))
  </fragment>
};

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
        <h1 className=css([
          A("font-size", "56px"),
          A("margin-top", "100px"),
          A("margin-bottom", "16px"),
          Media("max-width: 600px", [
            ("font-size", "32px"),
            ("margin-top", "40px"),
          ])
        ])>contentTitle</h1>
        <postAbout css date tags />
      </fragment>
    }
    middle=(
      <div
      className=css([
        A("font-size", "24px"),
        A("line-height", "36px"),
        A("hyphens", "auto"),
        Media("max-width: 600px", [
          ("font-size", "20px"),
          ("line-height", "30px"),
        ])
      ])
      >
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

let myBigFace = css => Css.(Html.(
  <a
    href="/about/"
    style="background-image: url(https://www.gravatar.com/avatar/313878fc8f316fc3fe4443b13913d0a4.png?s=200)"
    className=css([
      A("width", "120px"),
      A("height", "120px"),
      A("position", "absolute"),
      A("top", "-60px"),
      A("right", "50%"),
      A("margin-right", "-60px"),
      A("background-size", "cover"),
      A("border-radius", "60px"),
      A("z-index", "99"),
      A("text-indent", "-9999px"),
      A("border", "3px solid white"),
      A("background-color", "white"),
      A("box-shadow", "0 1px 1px rgba(0, 0, 0, 0.3)"),
    ])
  >"Jared Forsyth"</a>
));

let postList = (posts, contentTitle) => {
  open Html;
  let (css, inlineCss) = Css.startPage();
  /* let contentTitle = "All posts"; */
  let body = <pageWithTopAndBottom
    css
    backgroundImage="/images/trees.jpg"
    top=(
      <div className=css([A("padding", "1px"), A("position", "relative")])>
        (myBigFace(css))
        <h1 className=css([
          A("text-align", "center"),
          A("font-size", "56px"),
          A("margin-bottom", "16px"),
        ])>contentTitle</h1>
      </div>
    )
    middle=(
      List.map(
        ((config, teaser, _)) => {
          open Types;
          let href = ("/" ++ Filename.chop_extension(config.Types.fileName) ++ "/");
          let readTime = config.wordCount > 0
            ? string_of_int(config.wordCount / 225) ++ " minute read"
            : "Read more";
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
              <div className=css([
                A("hyphens", "auto"),
                A("padding-top", "16px"),
                A("font-size", "24px"),
                A("line-height", "36px"),
              ])>
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
