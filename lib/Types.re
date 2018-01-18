
type config = {
  title: string,
  tags: list(string),
  categories: list(string),
  date: (int, int, int),
  description: option(string),
  thumbnail: option(string),
  featured: bool,
  fileName: string
};
