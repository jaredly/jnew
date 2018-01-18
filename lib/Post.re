
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

let renderPost = (~title as contentTitle, ~description, ~date, ~tags, ~thumbnail, rawBody) => {
  let (css, inlineCss) = Css.startPage();
  open Html;
  open Css;

  let body = <pageWithTopAndBottom
      css
      top={
        <fragment>
          <a
            href="/"
            className=css([
              A("position", "fixed"),
              A("top", "10px"),
              A("left", "10px"),
              A("padding", "8px"),
              A("background-color", "black"),
              Media("max-width: 600px", [
                ("display", "none"),
              ])
            ])
          >
            <div className=css([
              A("height", "32px"),
              A("width", "32px"),
              A("background-size", "cover")
            ]) style="background-image: url(/images/logo/JF_64.png)" />
          </a>
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
  let (css, inlineCss) = Css.startPage();
  let contentTitle = "All posts";
  let body = <pageWithTopAndBottom
    css
    top=(
      <fragment>
        <h1 className=css([
          A("font-size", "56px"),
          A("margin-top", "150px"),
          A("margin-bottom", "16px"),
        ])>contentTitle</h1>
      </fragment>
    )
    middle=(
      List.map(
        ((config, teaser, _)) => {
          open Types;
          let href = (Filename.chop_extension(config.Types.fileName) ++ "/");
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
