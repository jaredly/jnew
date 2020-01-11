
module Json = {
  module Infix = {
    let (|?>) = (v, fn) => switch v {
      | Ok(v) => fn(v)
      | Error(e) => Error(e)
    };
    let (|?>>) = (v, fn) => switch v {
      | Ok(v) => Ok(fn(v))
      | Error(e) => Error(e)
    };
    let (|!) = (v, msg) => switch v {
      | Ok(v) => v
      | Error(e) => failwith("Unwrapping result, error '" ++ e ++ "', " ++ msg)
    };
    let opt = x => switch x {
      | Ok(v) => Some(v)
      | _ => None
    };
  };

  let get = (name, obj) => switch obj {
    | `Assoc(items) => switch (List.assoc(name, items)) {
      | exception Not_found => Error("No object attribute " ++ name)
      | res => Ok(res)
    }
    | _ => Error("Not an object")
  }
  let string = item => switch item {
    | `String(s) => Ok(s)
    | _ => Error("Not a string")
  }
  let array = item => switch item {
    | `List(items) => Ok(items)
    | _ => Error("Not an array")
  }
  let int = item => switch item {
    | `Int(int) => Ok(int)
    | _ => Error("Not an int")
  }
  let bool = item => switch item {
    | `Bool(bool) => Ok(bool)
    | _ => Error("Not a bool")
  }

  let mapResult = (fn, list) => {
    let rec loop = (col, items) => switch items {
      | [] => Ok(List.rev(col))
      | [one, ...rest] => switch (fn(one)) {
        | Error(e) => Error(e)
        | Ok(v) => loop([v, ...col], rest)
      }
    };
    loop([], list)
  };

  module Get = {
    open Infix;
    let stringList = (attr, obj) => obj |> get(attr) |?> array |?> mapResult(string) |> opt;
    let string = (attr, obj) => obj |> get(attr) |?> string |> opt;
    let bool = (attr, obj) => obj |> get(attr) |?> bool |> opt;
  }
}

type nmNode = {typ: string, depth: int, content: string, children: list(nmNode)};

let collectNodes = content => {
    print_endline("Collect");
  let rawNodes = Str.split(Str.regexp("^# "), content);
  Printf.printf("Split %d nodes\n", rawNodes |> List.length);
  rawNodes |> List.map(text => {
    let lines = String.split_on_char('\n', text);
    switch lines {
      | [attrsRaw, ...rest] =>
        let attrs = Yojson.Basic.from_string(attrsRaw);
        let content = rest |> List.map(line => Str.global_replace(Str.regexp("^  "), "", line)) |> String.concat("\n") |> String.trim;
        // print_endline("Input contents");
        // print_endline(rest |> String.concat("\n"));
        // print_endline("Content");
        // print_endline(content);
        open Json.Infix;
        let node = {
          typ: attrs |> Json.get("type") |?> Json.string |! "nm node type " ++ attrsRaw,
          depth: attrs |> Json.get("depth") |?> Json.int |! "nm node depth " ++ attrsRaw,
          content,
          children: []
        }
        node
      | [] => failwith("rawNodes")
    }
  })
}

let rec getChildren = (depth, children, nodes) => switch nodes {
  | [] => (children, [])
  | [node, ..._] when node.depth <= depth => 
    // Printf.printf("Children %d - %d\n", depth, node.depth);
    (children, nodes)
  | [node, ...rest] => 
    let (sub, rest) = getChildren(node.depth, [], rest);
    getChildren(depth, [{...node, children: sub |> List.rev}, ...children], rest)
}

let countMarkdownWords = content => Str.split(Str.regexp("[^a-zA-Z0-9-]+"), content) |> List.length;

let rec wordCount = (node) => {
  switch (node.typ) {
  | "list"
  | "orderedList"
  | "normal" | "header" => List.fold_left((c, a) => c + wordCount(a), countMarkdownWords(node.content), node.children)
  | "note" => countMarkdownWords(node.content)
  | "code" => 0
  | _ => failwith("Unexpected node type")
  };
}

let organizeNodes = nodes => {
    // print_endline("org")
  let rec loop = (organized, list) => switch list {
    | [node, ...rest] =>
    // print_endline("A node " ++ node.typ ++ " -- " ++ node.content);
      let (children, rest) = getChildren(node.depth, [], rest);
      loop([{...node, children: children |> List.rev}, ...organized], rest)
    | [] => 
    print_endline("Done?");
    organized
  };
  loop([], nodes)
};

let parse = text => collectNodes(text) |> organizeNodes;

let header = depth => {
    let b = Buffer.create(depth);
    for (x in 1 to depth) {
        Buffer.add_char(b, '#');
    };
    b |> Buffer.contents
};

let makeList = (el, items) => {
    "<" ++ el ++ ">"
    ++
    (items |> List.map(item => "<li>" ++ item ++ "</li>") |> String.concat("\n"))
    ++
    "</" ++ el ++ ">"
}

Printexc.record_backtrace(true)

let escapeCode = value => {
  value
  |> Str.global_replace(Str.regexp_string("&"), "&amp;")
  |> Str.global_replace(Str.regexp_string("\""), "&quot;")
  |> Str.global_replace(Str.regexp_string("<"), "&lt;")
  |> Str.global_replace(Str.regexp_string(">"), "&gt;")
}

let rec renderNode = (~ids, depth, node) => {
  switch (node.typ) {
  | "list" => [
      MarkdownParser.process(~ids, node.content),
      makeList(
        "ul",
        List.map(renderNode(~ids, depth + 1), node.children) |> List.concat,
      ),
    ]
  | "orderedList" => [
      MarkdownParser.process(~ids, node.content),
      makeList(
        "ol",
        List.map(renderNode(~ids, depth + 1), node.children) |> List.concat,
      ),
    ]
  | "note" => ["<note>" ++ MarkdownParser.process(~ids, node.content) ++ "</note>"]
  | "normal" => [
      MarkdownParser.process(~ids, node.content),
      ...List.map(renderNode(~ids, depth + 1), node.children) |> List.concat,
    ]
  | "header" => [
      MarkdownParser.process(~ids, header(depth) ++ node.content),
      ...List.map(renderNode(~ids, depth + 1), node.children) |> List.concat,
    ]
  | "code" => [
    "<pre><code>" ++ escapeCode(node.content) ++ "</code></pre>"
  ]
  | _ => failwith("Unexpected node type")
  };
};

let render = (nodes) => {
    let ids = Hashtbl.create(10);
    nodes |> List.map(renderNode(~ids, 1)) |> List.concat |> String.concat("\n")
}
