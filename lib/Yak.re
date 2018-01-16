
let page = (~title as name, ~description, ~body as bodyText, ~extraHead="", ()) => {
  open Html;
  <html>
    <head>
      <meta charset="utf8"/>
      <title>{name}</title>
      {extraHead}
    </head>
    <body>
      {bodyText}
    </body>
  </html>
};

/* gotta parse toml */

let unwrap = (message, x) => switch x { | None => failwith(message) | Some(x) => x };

type config = {
  title: string,
  tags: list(string),
  categories: list(string),
  date: string,
  description: option(string),
  thumbnail: option(string),
  featured: bool,
  fileName: string
};

let defaultConfig = fileName => {
  fileName,
  title: "JaredForsyth.com",
  tags: [],
  categories: [],
  date: "",
  description: None,
  thumbnail: None,
  featured: false
};

let parseConfig = (fileName, text) => {
  let opts = Toml.parse(text);
  let config = defaultConfig(fileName);
  let config = switch (Toml.string("title", opts)) {
  | None => config
  | Some(title) => {...config, title}
  };
  let config = switch (Toml.string("description", opts)) {
  | None => config
  | Some(description) => {...config, description: Some(description)}
  };
  let config = switch (Toml.stringList("tags", opts)) {
  | None => config
  | Some(tags) => {...config, tags}
  };
  let config = switch (Toml.stringList("categories", opts)) {
  | None => config
  | Some(categories) => {...config, categories}
  };
  let config = switch (Toml.string("date", opts)) {
  | None => config
  | Some(date) => {...config, date}
  };
  let config = switch (Toml.bool("featured", opts)) {
  | None => config
  | Some(featured) => {...config, featured}
  };
  config
};

let parseConfig = (fileName, raw) => {
  let divider = "\n---\n";
  switch (Str.split(Str.regexp(divider), raw)) {
  | [] => assert(false)
  | [single] => (defaultConfig(fileName), single)
  | [top, ...rest] => (parseConfig(fileName, top), String.concat(divider, rest))
  };
};

let run = () => {
  print_endline("Hello " ++ Unix.getcwd());
  let base = "./test/posts";
  Files.readDirectory(base)
  |> List.filter(f => Filename.check_suffix(f, ".md"))
  |> List.map(fileName => {
    let contents = Files.readFile(Filename.concat(base, fileName)) |> unwrap("Cannot read file");
    let (config, body) = parseConfig(fileName, raw);
    print_endline(config.title)
  }) |> ignore
  ;
  let json = Yojson.Basic.from_string("{\"2\":3}");
  let x = Yojson.Basic.to_string(json);
  print_endline(x);
  let md = Omd.of_string("Hello");
  let html = Omd.to_html(~override=?None, ~pindent=false, ~nl2br=true, ~cs=?None, md);

};
