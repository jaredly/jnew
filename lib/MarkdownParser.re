
/* let inlineLinker = {
  pub parser_extension = (parsed_so_far, prev, tokens) => {
    open Omd_representation;
    print_endline("check");
    switch tokens {
    /* | [Word(("http" | "https") as protocol), Colon, Slashs(0), ...rest] => {
print_endline("hi");
      let rec loop = (link, tokens) => switch tokens {
      | []
      | [(Space | Spaces(_) | Dot | Dots(_)), ..._] => ([Url(link, [Text(link)], ""), ...parsed_so_far], prev, tokens)
      | [tok, ...rest] => loop(link ++ Omd_lexer.string_of_token(tok), rest)
      };
      Some(loop(protocol ++ "://", rest))
    } */
    /* | [Word(_), ...rest] => {
print_endline("ho");
      None
    } */
    | [tok, ...rest] => {
print_endline(Omd_lexer.string_of_token(tok));
      None
    }
    | _ => None
    }
  };
  pub to_string = "";
}; */

let parse = rawBody => (Omd.to_html(Omd.of_string(~extensions=[], rawBody)));