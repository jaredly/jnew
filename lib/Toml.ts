// let module StrMap = Map.Make(String);
export type doc = {
    strings: { [key: string]: string };
    stringLists: { [key: string]: string[] };
    bools: { [key: string]: boolean };
};

let empty: doc = { strings: {}, stringLists: {}, bools: {} };

let startsWith = (string: string, needle: string) => string.startsWith(needle);
// String.length(string) >= String.length(needle) && String.sub(string, 0, String.length(needle)) == needle;

export let parseToml = (text: string) => {
    let doc = empty;
    const [_, result] = text
        .split(/\n/g)
        .map((s) => s.trim())
        .filter((s) => s.length > 0)
        .reduce(
            ([curList, doc]: [null | string, doc], line) => {
                /* print_endline(line); */
                if (startsWith(line, '- ')) {
                    const value = line.slice('- '.length);
                    if (!curList) {
                        throw new Error('no curlist' + text);
                    }
                    return [
                        curList,
                        {
                            ...doc,
                            stringLists: {
                                ...doc.stringLists,
                                [curList]: [value, ...doc.stringLists[curList]],
                            },
                        },
                    ];
                } else {
                    const [name, ...rest] = line.split(':');
                    if (!name) {
                        console.log('unexpected line', line);
                        return [curList, doc];
                    }
                    if (rest.length === 1 && rest[0] === '') {
                        return [
                            name.trim(),
                            {
                                ...doc,
                                stringLists: { ...doc.stringLists, [name]: [] },
                            },
                        ];
                    }
                    let value = rest.join(':').trim();
                    if (value === 'true' || value === 'false') {
                        return [
                            null,
                            {
                                ...doc,
                                bools: {
                                    ...doc.bools,
                                    [name]: value === 'true',
                                },
                            },
                        ];
                    }
                    if (value.startsWith('"')) {
                        value = value.slice(1, -1);
                    }
                    return [
                        null,
                        { ...doc, strings: { ...doc.strings, [name]: value } },
                    ];
                }
            },
            [null as null | string, doc],
        );
    return result;
};

// let string = (name, doc) => {
//   if (StrMap.mem(name, doc.strings)) {
//     Some(StrMap.find(name, doc.strings))
//   } else {
//     None
//   }
// };

// let stringList = (name, doc) => {
//   if (StrMap.mem(name, doc.stringLists)) {
//     Some(StrMap.find(name, doc.stringLists))
//   } else {
//     None
//   }
// };

// let bool = (name, doc) => {
//   if (StrMap.mem(name, doc.bools)) {
//     Some(StrMap.find(name, doc.bools))
//   } else {
//     None
//   }
// };
