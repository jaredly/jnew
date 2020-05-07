open Html;

let channel = normal("channel", []);
let title = normal("title", []);
let author = normal("author", []);
let description = normal("description", []);
let summary = normal("summary", []);
let cdata = text => {|<![CDATA[|} ++ text ++ {|]]>|};

let rss = (~children, ()) => {
  {|<rss
    xmlns:dc="http://purl.org/dc/elements/1.1/"
    xmlns:content="http://purl.org/rss/1.0/modules/content/"
    xmlns:atom="http://www.w3.org/2005/Atom"
    version="2.0">|}
  ++ String.concat("\n", children)
  ++ "</rss>";
};

let item = normal("item", []);
let link = normal("link", []);
let guid = normal("guid", []);
let category = normal("category", []);
let pubDate = normal("pubDate", []);
let content = normal("content", []);

type post = {
  title: string,
  description: string,
  url: string,
  content: string,
  date: (int, int, int),
  category: option(string),
};

let escapeContent = value => {
  value
  |> Str.global_replace(Str.regexp_string("&"), "&amp;")
  |> Str.global_replace(Str.regexp_string("<"), "&lt;")
  |> Str.global_replace(Str.regexp_string(">"), "&gt;");
                                                         //   |> Str.global_replace(Str.regexp_string("\""), "&quot;")
};

let force = x =>
  switch (x) {
  | None => failwith("op")
  | Some(x) => x
  };

let datePrinter =
  ODate.Unix.To.generate_printer("%d %b %Y %H:%M:%S GMT") |> force;
let makeDate = ((year, month, day)) =>
  ODate.Unix.make(~day, ~month=ODate.Month.of_int(month), ~year, ());

let wrapper = (~siteTitle, ~posts) =>
  <rss>
    <channel>
      <title> {cdata(siteTitle)} </title>
      //   <description> {cdata(siteDescription)} </description>
      //   <lastBuildDate>
      {posts
       |> List.map(post => {
            <item>

                <title> {cdata(post.title)} </title>
                <description> {cdata(post.description)} </description>
                <summary> {cdata(post.description)} </summary>
                <author> "Jared Forsyth" </author>
                <link> {post.url} </link>
                <guid> {post.url} </guid>
                {switch (post.category) {
                 | None => ""
                 | Some(cat) => <category> {cdata(cat)} </category>
                 }}
                <pubDate>
                  {ODate.Unix.To.string(datePrinter, makeDate(post.date))}
                </pubDate>
              </item>
              //   <content> {cdata(post.content)} </content>
          })
       |> String.concat("\n\n")}
    </channel>
  </rss>;