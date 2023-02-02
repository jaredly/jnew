import { process } from './MarkdownParser';

export type nmNode = {
    typ: string;
    depth: number;
    content: string;
    children: nmNode[];
    types: any;
};

let collectNodes = (content: string) => {
    // console.log("Collect");
    let rawNodes = content
        .split(/^# /gm)
        .map((m) => m.trim())
        .filter(Boolean);
    // Printf.printf("Split %d nodes\n", rawNodes |> List.length);
    return rawNodes.map((text) => {
        let [attrsRaw, ...rest] = text.split('\n');
        let attrs = JSON.parse(attrsRaw);
        let content = rest
            .map((line) => line.replace(/^  /, ''))
            .join('\n')
            .trim();
        let node = {
            typ: attrs.type,
            depth: attrs.depth,
            types: attrs.types,
            content,
            children: [],
        };
        return node;
    });
};

let getChildren = (
    depth: number,
    children: nmNode[],
    nodes: nmNode[],
): [nmNode[], nmNode[]] => {
    if (!nodes.length) {
        return [children, []];
    }
    const node = nodes[0];
    if (node.depth <= depth) {
        return [children, nodes];
    }
    const [sub, rest] = getChildren(node.depth, [], nodes.slice(1));
    return getChildren(
        depth,
        [{ ...node, children: sub.reverse() }, ...children],
        rest,
    );
    // switch nodes {
    // | [] => (children, [])
    // | [node, ..._] when node.depth <= depth =>
    //   // Printf.printf("Children %d - %d\n", depth, node.depth);
    //   (children, nodes)
    // | [node, ...rest] =>
    //   let (sub, rest) = getChildren(node.depth, [], rest);
    //   getChildren(depth, [{...node, children: sub |> List.rev}, ...children], rest)
};

let countMarkdownWords = (content: string) =>
    content.split(/[^a-zA-Z0-9-]+/).length;

export let wordCount = (node: nmNode): number => {
    switch (node.typ) {
        case 'list':
        case 'orderedList':
        case 'normal':
        case 'header':
            return node.children.reduce(
                (c, a) => c + wordCount(a),
                countMarkdownWords(node.content),
            );
        case 'note':
            return countMarkdownWords(node.content);
        case 'code':
            return 0;
    }
    return 0;
};

let organizeNodes = (nodes: nmNode[]) => {
    // print_endline("org")
    let loop = (organized: nmNode[], list: nmNode[]): nmNode[] => {
        if (!list.length) {
            return organized;
        }
        const [node, ...rest] = list;
        const [children, rrest] = getChildren(node.depth, [], rest);
        return loop(
            [{ ...node, children: children.reverse() }, ...organized],
            rrest,
        );
        // switch list {
        // | [node, ...rest] =>
        // // print_endline("A node " + node.typ + " -- " + node.content);
        //   let (children, rest) = getChildren(node.depth, [], rest);
        //   loop([{...node, children: children |> List.rev}, ...organized], rest)
        // | [] =>
        // print_endline("Done?");
        // organized
    };
    return loop([], nodes);
};

export let parseNotableMind = (text: string) =>
    organizeNodes(collectNodes(text));

let header = (depth: number) => {
    let b = '';
    for (let x = 0; x < depth; x++) {
        b += '#';
    }
    return b + ' ';
};

let makeList = (el: string, items: string[]) => {
    return (
        '<' +
        el +
        '>' +
        items.map((item) => '<li>' + item + '</li>').join('\n') +
        '</' +
        el +
        '>'
    );
};

// Printexc.record_backtrace(true)

// let escapeCode = value => {
//   value
//   |> Str.global_replace(Str.regexp_string("&"), "&amp;")
//   |> Str.global_replace(Str.regexp_string("\""), "&quot;")
//   |> Str.global_replace(Str.regexp_string("<"), "&lt;")
//   |> Str.global_replace(Str.regexp_string(">"), "&gt;")
// };

let getLanguage = (types: any) => {
    return types?.code?.language ?? 'javascript';
    // switch (types) {
    //   | Some(`Assoc(items)) => switch (List.assoc_opt("code", items)) {
    //     | Some(`Assoc(items)) => switch (List.assoc_opt("language", items)) {
    //       | Some(`String(v)) => Some(v)
    //       | _ => None
    //     }
    //     | _ => None
    //   }
    //   | _ => None
    // }
};

import prism from 'prismjs';

let highlightCode = (code: string, language?: string) => {
    language = language ?? 'javascript';
    const html = prism.highlight(code, prism.languages.javascript, language);
    // let (stdout, stderr, _) = Commands.execFull(input=code, Printf.sprintf({|%s %s|}, highlighter, Filename.quote(language)));
    // print_endline(String.concat("\n", stderr));
    // let output = String.concat("\n", stdout);
    return html;
};

type Ids = { [key: string]: string };
let renderNode = (ids: Ids, depth: number, node: nmNode): string[] => {
    switch (node.typ) {
        case 'list':
            return [
                process(node.content, ids),
                makeList(
                    'ul',
                    node.children
                        .map((child) => renderNode(ids, depth + 1, child))
                        .map((items) => '<div>' + items.join('\n') + '</div>'),
                ),
            ];
        case 'orderedList':
            return [
                process(node.content, ids),
                makeList(
                    'ol',
                    node.children
                        .map((child) => renderNode(ids, depth + 1, child))
                        .map((items) => '<div>' + items.join('\n') + '</div>'),
                ),
            ];
        case 'note':
            return [
                "<div class='note'>" + process(node.content, ids) + '</div>',
            ];
        case 'normal':
            return [
                process(node.content, ids),
                ...node.children.flatMap((child) =>
                    renderNode(ids, depth + 1, child),
                ),
            ];
        case 'header':
            return [
                process(header(depth) + node.content, ids),
                ...node.children.flatMap((child) =>
                    renderNode(ids, depth + 1, child),
                ),
            ];
        case 'code':
            return [
                '<pre><code>' +
                    highlightCode(node.content, getLanguage(node.types)) +
                    '</code></pre>',
            ];
    }
    throw new Error('nope');
};

export let renderNm = (nodes: nmNode[]) => {
    let ids: Ids = {};
    return nodes.flatMap((node) => renderNode(ids, 1, node)).join('\n');
};
