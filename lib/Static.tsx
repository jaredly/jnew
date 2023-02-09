/** @jsx jsx */
import { startPage } from './Css';
import { process } from './MarkdownParser';
import { PageHead, PageWithTopAndBottom, Styles } from './Shared';
import { A, CssFn, Hover, jsx, Media } from './sjsx';

let renderInner = ({
    contentTitle,
    description,
    rawBody,
}: {
    contentTitle: string;
    description: string;
    rawBody: string;
}) => {
    let { css, inlineCss } = startPage();

    let body = (
        <PageWithTopAndBottom
            css={css}
            top={
                <h1 className={css(Styles.titleWithTopMargin)}>
                    {contentTitle}
                </h1>
            }
            middle={
                <div className={css(Styles.bodyText)}> {process(rawBody)} </div>
            }
            bottom={'This is the personal site of Jared Forsyth'}
        />
    );

    return (
        <html>
            <PageHead title={contentTitle} description={description}>
                <style>{inlineCss()}</style>
            </PageHead>
            {body}
        </html>
    );
};

// export let render = (fileName: string, opts: opts, rawBody: string) => {
//   let opts = opts |! "No options for static file " ++ fileName;
//   let title = Toml.string("title", opts) |! "No title for static page " ++ fileName;
//   let description = Toml.string("description", opts);
//   return (renderInner(title, description, rawBody))
// };
