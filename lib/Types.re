
let module StrMap = Map.Make(String);

type config = {
  title: string,
  tags: list(string),
  categories: list(string),
  date: (int, int, int),
  description: option(string),
  thumbnail: option(string),
  article_image: option(string),
  featured: bool,
  draft: bool,
  fileName: string,
  wordCount: option(int)
};
