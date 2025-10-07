import {mkdir, mkdirSync, readdirSync, readFileSync, writeFileSync} from 'fs';
import {collectTalks, renderTalkList} from './Talk';
import {chopSuffix} from './Util';
// import toml from 'toml';
import {parseProject, project, renderList, renderProject} from './Project';
import {dateSort} from './Shared';
import {parseToml} from './Toml';
import {parseNm, parsePost, post, postList, renderPost} from './Post';
import {renderPoem} from './Poetry';

let sortPostsByDate = ({config: {date: date1}}: post, {config: {date: date2}}: post) => dateSort(date2, date1);

let sortProjectsByDate = ({updates}: project, {updates: updates2}: project) =>
    updates.length && updates2.length ? dateSort(updates2[0][0], updates[0][0]) : updates.length ? 1 : -1;

let splitTopYaml = (text: string): [any | null, string] => {
    let divider = '\n---\n';
    const parts = text.split(divider);
    if (parts.length === 1) {
        return [null, parts[0]];
    }
    return [parseToml(parts[0]), parts.slice(1).join(divider)];
};

let collectPages = <V>(inputDir: string, outputDir: string, baseDir: string, parse: (name: string, text: string) => V): [string, V][] => {
    let base = inputDir + '/' + baseDir;
    return readdirSync(base)
        .filter((f) => f.endsWith('.md') || f.endsWith('.html') || f.endsWith('.nm.txt'))
        .map((fileName) => {
            let fullName = base + '/' + fileName;
            let contents = readFileSync(fullName, 'utf8');
            let result = parse(baseDir + '/' + fileName, contents);
            let dest = outputDir + '/' + baseDir + '/' + chopSuffix(fileName);
            mkdirSync(dest, {recursive: true});
            let fullDest = dest + '/' + 'index.html';
            return [fullDest, result];
        });
};

let assembleTags = (posts: post[]) => {
    return posts.reduce((byTag, post) => {
        return (
            post.config.tags?.reduce(
                (byTag, tag) => ({
                    ...byTag,
                    [tag]: [post, ...(byTag[tag] || [])],
                }),
                byTag,
            ) ?? byTag
        );
    }, {} as {[key: string]: post[]});
};

let assembleProjectTags = (projects: project[]) => {
    return projects.reduce((byTag, config) => {
        return (
            config.tags?.reduce(
                (byTag, tag) => ({
                    ...byTag,
                    [tag]: [config, ...(byTag[tag] || [])],
                }),
                byTag,
            ) ?? byTag
        );
    }, {} as {[key: string]: project[]});
};

export let processProjects = (inputDir: string, outputDir: string) => {
    /* Project pages */
    let projects = collectPages(inputDir, outputDir, 'projects', (fileName, contents) => {
        let [opts, body] = splitTopYaml(contents);
        return parseProject(fileName, opts, body);
    })
        .map(([dest, config]) => {
            let html = renderProject(config);
            writeFileSync(dest, html, 'utf8');
            return config;
        })
        .sort(sortProjectsByDate);
    let projectTags = assembleProjectTags(projects);
    let projectTagCounts = Object.entries(projectTags)
        .map(([res, value]) => [res, value.length] as [string, number])
        .sort(([k, n], [v, n2]) => n2 - n);

    let html = renderList({
        tags: projectTagCounts,
        projects,
        contentTitle: 'All projects',
    });
    writeFileSync(outputDir + '/projects/index.html', html);

    Object.entries(projectTags).forEach(([tag, posts]) => {
        let dest = outputDir + '/projects/tags/' + tag;
        mkdirSync(dest, {recursive: true});
        let html = renderList({
            tags: projectTagCounts,
            projects: posts.sort(sortProjectsByDate),
            contentTitle: 'Tag: ' + tag,
        });
        writeFileSync(dest + '/index.html', html, 'utf8');
    });

    return projects;
};

export let processPoetry = (inputDir: string, outputDir: string) => {
    let fullPosts = collectPages(inputDir, outputDir, 'poems', (fileName, contents) => {
        let [opts, body] = splitTopYaml(contents);
        return parsePost(fileName, opts, body);
    });
    let posts = fullPosts
        .map((p) => p[1])
        .filter(Boolean)
        .sort(sortPostsByDate);
    fullPosts.forEach(([dest, post]) => {
        let html = renderPoem(posts, post);
        writeFileSync(dest, html, 'utf8');
    });

    let tags = assembleTags(posts);
    let tagCounts = Object.entries(tags)
        .map(([res, value]) => [res, value.length] as [string, number])
        .sort(([k, n], [v, n2]) => n2 - n);

    let {main: html, rss} = postList('https://jaredforsyth.com', posts, tagCounts, 'All poems', false, '/poems');

    writeFileSync(outputDir + '/poems/index.html', html);
    writeFileSync(outputDir + '/poems/rss.xml', rss);

    Object.entries(tags).forEach(([tag, posts]) => {
        let dest = outputDir + '/poems/tags/' + tag;
        mkdirSync(dest, {recursive: true});
        let {main: html, rss} = postList('https://jaredforsyth.com', posts.sort(sortPostsByDate), tagCounts, 'Tag: ' + tag, false, '/poems');
        writeFileSync(dest + '/index.html', html);
        writeFileSync(dest + '/rss.xml', rss);
    }, tags);

    return posts;
};

export let processBlog = (excludeDrafts: boolean, inputDir: string, outputDir: string) => {
    /* Posts */
    let fullPosts = collectPages(inputDir, outputDir, 'posts', (fileName, contents) => {
        if (fileName.endsWith('.nm.txt')) {
            return parseNm(fileName, contents);
            // throw new Error('no nm');
            // console.log(fileName);
        } else {
            let [opts, body] = splitTopYaml(contents);
            return parsePost(fileName, opts, body);
        }
    });
    let allPosts = fullPosts
        .map((p) => p[1])
        .filter(Boolean)
        .sort(sortPostsByDate);
    let posts = excludeDrafts ? allPosts.filter((post) => !post.config.draft) : allPosts;
    fullPosts.forEach(([dest, post]) => {
        let html = renderPost(posts, post);
        writeFileSync(dest, html, 'utf8');
    });
    //   renderPages(renderPost(posts), fullPosts)

    let tags = assembleTags(posts);
    let tagCounts = Object.entries(tags)
        .map(([res, value]) => [res, value.length] as [string, number])
        .sort(([k, n], [v, n2]) => n2 - n);

    let {main: html, rss} = postList('https://jaredforsyth.com', posts, tagCounts, 'All posts');
    writeFileSync(outputDir + '/posts/index.html', html);

    writeFileSync(outputDir + '/posts/rss.xml', rss);

    Object.entries(tags).forEach(([tag, posts]) => {
        let dest = outputDir + '/tags/' + tag;
        mkdirSync(dest, {recursive: true});
        let {main: html, rss} = postList('https://jaredforsyth.com', posts.sort(sortPostsByDate), tagCounts, 'Tag: ' + tag);
        writeFileSync(dest + '/index.html', html);
        writeFileSync(dest + '/rss.xml', rss);
    }, tags);

    return posts;
};

export let processTalks = (inputDir: string, outputDir: string) => {
    /* Talks */
    let talks = collectTalks(inputDir + '/talks.json');
    let html = renderTalkList(talks, 'Talks');
    mkdirSync(outputDir + '/talks/', {recursive: true});
    writeFileSync(outputDir + '/talks/index.html', html, 'utf8');
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
