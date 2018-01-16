
let props = "";

let formatAttributes = (attrs) => String.concat("", List.map(((name, v)) => " " ++ name ++ "\"" ++ String.escaped(v) ++ "\"", attrs));

let normal = (name, preattrs, ~attrs=[], ~children, ()) => {
  "<" ++ name ++ formatAttributes(preattrs @ attrs) ++ ">\n" ++ String.concat("\n", children) ++ "\n</" ++ name ++ ">"
};

let meta = (~charset="") => normal("meta", [("charset", charset)]);
let html = normal("html", []);
let body = normal("body", []);

let script = (~src) => normal("script", [("src", src)]);
let escapeScript = str => str; /* TODO impl */
let inlineScript = (~children, ()) => normal("script",  [], ~children=List.map(escapeScript, children), ());

let head = normal("head", []);
let title = normal("title", [])
