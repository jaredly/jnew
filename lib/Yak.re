let unwrap = (message, x) =>
  switch (x) {
  | None => failwith(message)
  | Some(x) => x
  };
let (/+) = Filename.concat;

open Shared.Infix;
open Types;

let sortPostsByDate =
    ({Post.config: {date: date1}}, {Post.config: {date: date2}}) =>
  Shared.dateSort(date2, date1);

let sortProjectsByDate = ({Project.updates}, {Project.updates: updates2}) =>
  switch (updates, updates2) {
  | ([(date1, _, _), ..._], [(date2, _, _), ..._]) =>
    Shared.dateSort(date2, date1)
  | ([], []) => 0
  | ([], _) => (-1)
  | (_, []) => 1
  };

let splitTopYaml = text => {
  let divider = "\n---\n";
  switch (Str.split(Str.regexp(divider), text)) {
  | [] => assert(false)
  | [single] => (None, single)
  | [top, ...rest] => (
      Some(Toml.parse(top)),
      String.concat(divider, rest),
    )
  };
};

let collectPages = (inputDir, outputDir, baseDir, parse) => {
  let base = inputDir /+ baseDir;
  Files.readDirectory(base)
  |> List.filter(f =>
       Filename.check_suffix(f, ".md")
       || Filename.check_suffix(f, ".html")
       || Filename.check_suffix(f, ".nm.txt")
     )
  |> List.map(fileName => {
       let fullName = base /+ fileName;
       let contents = Files.readFile(fullName) |> unwrap("Cannot read file");
       let result = parse(baseDir /+ fileName, contents);
       let dest = outputDir /+ baseDir /+ Util.chopSuffix(fileName);
       Files.mkdirp(dest);
       let fullDest = dest /+ "index.html";
       (fullDest, result);
     });
};

let renderPages = (render, pages) => {
  List.map(
    ((dest, config)) => {
      let html = render(config);
      Files.writeFile(dest, html)
      |> Util.expectTrue("Failed to write file " ++ dest);
      config;
    },
    pages,
  );
};

let assembleTags = posts => {
  posts
  |> List.fold_left(
       (byTag, {Post.config, intro, body}) => {
         let byTag =
           List.fold_left(
             (byTag, tag) =>
               StrMap.add(
                 tag,
                 switch (StrMap.find(tag, byTag)) {
                 | exception Not_found => [{config, intro, body}]
                 | items => [{Post.config, intro, body}, ...items]
                 },
                 byTag,
               ),
             byTag,
             config.tags,
           );
         byTag;
       },
       StrMap.empty,
     );
};

let assembleProjectTags = projects => {
  projects
  |> List.fold_left(
       (byTag, config) => {
         let byTag =
           List.fold_left(
             (byTag, tag) =>
               StrMap.add(
                 tag,
                 switch (StrMap.find(tag, byTag)) {
                 | exception Not_found => [config]
                 | items => [config, ...items]
                 },
                 byTag,
               ),
             byTag,
             config.Project.tags,
           );
         byTag;
       },
       StrMap.empty,
     );
};

let processProjects = (inputDir, outputDir) => {
  /* Project pages */
  let projects =
    collectPages(
      inputDir,
      outputDir,
      "projects",
      (fileName, contents) => {
        let (opts, body) = splitTopYaml(contents);
        Project.parse(fileName, opts, body);
      },
    )
    |> renderPages(Project.render)
    |> List.sort(sortProjectsByDate);
  let projectTags = assembleProjectTags(projects);
  let projectTagCounts =
    StrMap.fold(
      (key, value, res) => [(key, List.length(value)), ...res],
      projectTags,
      [],
    )
    |> List.sort(((k, n), (v, n2)) => n2 - n);

  let html = Project.renderList(projectTagCounts, projects, "All projects");
  Files.writeFile(outputDir /+ "projects/index.html", html) |> ignore;

  StrMap.iter(
    (tag, posts) => {
      let dest = outputDir /+ "projects/tags" /+ tag;
      Files.mkdirp(dest);
      let html =
        Project.renderList(
          projectTagCounts,
          List.sort(sortProjectsByDate, posts),
          "Tag: " ++ tag,
        );
      Files.writeFile(dest /+ "index.html", html) |> ignore;
    },
    projectTags,
  );

  projects;
};

