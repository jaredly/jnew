let (/+) = Filename.concat;

let run = (~excludeDrafts) => {
  let outputDir = "./test/pages/";
  let inputDir = "./test";

  let projects = Lib.Yak.processProjects(inputDir, outputDir);
  let posts = Lib.Yak.processBlog(~excludeDrafts, inputDir, outputDir);
  let talks = Lib.Yak.processTalks(inputDir, outputDir);

  Lib.Yak.setupRedirectsForOldPosts(outputDir, posts);

  /* Home page */
  Lib.Files.writeFile(
    outputDir /+ "index.html",
    Lib.Home.render(~projects, ~posts, ~talks),
  )
  |> ignore;

  Lib.MarkdownParser.saveTwitterCache();
  print_endline("Finished!");
};

print_endline(String.concat("; ", Array.to_list(Sys.argv)));

switch (Sys.argv) {
| [|_, "drafts"|] => run(~excludeDrafts=false)
| _ => run(~excludeDrafts=true)
};