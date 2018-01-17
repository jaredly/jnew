
let module StrMap = Map.Make(String);
type doc = {
  strings: StrMap.t(string),
  stringLists: StrMap.t(list(string)),
  bools: StrMap.t(bool)
};

let empty = {strings: StrMap.empty, stringLists: StrMap.empty, bools: StrMap.empty};

let startsWith = (string, needle) => String.length(string) >= String.length(needle) && String.sub(string, 0, String.length(needle)) == needle;

let parse = (text) => {
  let doc = empty;
  Str.split(Str.regexp("\n"), text)
  |> List.map(String.trim)
  |> List.filter(s => String.length(s) > 0)
  |> List.fold_left(
    ((curList, doc), line) => {
      /* print_endline(line); */
      if (startsWith(line, "- ")) {
        switch curList {
        | None => failwith("Unexpected list item, not in a list: " ++ line)
        | Some(name) => {
          let value = String.sub(line, String.length("- "), String.length(line) - 2);
          (curList, {...doc, stringLists: StrMap.add(name, [value, ...StrMap.find(name, doc.stringLists)], doc.stringLists)})
        }
        }
      } else {
        switch (Str.split_delim(Str.regexp(":"), line)) {
        | [] | [_] => {
          print_endline("Skipping unexpected line... " ++ line);
          (None, doc)
        }
        | [name, ""] => {
          let name = String.trim(name);
          (Some(name), {...doc, stringLists: StrMap.add(name, [], doc.stringLists)})
        }
        | [name, ...rest] => {
          let name = String.trim(name);
          let value = String.concat(":", rest) |> String.trim;
          switch value {
          | "true" => (None, {...doc, bools: StrMap.add(name, true, doc.bools)})
          | "false" => (None, {...doc, bools: StrMap.add(name, false, doc.bools)})
          | _ =>
            let value = if (value.[0] == '"') {
              String.sub(value, 1, String.length(value) - 2)
            } else {
              value
            };
            (None, {...doc, strings: StrMap.add(name, value, doc.strings)});
          }
        }
        }
      }
    },
    (None, doc)
  ) |> ((_, doc)) => doc
};

let string = (name, doc) => {
  if (StrMap.mem(name, doc.strings)) {
    Some(StrMap.find(name, doc.strings))
  } else {
    None
  }
};

let stringList = (name, doc) => {
  if (StrMap.mem(name, doc.stringLists)) {
    Some(StrMap.find(name, doc.stringLists))
  } else {
    None
  }
};

let bool = (name, doc) => {
  if (StrMap.mem(name, doc.bools)) {
    Some(StrMap.find(name, doc.bools))
  } else {
    None
  }
};