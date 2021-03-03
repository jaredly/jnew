import { mkdir, mkdirSync, readdirSync, readFileSync } from 'fs';
import { collectTalks } from './Talk';
import { chopSuffix } from './Util';
// import toml from 'toml';
import { parseProject, project } from './Project';
import { dateSort } from './Shared';
import { parseToml } from './Toml';

// let sortPostsByDate =
//     ({config: {date: date1}}, {config: {date: date2}}) =>
//   dateSort(date2, date1);

let sortProjectsByDate = (
    { updates }: project,
    { updates: updates2 }: project,
) =>
    updates.length && updates2.length
        ? dateSort(updates2[0][0], updates[0][0])
        : updates.length
        ? 1
        : -1;

let splitTopYaml = (text: string): [any | null, string] => {
    let divider = '\n---\n';
    const parts = text.split(divider);
    if (parts.length === 1) {
        return [null, parts[0]];
    }
    return [parseToml(parts[0]), parts.slice(1).join(divider)];
};

let collectPages = <V>(
    inputDir: string,
    outputDir: string,
    baseDir: string,
    parse: (name: string, text: string) => V,
): [string, V][] => {
    let base = inputDir + '/' + baseDir;
    return readdirSync(base)
        .filter(
            (f) =>
                f.endsWith('.md') ||
                f.endsWith('.html') ||
                f.endsWith('.nm.txt'),
        )
        .map((fileName) => {
            let fullName = base + '/' + fileName;
            let contents = readFileSync(fullName, 'utf8');
            let result = parse(baseDir + '/' + fileName, contents);
            let dest = outputDir + '/' + baseDir + '/' + chopSuffix(fileName);
            mkdirSync(dest, { recursive: true });
            let fullDest = dest + '/' + 'index.html';
            return [fullDest, result];
        });
};

// let renderPages = (render, pages) => {
//   List.map(
//     ((dest, config)) => {
//       let html = render(config);
//       Files.writeFile(dest, html)
//       |> Util.expectTrue("Failed to write file " ++ dest);
//       config;
//     },
//     pages,
//   );
// };

// let assembleTags = posts => {
//   posts
//   |> List.fold_left(
//        (byTag, {Post.config, intro, body}) => {
//          let byTag =
//            List.fold_left(
//              (byTag, tag) =>
//                StrMap.add(
//                  tag,
//                  switch (StrMap.find(tag, byTag)) {
//                  | exception Not_found => [{config, intro, body}]
//                  | items => [{Post.config, intro, body}, ...items]
//                  },
//                  byTag,
//                ),
//              byTag,
//              config.tags,
//            );
//          byTag;
//        },
//        StrMap.empty,
//      );
// };

// let assembleProjectTags = projects => {
//   projects
//   |> List.fold_left(
//        (byTag, config) => {
//          let byTag =
//            List.fold_left(
//              (byTag, tag) =>
//                StrMap.add(
//                  tag,
//                  switch (StrMap.find(tag, byTag)) {
//                  | exception Not_found => [config]
//                  | items => [config, ...items]
//                  },
//                  byTag,
//                ),
//              byTag,
//              config.Project.tags,
//            );
//          byTag;
//        },
//        StrMap.empty,
//      );
// };

export let processProjects = (inputDir: string, outputDir: string) => {
    /* Project pages */
    let projects = collectPages(
        inputDir,
        outputDir,
        'projects',
        (fileName, contents) => {
            let [opts, body] = splitTopYaml(contents);
            return parseProject(fileName, opts, body);
        },
    )
        // |> renderPages(Project.render)
        .map((m) => m[1])
        .sort(sortProjectsByDate);
    // let projectTags = assembleProjectTags(projects);
    // let projectTagCounts =
    //   StrMap.fold(
    //     (key, value, res) => [(key, List.length(value)), ...res],
    //     projectTags,
    //     [],
    //   )
    //   |> List.sort(((k, n), (v, n2)) => n2 - n);

    // let html = Project.renderList(projectTagCounts, projects, "All projects");
    // Files.writeFile(outputDir /+ "projects/index.html", html) |> ignore;

    // StrMap.iter(
    //   (tag, posts) => {
    //     let dest = outputDir /+ "projects/tags" /+ tag;
    //     Files.mkdirp(dest);
    //     let html =
    //       Project.renderList(
    //         projectTagCounts,
    //         List.sort(sortProjectsByDate, posts),
    //         "Tag: " ++ tag,
    //       );
    //     Files.writeFile(dest /+ "index.html", html) |> ignore;
    //   },
    //   projectTags,
    // );

    return projects;
};

