import fs from 'fs';
import fetch from 'node-fetch';

const main = async () => {
    const gists = JSON.parse(fs.readFileSync('./test/gists.json', 'utf8'));
    await Promise.all(
        gists.posts.map((post: string) =>
            fetch(`https://api.github.com/gists/${post}`, {
                headers: {
                    Accept: 'application/vnd.github+json',
                },
            })
                .then((res) => res.json())
                .then((raw) => {
                    const data = raw as {
                        files: {
                            [key: string]: {
                                filename: string;
                                content: string;
                                truncated: boolean;
                            };
                        };
                    };
                    if (!data.files) {
                        console.log(post, data);
                        throw new Error('Bad response');
                    }
                    const keys = Object.keys(data.files).filter((k) =>
                        data.files[k].filename.endsWith('.md'),
                    );
                    if (keys.length !== 1) {
                        console.log(post, data);
                        throw new Error('wrong number of markdonws');
                    }
                    const key = keys[0];
                    if (data.files[key].truncated) {
                        throw new Error(`Trunkated ${key} ${post}`);
                    }
                    fs.writeFileSync(
                        `./test/posts/${post}.md`,
                        data.files[key].content,
                        'utf8',
                    );
                }),
        ),
    );
};

main();
