
let unwrap = (message, x) => switch x { | None => failwith(message) | Some(x) => x };

let (|?) = (x, y) => switch x { | None => y | Some(x) => x };
let (|?>) = (x, f) => switch x { | None => None | Some(x) => f(x) };
let (|?>>) = (x, f) => switch x { | None => None | Some(x) => Some(f(x)) };
let (|!) = (x, y) => switch x { | None => failwith(y) | Some(x) => x };

type venue = {
  where: string,
  video: option(string),
  date: option((int, int, int))
};

type talk = {
  title: string,
  image: option(string),
  slides: option(string),
  tweets: list(string),
  venues: list(venue)
};

let get = (name, items) => switch (List.assoc(name, items)) {
| exception Not_found => None
| v => Some(v)
};

let jsonString = item => switch item {
| `String(v) => Some(v)
| _ => None
};

let jsonArray = item => switch item {
| `List(v) => Some(v)
| _ => None
};

let parseVenue = item => switch item {
| `Assoc(items) => {
  let where = get("where", items) |?> jsonString |! "Where is required for a venue";
  let date = get("date", items) |?> jsonString |?>> Post.parseDate;
  let video = get("video", items) |?> jsonString;
  {where, date, video}
}
| _ => failwith("Bad venue json")
};

let collect = fname => {
  switch (Files.readFile(fname)) {
  | None => []
  | Some(text) => switch (Yojson.Basic.from_string(~fname, text)) {
    | exception _ => {
      failwith("Failed to parse talks json " ++ fname);
    }
    | `List(talks) => {
      List.map(
        talk => switch talk {
          | `Assoc(items) => {
            let title = get("title", items) |?> jsonString |! "Title required for talk";
            let slides = get("slides", items) |?> jsonString;
            let image = get("image", items) |?> jsonString;
            let tweets = get("tweets", items) |?> jsonArray |?>> List.map(jsonString) |?>> List.map(unwrap("Tweet must be a string")) |? [];
            let venues = get("venues", items) |?> jsonArray |?>> List.map(parseVenue) |? [];
            {title, image, slides, venues, tweets}
          }
          | _ => failwith("Bad talk json, not an assoc")
        },
        talks
      )
    }
    | _ => {
      failwith("Unexpected json in talks file " ++ fname);
    }
  }
  }
};
