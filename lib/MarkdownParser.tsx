import md from 'markdown-it';
// let isTwitterUrl = url => Str.string_match(Str.regexp("https?://twitter.com/[^/]+/status"), url, 0);

// let cacheFile = "./.tweet-cache";

// let module StrMap = Map.Make(String);

// let loadTwitterCache = (table) => {
//   switch (Yojson.Basic.from_file(cacheFile)) {
// | exception _ => {
//   print_endline("Unable to parse twitter cache");
//   ()
// }
// | `Assoc(items) =>
//   List.iter(
//     ((name, value)) => {
//       switch value {
//       | `String(value) => Hashtbl.replace(table, name, value)
//       | _ => {
//         print_endline("Unexpected value for " ++ name ++ ": " ++ Yojson.Basic.to_string(value));
//       }
//       }
//     },
//     items
//   )
// | _ => {
//   print_endline("Unexpected json from twitter cache");
//   ()
// }
// };
// };

// let twitterCache = Hashtbl.create(100);
// loadTwitterCache(twitterCache);

export let saveTwitterCache = () => {
    // let items = Hashtbl.fold(
    //   (key, value, items) => [(key, `String(value)), ...items],
    //   twitterCache,
    //   []
    // );
    // Yojson.Basic.to_file(cacheFile, `Assoc(items))
};

// open Omd;
// let twitterEmbed = element => switch element {
// | Url(href, [Text(contents)], "") when (href == contents || contents == "tweet") && isTwitterUrl(href) => {
//   let contents = if (Hashtbl.mem(twitterCache, href)) {
//     Some(Hashtbl.find(twitterCache, href))
//   } else {
//     Curl.global_init(Curl.CURLINIT_GLOBALSSL);
//     let t = Curl.init();
//     Curl.set_url(t, "https://publish.twitter.com/oembed?omit_script=1&url=" ++ href);
//     let data = ref("");
//     Curl.set_writefunction(t, newData => {
//       data := data^ ++ newData;
//       String.length(newData);
//     });
//     Curl.perform(t);
//     Curl.cleanup(t);
//     Curl.global_cleanup();
//     let parsed = switch (Yojson.Basic.from_string(data^)) {
//     | exception _ => None
//     | data => {
//       switch data {
//       | `Assoc(items) => {
//         switch (List.assoc("html", items)) {
//         | exception Not_found => None
//         | `String(html) => Some(html)
//         | _ => None
//         }
//       }
//       | _ => None
//       }
//     }
//     };
//     switch parsed {
//     | None => {
//       print_endline("Unexpected twitter response: " ++ data^);
//       None
//     }
//     | Some(html) => {
//       Hashtbl.replace(twitterCache, href, html);
//       Some(html)
//     }
//     };
//   };
//   switch contents {
//   | None => None
//   | Some(contents) =>
//   /* Some([Url(href, [Text("A TWITTER TWEET")], title)]) */
//     Some([
//       Url(href, [Text(href)], ""),
//       NL,
//       Raw(contents)
//     ])
//   }
// }
// | _ => None
// };

// let process = (~ids=?, rawBody) => {
//   let md = Omd.of_string(~extensions=[], rawBody);
//   let md = Omd.visit(twitterEmbed, md);
//   Omd.to_html(~ids?, md)
// };
import mdi from 'markdown-it-anchor';
const mdit = md('commonmark').use(mdi, {
    permalink: mdi.permalink.headerLink(),
    slugify: (text) => {
        // console.log(text);
        return text
            .trim()
            .replaceAll('"', '&quot;')
            .replaceAll(/[^a-zA-Z0-9_-]+/g, '-')
            .replace(/-$/, '');
    },
});

export const process = (rawBody: string, ids?: { [key: string]: string }) => {
    return mdit.render(rawBody);
};
