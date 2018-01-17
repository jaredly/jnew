
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

let check = (opt, base, fn) => switch opt {
| None => base
| Some(value) => fn(value)
};

let parseConfig = (fileName, text) => {
  let opts = Toml.parse(text);
  let config = defaultConfig(fileName);
  let config = check(Toml.string("title", opts), config, title => {...config, title});
  let config = check(Toml.string("description", opts), config, description => {...config, description: Some(description)});
  let config = check(Toml.stringList("tags", opts), config, tags => {...config, tags});
  let config = check(Toml.stringList("categories", opts), config, categories => {...config, categories});
  let config = check(Toml.string("date", opts), config, date => {...config, date});
  let config = check(Toml.bool("featured", opts), config, featured => {...config, featured});
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
  |> List.iter(fileName => {
    let contents = Files.readFile(Filename.concat(base, fileName)) |> unwrap("Cannot read file");
    let (config, body) = parseConfig(fileName, contents);
    let dest = Filename.concat("./test/pages/", Filename.chop_extension(fileName) ++ ".html");
    let html = Post.renderPost(
      ~title=config.title,
      ~description=config.description,
      ~thumbnail=config.thumbnail,
      body
    );
    Files.writeFile(dest, html) |> ignore;
    print_endline(config.title)
  });

};
