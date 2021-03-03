import { writeFileSync } from 'fs';
import { render } from '../lib/Home';
import { saveTwitterCache } from '../lib/MarkdownParser';
import { post } from '../lib/Post';
import { project } from '../lib/Project';
import { processProjects, processTalks } from '../lib/Yak';

let run = (excludeDrafts: boolean) => {
    let outputDir = './test/pages/';
    let inputDir = './test';

    let projects = processProjects(inputDir, outputDir);
    let posts: post[] = []; // processBlog(excludeDrafts, inputDir, outputDir);
    let talks = processTalks(inputDir, outputDir);

    // setupRedirectsForOldPosts(outputDir, posts);

    /* Home page */
    writeFileSync(
        outputDir + '/index.html',
        render(projects, posts, talks),
        'utf8',
    );

    saveTwitterCache();
    console.log('Finished!');
};

const [_, drafts] = process.argv;
run(drafts !== 'drafts');