let processBlog = (~excludeDrafts=true, inputDir, outputDir) => {
  /* Posts */
  let fullPosts =
    collectPages(inputDir, outputDir, "posts", (fileName, contents) =>
      if (Filename.check_suffix(fileName, ".nm.txt")) {
        Post.parseNm(fileName, contents);
      } else {
        let (opts, body) = splitTopYaml(contents);
        Post.parse(fileName, opts, body);
      }
    );
  let posts = List.map(snd, fullPosts) |> List.sort(sortPostsByDate);
  let posts =
    excludeDrafts
      ? posts |> List.filter(post => !post.Post.config.draft) : posts;
  renderPages(Post.render(posts), fullPosts) |> ignore;

  let tags = assembleTags(posts);
  let tagCounts =
    StrMap.fold(
      (key, value, res) => [(key, List.length(value)), ...res],
      tags,
      [],
    )
    |> List.sort(((k, n), (v, n2)) => n2 - n);

  let (html, rss) =
    Post.postList(
      ~urlBase="https://jaredforsyth.com",
      posts,
      tagCounts,
      "All posts",
    );
  Files.writeFile(outputDir /+ "posts/index.html", html) |> ignore;

  Files.writeFile(outputDir /+ "posts/rss.xml", rss) |> ignore;

  StrMap.iter(
    (tag, posts) => {
      let dest = outputDir /+ "tags" /+ tag;
      Files.mkdirp(dest);
      let (html, rss) =
        Post.postList(
          ~urlBase="https://jaredforsyth.com",
          List.sort(sortPostsByDate, posts),
          tagCounts,
          "Tag: " ++ tag,
        );
      Files.writeFile(dest /+ "index.html", html) |> ignore;
      Files.writeFile(dest /+ "rss.xml", rss) |> ignore;
    },
    tags,
  );

  posts;
};

let processTalks = (inputDir, outputDir) => {
  /* Talks */
  let talks = Talk.collect(inputDir /+ "talks.json");
  let html = Talk.renderList(talks, "Talks");
  Files.mkdirp(outputDir /+ "talks/");
  Files.writeFile(outputDir /+ "talks/index.html", html)
  |> Util.expectTrue("Unable to write talks page");
  talks;
};

let redirect = (url, contentTitle, description) => {
  open Html;
  let metaHead = Shared.metaHead;

  <html>
    <head>
      <metaHead title=contentTitle description />
      {{|<meta http-equiv="refresh" content="0; URL='|} ++ url ++ {|'" />|}}
    </head>
    <body> {"This page has moved to " ++ url} </body>
  </html>;
};

let setupRedirectsForOldPosts = (outputDir, posts) => {
  let oldPosts =
    posts
    |> List.filter(({Post.config: {Types.date}}) =>
         Shared.dateSort(date, (2018, 1, 14)) < 0
       );
  oldPosts
  |> List.iter(
       (
         {
           Post.config: {
             Types.title,
             description,
             fileName,
             date: (year, month, day),
           },
         },
       ) => {
       let slug = Filename.basename(fileName) |> Util.chopSuffix;
       let path =
         Printf.sprintf("%04d/%02d/%02d/%s/", year, month, day, slug);
       let fullPath = outputDir /+ path;
       let realPath = "/posts" /+ slug;
       /* print_endline(fullPath); */
       Files.mkdirp(fullPath);
       Files.writeFile(
         fullPath /+ "index.html",
         redirect(
           realPath,
           title,
           description |? "Thoughts about programming, mostly",
         ),
       )
       |> Util.expectTrue("Unable to write redirect " ++ fullPath);
     });
};