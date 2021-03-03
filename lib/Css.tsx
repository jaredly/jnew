import { CssAttr as item, Hover, Media } from './sjsx';
import objectHash from 'object-hash';

type pair = [string, string];
let attrs = (items: pair[]) =>
    items.map(([attr, value]) => '  ' + attr + ': ' + value + ';').join('\n');
let rule = (name: string, items: pair[]) =>
    name + ' {\n' + attrs(items) + '\n}';

let selectors = (k: string, items: item[]) => {
    type Alls = [
        pair[],
        { name: string; items: pair[] }[],
        { name: string; items: pair[] }[],
        pair[],
    ];
    let [hover, subs, media, normals] = items.reduce(
        ([hover, subs, media, normals]: Alls, item): Alls => {
            switch (item.type) {
                case 'A':
                    return [
                        hover,
                        subs,
                        media,
                        [[item.name, item.value], ...normals],
                    ];
                case 'Hover':
                    return [hover.concat(item.children), subs, media, normals];
                case 'Sub':
                    return [
                        hover,
                        [
                            { name: item.selector, items: item.children },
                            ...subs,
                        ],
                        media,
                        normals,
                    ];
                case 'Media':
                    return [
                        hover,
                        subs,
                        [
                            { name: item.selector, items: item.children },
                            ...media,
                        ],
                        normals,
                    ];
            }
        },
        [[], [], [], []] as Alls,
    );
    subs.reverse();
    normals.reverse();

    let ruleName = '.style-' + k;

    return [
        rule(ruleName, normals),
        ...media.map(
            ({ name, items }) =>
                '@media(' + name + ') {\n' + rule(ruleName, items) + '\n}',
        ),
        ...subs.map(({ name, items }) => rule(ruleName + ' ' + name, items)),
        ...(hover.length ? [rule('.style-' + k + ':hover', hover)] : []),
    ];
};

export let startPage = () => {
    let ids: { [key: string]: item[] } = {};

    let css = (items: item[]) => {
        let hash = objectHash(items);
        ids[hash] = items;
        return 'style-' + hash;
    };

    let inlineCss = () => {
        return Object.entries(ids)
            .flatMap(([key, items]) => selectors(key, items))
            .join('\n');
    };

    return { css, inlineCss };
};
