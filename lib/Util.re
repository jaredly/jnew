
let split = (needle, haystack) => Str.split(Str.regexp(needle), haystack);
let splitFirst = (needle, haystack) => {
  switch (Str.split(Str.regexp(needle), haystack)) {
  | [] => assert(false)
  | [one] => (None, one)
  | [first, ...rest] => (Some(first), String.concat(needle, rest))
  }
};