// let processBlog = (~excludeDrafts=true, inputDir, outputDir) => {
//   /* Posts */
//   let fullPosts =
//     collectPages(inputDir, outputDir, "posts", (fileName, contents) =>
//       if (Filename.check_suffix(fileName, ".nm.txt")) {
//         Post.parseNm(fileName, contents);
//       } else {
//         let (opts, body) = splitTopYaml(contents);
//         Post.parse(fileName, opts, body);
//       }
//     );
//   let posts = List.map(snd, fullPosts) |> List.sort(sortPostsByDate);
//   let posts =
//     excludeDrafts
//       ? posts |> List.filter(post => !post.Post.config.draft) : posts;
//   renderPages(Post.render(posts), fullPosts) |> ignore;

//   let tags = assembleTags(posts);
//   let tagCounts =
//     StrMap.fold(
//       (key, value, res) => [(key, List.length(value)), ...res],
//       tags,
//       [],
//     )
//     |> List.sort(((k, n), (v, n2)) => n2 - n);

//   let (html, rss) =
//     Post.postList(
//       ~urlBase="https://jaredforsyth.com",
//       posts,
//       tagCounts,
//       "All posts",
//     );
//   Files.writeFile(outputDir /+ "posts/index.html", html) |> ignore;

//   Files.writeFile(outputDir /+ "posts/rss.xml", rss) |> ignore;

//   StrMap.iter(
//     (tag, posts) => {
//       let dest = outputDir /+ "tags" /+ tag;
//       Files.mkdirp(dest);
//       let (html, rss) =
//         Post.postList(
//           ~urlBase="https://jaredforsyth.com",
//           List.sort(sortPostsByDate, posts),
//           tagCounts,
//           "Tag: " ++ tag,
//         );
//       Files.writeFile(dest /+ "index.html", html) |> ignore;
//       Files.writeFile(dest /+ "rss.xml", rss) |> ignore;
//     },
//     tags,
//   );

//   posts;
// };

export let processTalks = (inputDir: string, outputDir: string) => {
    /* Talks */
    let talks = collectTalks(inputDir + '/talks.json');
    // let html = Talk.renderList(talks, "Talks");
    // Files.mkdirp(outputDir /+ "talks/");
    // Files.writeFile(outputDir /+ "talks/index.html", html)
    // |> Util.expectTrue("Unable to write talks page");
    return talks;
};

// let redirect = (url, contentTitle, description) => {
//   open Html;
//   let metaHead = Shared.metaHead;

//   <html>
//     <head>
//       <metaHead title=contentTitle description />
//       {{|<meta http-equiv="refresh" content="0; URL='|} ++ url ++ {|'" />|}}
//     </head>
//     <body> {"This page has moved to " ++ url} </body>
//   </html>;
// };

// let setupRedirectsForOldPosts = (outputDir, posts) => {
//   let oldPosts =
//     posts
//     |> List.filter(({Post.config: {Types.date}}) =>
//          Shared.dateSort(date, (2018, 1, 14)) < 0
//        );
//   oldPosts
//   |> List.iter(
//        (
//          {
//            Post.config: {
//              Types.title,
//              description,
//              fileName,
//              date: (year, month, day),
//            },
//          },
//        ) => {
//        let slug = Filename.basename(fileName) |> Util.chopSuffix;
//        let path =
//          Printf.sprintf("%04d/%02d/%02d/%s/", year, month, day, slug);
//        let fullPath = outputDir /+ path;
//        let realPath = "/posts" /+ slug;
//        /* print_endline(fullPath); */
//        Files.mkdirp(fullPath);
//        Files.writeFile(
//          fullPath /+ "index.html",
//          redirect(
//            realPath,
//            title,
//            description |? "Thoughts about programming, mostly",
//          ),
//        )
//        |> Util.expectTrue("Unable to write redirect " ++ fullPath);
//      });
// };
