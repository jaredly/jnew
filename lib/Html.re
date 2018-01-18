
let props = "";

let formatAttributes = (attrs) => String.concat("", List.map(((name, v)) => " " ++ name ++ "=\"" ++ String.escaped(v) ++ "\"", attrs));

let selfClosing = tag => switch tag {
| "meta" => true
| "img" => true
| _ => false
};

let normal = (name, preattrs, ~attrs=[], ~children, ()) => {
  let head = name ++ formatAttributes(preattrs @ attrs);
  if (children == []) {
    "<" ++ head ++ (selfClosing(name) ? "/>" : "></" ++ name ++ ">")
  } else {
    "<" ++ head ++ ">\n" ++ String.concat("\n", children) ++ "\n</" ++ name ++ ">"
  }
};

let filterPresentValues = List.fold_left(
  (res, (name, value)) => {
    switch value {
    | None => res
    | Some(thing) => [(name, thing), ...res]
    }
  },
  []
);

let filterPresentValues = vals => List.rev(filterPresentValues(vals));

let contentTag = (tagName, preattrs,
  ~id=?,
  ~lang=?,
  ~className=?,
  ~style=?,
  ~itemprop=?,
  ~itemtype=?,
  ~itemscope=?,
  ~name=?,
  ~role=?
) => normal(tagName, (preattrs @ [
  ("class", className),
  ("id", id),
  ("lang", lang),
  ("style", style),
  ("itemprop", itemprop),
  ("itemtype", itemtype),
  ("itemscope", itemscope),
  ("role", role),
  ("name", name),
  /* ("") */
]) |> filterPresentValues);

let meta = (~name, ~content) => normal("meta", [("name", name), ("content", content)]);
let pmeta = (~property, ~content) => normal("meta", [("property", property), ("content", content)]);
let link = (~rel, ~href, ~media=?) => normal("link", [("rel", rel), ("href", href)]);
let module Meta = {
  let charset = (name) => "<meta charset=\"" ++ String.escaped(name) ++ "\">";
  let mobile = "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"/>";
  let description = (description) => "<meta name=\"description\" content=\"" ++ description ++ "\">";
};
let html = (~attrs=[], ~children, ()) => "<!DOCTYPE HTML>\n" ++ normal("html", [], ~attrs, ~children, ());

let fragment = (~children, ()) => String.concat("\n", children);

let time = (~datetime) => contentTag("time", [("datetime", Some(datetime))]);
let body = contentTag("body", []);
let section = contentTag("section", []);
let article = contentTag("article", []);
let main = contentTag("main", []);
let header = contentTag("header", []);
let footer = contentTag("footer", []);
let h1 = contentTag("h1", []);
let h2 = contentTag("h2", []);
let h3 = contentTag("h3", []);
let h4 = contentTag("h4", []);
let div = contentTag("div", []);
let span = contentTag("span", []);
let a = (~href, ~target=?) => contentTag("a", [("href", Some(href)), ("target", target)]);

let script = (~src) => normal("script", [("src", src)]);
let escapeScript = str => str; /* TODO impl */
let inlineScript = (~children, ()) => normal("script",  [], ~children=List.map(escapeScript, children), ());
let style = normal("style", []);

let head = normal("head", []);
let title = normal("title", []);


let startPage = () => {
  let ids = Hashtbl.create(50);

  let css = (items) => {
    let hash = Hashtbl.hash(items);
    Hashtbl.add(ids, hash, items);
    "style-" ++ string_of_int(hash);
  };

  let inlineCss = () => {
    Hashtbl.fold(
      (k, v, res) => [".style-" ++ string_of_int(k) ++ " {\n" ++ String.concat(
        "\n",
        List.map(((attr, value)) => "  " ++ attr ++ ": " ++ value ++ ";", v)
      ) ++ "\n}", ...res],
      ids,
      []
    ) |> String.concat("\n")
  };

  (css, inlineCss)
};
