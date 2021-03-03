// ok

namespace JSX {
    type props = {
        className?: string;
        id?: string;
        lang?: string;
        title?: string;
        style?: string;
        itemprop?: string;
        itemtype?: string;
        itemscope?: string;
        role?: string;
        name?: string;
    };
    export interface IntrinsicElements {
        html: props;
        link: { rel: string; href: string; media?: string };
        meta:
            | { name: string; content: string }
            | { charset: string }
            | { property: string; content: string };
        div: props;
        time: props & { datetime?: string };
        img: props & { src: string; alt?: string };
        body: props;
        section: props;
        article: props;
        main: props;
        header: props;
        footer: props;
        h1: props;
        h2: props;
        h3: props;
        h4: props;
        h4: props;
        span: props;
        ul: props;
        li: props;
        p: props;
        a: props & { href?: string; target?: string };
        script: { src: string; async?: boolean };
        style: props;
        head: props;
        title: props;
        fragment: props;
    }
}
