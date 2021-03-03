
let run = (excludeDrafts: boolean) => {
  let outputDir = "./test/pages/";
  let inputDir = "./test";

  let projects = processProjects(inputDir, outputDir);
  let posts = processBlog(excludeDrafts, inputDir, outputDir);
  let talks = processTalks(inputDir, outputDir);

  setupRedirectsForOldPosts(outputDir, posts);

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

const [_, drafts] = process.argv
run(drafts !== 'drafts')