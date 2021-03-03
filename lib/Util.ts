import path from 'path';

// let split = (needle, haystack) => Str.split(Str.regexp(needle), haystack);
// let splitFirst = (needle, haystack) => {
//   switch (Str.split(Str.regexp(needle), haystack)) {
//   | [] => assert(false)
//   | [one] => (None, one)
//   | [first, ...rest] => (Some(first), String.concat(needle, rest))
//   }
// };
// let expectTrue = (message, result) => if (!result) {failwith(message)} else {()};

export let chopSuffix = (n: string) => {
    if (n.endsWith('.nm.txt')) {
        return n.slice(0, -'.nm.txt'.length);
    }
    return n.slice(0, -path.extname(n).length);
};
