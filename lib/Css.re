
type item =
  | A(string, string)
  | Hover(list((string, string)))
  | Sub(string, list((string, string)))
  | Media(string, list((string, string)))
  /* | MarginRight(int) */
  ;

let attrs = items => String.concat("\n", List.map(((attr, value)) => "  " ++ attr ++ ": " ++ value ++ ";", items));
let rule = (name, items) => name ++ " {\n" ++ attrs(items) ++ "\n}";

let selectors = (k, items, res) => {
  let (hover, subs, media, normals) = List.fold_left(
    (((hover, subs, media, normals), item) => switch item {
    | A(attr, value) => (hover, subs, media, [(attr, value), ...normals])
    | Hover(items) => (hover @ items, subs, media, normals)
    | Sub(name, items) => (hover, [(name, items), ...subs], media, normals)
    | Media(name, items) => (hover, subs, [(name, items), ...media], normals)
    }),
    ([], [], [], []),
    items
  );
  let subs = List.rev(subs);
  let normals = List.rev(normals);

  let ruleName = ".style-" ++ string_of_int(k);

  [rule(ruleName, normals)]
  @
  List.map(
    ((name, items)) => "@media(" ++ name ++ ") {\n" ++ rule(ruleName, items) ++ "\n}",
    media
  )
  @
  List.map(
    ((name, items)) => rule(name, items),
    subs
  )
  @
  switch hover {
  | [] => []
  | items => [rule(".style-" ++ string_of_int(k), hover)]
  }
  @ res
};

let startPage = () => {
  let ids = Hashtbl.create(50);

  let css = (items) => {
    let hash = Hashtbl.hash(items);
    Hashtbl.replace(ids, hash, items);
    "style-" ++ string_of_int(hash);
  };

  let inlineCss = () => {
    Hashtbl.fold((k, v, res) => selectors(k, v, res), ids, []) |> String.concat("\n")
  };

  (css, inlineCss)
};

