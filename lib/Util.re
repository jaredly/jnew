
let split = (needle, haystack) => Str.split(Str.regexp(needle), haystack);
let splitFirst = (needle, haystack) => {
  switch (Str.split(Str.regexp(needle), haystack)) {
  | [] => assert(false)
  | [one] => (None, one)
  | [first, ...rest] => (Some(first), String.concat(needle, rest))
  }
};
let expectTrue = (message, result) => if (!result) {failwith(message)} else {()};

let chopSuffix = n => {
  if (Filename.check_suffix(n, ".nm.txt")) {
    Filename.chop_suffix(n, ".nm.txt")
  } else {
    Filename.chop_extension(n)
  }
}
