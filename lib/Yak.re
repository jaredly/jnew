
let unwrap = (message, x) => switch x { | None => failwith(message) | Some(x) => x };

open Types;

let sortPostsByDate = (({date: date1}, _, _), ({date: date2}, _, _)) => Shared.dateSort(date2, date1);

let sortProjectsByDate = ({Project.updates}, {Project.updates: updates2}) => switch (updates, updates2) {
| ([(date1, _, _), ..._], [(date2, _, _), ..._]) => Shared.dateSort(date2, date1)
| ([], []) => 0
| ([], _) => -1
| (_, []) => 1
};

/*
 *
 * /
 *  - blog posts
 *    - listed by title / tags?
 *  - projects
 *    - screenshot + name
 *  - about me blurb
 *    - just text, with a photo
 *  - talks
 *    - maybe screenshots of the first slide?
 *    - with links to the video(s)
 *
 * /about
 *
 * /posts/ - the blog deal, same as my current front page
 * /posts/some-post/
 *
 * /tags/ - a listing of tags, by popularity maybe?
 * /tags/some-tag/
 *
 * /projects/ - a grid of screenshots probably
 * /projects/gravitron/
 *
 * /talks/ - a listing of the talks I've given
 * /talks/talk/ - probably the slides?
 *
 *
 * --- general ---
 * have a listing across the top of the site, no bar, just links
 * also maybe doesn't float? but it could maybe.
 * Links to [blog] [projects] [talks] [about me]
 */

let base = "./test/";

let splitTopYaml = (text) => {
  let divider = "\n---\n";
  switch (Str.split(Str.regexp(divider), text)) {
  | [] => assert(false)
  | [single] => (None, single)
  | [top, ...rest] => (Some(Toml.parse(top)), String.concat(divider, rest))
  };
};

let collectPages = (baseDir, render) => {
  let base = Filename.concat(base, baseDir);
  let posts = Files.readDirectory(base)
  |> List.filter(f => Filename.check_suffix(f, ".md")) /* TODO .html suffix too */
  |> List.map((fileName) => {
    let fullName = Filename.concat(base, fileName);
    let contents = Files.readFile(fullName) |> unwrap("Cannot read file");
    let (opts, body) = splitTopYaml(contents);
    /* print_endline(String.escaped(body)); */
    let dest = Filename.concat(Filename.concat("./test/pages/", baseDir), Filename.chop_extension(fileName));
    Files.mkdirp(dest);
    let (html, result) = render(Filename.concat(baseDir, fileName), opts, body);
    Files.writeFile(Filename.concat(dest, "index.html"), html) |> ignore;
    result
  });
  posts
};

let assembleTags = posts => {
  posts |> List.fold_left((byTag, (config, intro, body)) => {
    let byTag = List.fold_left(
      (byTag, tag) => StrMap.add(tag, switch (StrMap.find(tag, byTag)) {
      | exception Not_found => [(config, intro, body)]
      | items => [(config, intro, body), ...items]
      }, byTag),
      byTag,
      config.tags
    );
    byTag
  }, StrMap.empty);
};

let run = () => {
  /* Static pages */
  collectPages("static", Static.render) |> ignore;
  /* Project pages */
  let projects = collectPages("projects", Project.render) |> List.sort(sortProjectsByDate);
  let html = Project.renderList(projects, "All projects");
  Files.writeFile("./test/pages/projects/index.html", html) |> ignore;
  /* Posts */
  let posts = collectPages("posts", Post.render) |> List.sort(sortPostsByDate);
  let html = Post.postList(posts, "All posts");
  Files.writeFile("./test/pages/posts/index.html", html) |> ignore;

  let tags = assembleTags(posts);
  StrMap.iter((tag, posts) => {
    let dest = Filename.concat("./test/pages/tags", tag);
    Files.mkdirp(dest);
    let html = Post.postList(List.sort(sortPostsByDate, posts), "Tag: " ++ tag);
    Files.writeFile(Filename.concat(dest, "index.html"), html) |> ignore;
  }, tags);

  /* Talks */
  let talks = Talk.collect("./test/talks.json");

  /* Home page */
  Files.writeFile("./test/pages/index.html", Home.render(~projects, ~posts, ~tags, ~talks)) |> ignore;

  MarkdownParser.saveTwitterCache();
  print_endline("Finished!");
};


/* let run = () => {
  print_endline("Hello " ++ Unix.getcwd());
  let base = "./test/posts";
  let (posts, byTag) = Files.readDirectory(base)
  |> List.filter(f => Filename.check_suffix(f, ".md")) /* TODO .html suffix too */
  |> List.fold_left(((posts, byTag), fileName) => {
    let contents = Files.readFile(Filename.concat(base, fileName)) |> unwrap("Cannot read file");
    let (config, body) = parseConfig(fileName, contents);
    let intro = getIntro(body);
    let wordCount = Str.split(Str.regexp("[^a-zA-Z0-9-]"), body) |> List.length;
    let config = {...config, wordCount};
    let dest = Filename.concat("./test/pages/", Filename.chop_extension(fileName));
    Files.mkdirp(dest);
    let html = Post.renderPost(config, body);
    Files.writeFile(Filename.concat(dest, "index.html"), html) |> ignore;
    let byTag = List.fold_left(
      (byTag, tag) => StrMap.add(tag, switch (StrMap.find(tag, byTag)) {
      | exception Not_found => [(config, intro, body)]
      | items => [(config, intro, body), ...items]
      }, byTag),
      byTag,
      config.tags
    );
    ([(config, intro, body), ...posts], byTag)
  }, ([], StrMap.empty));
  print_endline("Done!");
  let sorted = List.sort(sortPostsByDate, posts);
  /* All posts list */
  let html = Post.postList(sorted, "All posts");
  Files.writeFile("./test/pages/index.html", html) |> ignore;
  StrMap.iter((tag, posts) => {
    print_endline("Tag: " ++ tag);
    let dest = Filename.concat("./test/pages/tags", tag);
    Files.mkdirp(dest);
    let html = Post.postList(List.sort(sortPostsByDate, posts), "Tag: " ++ tag);
    Files.writeFile(Filename.concat(dest, "index.html"), html) |> ignore;
  }, byTag);
  /* Front page I think */
}; */
