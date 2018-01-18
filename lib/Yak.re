
let unwrap = (message, x) => switch x { | None => failwith(message) | Some(x) => x };

open Types;

let defaultConfig = fileName => {
  fileName,
  title: "JaredForsyth.com",
  tags: [],
  categories: [],
  date: (0, 0, 0), /* Year, Month, Day */
  description: None,
  thumbnail: None,
  featured: false
};

let check = (opt, base, fn) => switch opt {
| None => base
| Some(value) => fn(value)
};

let parseDate = text => {
  Scanf.sscanf(text, "%d-%d-%d", (year, month, day) => (year, month, day))
};

let parseConfig = (fileName, text) => {
  let opts = Toml.parse(text);
  let config = defaultConfig(fileName);
  let config = check(Toml.string("title", opts), config, title => {...config, title});
  let config = check(Toml.string("description", opts), config, description => {...config, description: Some(description)});
  let config = check(Toml.stringList("tags", opts), config, tags => {...config, tags});
  let config = check(Toml.stringList("categories", opts), config, categories => {...config, categories});
  let config = check(Toml.string("date", opts), config, date => {...config, date: parseDate(date)});
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

let getIntro = body => switch (Str.split(Str.regexp("\n<!-- more -->\n"), body)) {
| [] => assert(false)
| [one] => None
| [top, ...rest] => Some(top)
};

let dateSort = ((y1, m1, d1), (y2, m2, d2)) => {
  switch (y1 - y2) {
  | 0 => switch (m1 - m2) {
    | 0 => d1 - d2
    | x => x
    }
  | x => x
  }
};

let run = () => {
  print_endline("Hello " ++ Unix.getcwd());
  let base = "./test/posts";
  let posts = Files.readDirectory(base)
  |> List.filter(f => Filename.check_suffix(f, ".md"))
  |> List.map(fileName => {
    let contents = Files.readFile(Filename.concat(base, fileName)) |> unwrap("Cannot read file");
    let (config, body) = parseConfig(fileName, contents);
    let intro = getIntro(body);
    let dest = Filename.concat("./test/pages/", Filename.chop_extension(fileName) ++ ".html");
    let html = Post.renderPost(
      ~title=config.title,
      ~date=config.date,
      ~tags=config.tags,
      ~description=config.description,
      ~thumbnail=config.thumbnail,
      body
    );
    Files.writeFile(dest, html) |> ignore;
    (config, intro, body)
  });
  print_endline("Done!");
  let sorted = List.sort((({date: date1}, _, _), ({date: date2}, _, _)) => dateSort(date2, date1), posts);
  /* All posts list */
  let html = Post.postList(sorted);
  Files.writeFile("./test/pages/index.html", html) |> ignore;
  /* First page I think */
};
