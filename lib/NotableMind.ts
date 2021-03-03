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
        // // print_endline("A node " ++ node.typ ++ " -- " ++ node.content);
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

// let header = (depth: string) => {
//     let b = Buffer.create(depth);
//     for (x in 1 to depth) {
//         Buffer.add_char(b, '#');
//     };
//     b |> Buffer.contents
// };

// let makeList = (el, items) => {
//     "<" ++ el ++ ">"
//     ++
//     (items |> List.map(item => "<li>" ++ item ++ "</li>") |> String.concat("\n"))
//     ++
//     "</" ++ el ++ ">"
// }

// Printexc.record_backtrace(true)

// let escapeCode = value => {
//   value
//   |> Str.global_replace(Str.regexp_string("&"), "&amp;")
//   |> Str.global_replace(Str.regexp_string("\""), "&quot;")
//   |> Str.global_replace(Str.regexp_string("<"), "&lt;")
//   |> Str.global_replace(Str.regexp_string(">"), "&gt;")
// };

// let getLanguage = types => {
//   switch types {
//     | Some(`Assoc(items)) => switch (List.assoc_opt("code", items)) {
//       | Some(`Assoc(items)) => switch (List.assoc_opt("language", items)) {
//         | Some(`String(v)) => Some(v)
//         | _ => None
//       }
//       | _ => None
//     }
//     | _ => None
//   }
// }

// let highlightCode = (code, language) => {
//   let highlighter = "highlighter/highlight.js";
//   let language = switch language {
//     | None => "javascript"
//     | Some(x) => x
//   };
//   let (stdout, stderr, _) = Commands.execFull(~input=code, Printf.sprintf({|%s %s|}, highlighter, Filename.quote(language)));
//   print_endline(String.concat("\n", stderr));
//   let output = String.concat("\n", stdout);
//   output
// };

// let rec renderNode = (~ids, depth, node) => {
//   switch (node.typ) {
//   | "list" => [
//       MarkdownParser.process(~ids, node.content),
//       makeList(
//         "ul",
//         List.map(renderNode(~ids, depth + 1), node.children) |> List.map(items => "<div>" ++ String.concat("\n", items) ++ "</div>"),
//       ),
//     ]
//   | "orderedList" => [
//       MarkdownParser.process(~ids, node.content),
//       makeList(
//         "ol",
//         List.map(renderNode(~ids, depth + 1), node.children) |> List.map(items => "<div>" ++ String.concat("\n", items) ++ "</div>"),
//       ),
//     ]
//   | "note" => ["<div class='note'>" ++ MarkdownParser.process(~ids, node.content) ++ "</div>"]
//   | "normal" => [
//       MarkdownParser.process(~ids, node.content),
//       ...List.map(renderNode(~ids, depth + 1), node.children) |> List.concat,
//     ]
//   | "header" => [
//       MarkdownParser.process(~ids, header(depth) ++ node.content),
//       ...List.map(renderNode(~ids, depth + 1), node.children) |> List.concat,
//     ]
//   | "code" => [
//     "<pre><code>" ++ highlightCode(node.content, getLanguage(node.types)) ++ "</code></pre>"
//   ]
//   | _ => failwith("Unexpected node type")
//   };
// };

// let render = (nodes) => {
//     let ids = Hashtbl.create(10);
//     nodes |> List.map(renderNode(~ids, 1)) |> List.concat |> String.concat("\n")
// }
