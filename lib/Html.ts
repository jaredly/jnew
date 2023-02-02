let props = '';

let escapeAttributeValue = (value: string) =>
    typeof value === 'string'
        ? value.replace(/&/g, '&amp;').replace(/\"/g, '&quot;')
        : typeof value === 'boolean'
        ? value + ''
        : (console.log('attributed value is', value), JSON.stringify(value));

let formatAttributes = (attrs: [string, string][]) =>
    attrs
        .map(
            ([name, v]) =>
                ' ' +
                (name === 'className' ? 'class' : name) +
                '="' +
                escapeAttributeValue(v) +
                '"',
        )
        .join(' ');

let selfClosing = (tag: string) => {
    switch (tag) {
        case 'meta':
            return true;
        case 'br':
            return true;
        case 'img':
            return true;
        default:
            return false;
    }
};

export let normal = (
    name: string,
    preattrs: [string, string][],
    { attrs, children }: { attrs: [string, string][]; children: string[] },
) => {
    let head = name + formatAttributes(preattrs.concat(attrs));
    if (children.length === 0) {
        return '<' + head + (selfClosing(name) ? '/>' : '></' + name + '>');
    } else {
        let body = children.join('\n');
        if (body.includes('\n')) {
            return '<' + head + '>\n' + body + '\n</' + name + '>';
        } else {
            return '<' + head + '>' + body + '</' + name + '>';
        }
    }
};

// let filterPresentValues = List.fold_left(
//   (res, (name, value)) => {
//     switch value {
//     | None => res
//     | Some(thing) => [(name, thing), ...res]
//     }
//   },
//   []
// );
// let filterPresentValues = vals => List.rev(filterPresentValues(vals));

const supported = [
    'class',
    'id',
    'lang',
    'title',
    'style',
    'itemprop',
    'itemtype',
    'itemscope',
    'role',
    'name',
];

type pairs = [string, string][];
// let contentTag = (tagName: string, preattrs: pairs,
//   attrs: JSX.props
// ) => normal(tagName, (preattrs.concat([
//   ("class", className),
//   ("id", id),
//   ("lang", lang),
//   ("title", title),
//   ("style", style),
//   ("itemprop", itemprop),
//   ("itemtype", itemtype),
//   ("itemscope", itemscope),
//   ("role", role),
//   ("name", name),
//   /* ("") */
// ])));

// let meta = (~name, ~content) => normal("meta", [("name", name), ("content", content)]);
// let pmeta = (~property, ~content) => normal("meta", [("property", property), ("content", content)]);
// let link = (~rel, ~href, ~media=?) => normal("link", [("rel", rel), ("href", href)]);
// let module Meta = {
//   let charset = (name) => "<meta charset=\"" + String.escaped(name) + "\">";
//   let mobile = "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"/>";
//   let description = (description) => "<meta name=\"description\" content=\"" + description + "\">";
// };
// let html = (~attrs=[], ~children, ()) => "<!DOCTYPE HTML>\n" + normal("html", [], ~attrs, ~children, ());

// let fragment = (~children, ()) => String.concat("\n", children);

// let time = (~datetime) => contentTag("time", [("datetime", Some(datetime))]);
// let img = (~src, ~alt) => contentTag("img", [("src", Some(src)), ("alt", Some(alt))]);
// let body = contentTag("body", []);
// let section = contentTag("section", []);
// let article = contentTag("article", []);
// let main = contentTag("main", []);
// let header = contentTag("header", []);
// let footer = contentTag("footer", []);
// let h1 = contentTag("h1", []);
// let h2 = contentTag("h2", []);
// let h3 = contentTag("h3", []);
// let h4 = contentTag("h4", []);
// let div = contentTag("div", []);
// let span = contentTag("span", []);
// let ul = contentTag("ul", []);
// let li = contentTag("li", []);
// let p = contentTag("p", []);
// let a = (~href, ~target=?) => contentTag("a", [("href", Some(href)), ("target", target)]);

// let script = (~src) => normal("script", [("src", src)]);
// let asyncScript = (~src) => normal("script", [("async", "true"), ("src", src)]);
// let escapeScript = str => str; /* TODO impl */
// let inlineScript = (~children, ()) => normal("script",  [], ~children=List.map(escapeScript, children), ());
// let style = normal("style", []);

// let head = normal("head", []);
// let title = normal("title", []);
