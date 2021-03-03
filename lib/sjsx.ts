// Ok

import { normal } from './Html';

export const jsx = (
    name: string | ((args: any & { children: string[] }) => string),
    args: any,
    ...children: string[]
): string => {
    // console.log(name, children.length);
    if (name === 'fragment') {
        return children.join('\n');
    }
    if (typeof name === 'string') {
        return normal(name, args ? Object.entries(args) : [], {
            children,
            attrs: [],
        });
    }
    const res = name({ ...args, children });
    if (res == null) {
        console.log(name);
    }
    return res;
};

const styles = [];
export type CssAttr =
    | { type: 'A'; name: string; value: string }
    | {
          type: 'Media' | 'Sub';
          selector: string;
          children: [string, string][];
      }
    | { type: 'Hover'; children: [string, string][] };
export const A = (name: string, value: string): CssAttr => ({
    type: 'A',
    name,
    value,
});
export const Hover = (children: [string, string][]): CssAttr => ({
    type: 'Hover',
    children,
});
export const Sub = (
    selector: string,
    children: [string, string][],
): CssAttr => ({
    type: 'Sub',
    selector,
    children,
});
export const Media = (
    selector: string,
    children: [string, string][],
): CssAttr => ({
    type: 'Media',
    selector,
    children,
});
export const css = (attributes: CssAttr[]) => {
    // STOPSHIP
    return 'background: red';
};
export type CssFn = (attributes: CssAttr[]) => string;
