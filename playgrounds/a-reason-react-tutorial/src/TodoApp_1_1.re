type item = {
  title: string,
  completed: bool
};

type state = {
  /* this is a type w/ a type argument,
  * similar to List<Item> in TypeScript,
  * Flow, or Java */
  items: list(item)
};

// I've gone ahead and made a shortened name for converting strings to elements
let str = React.string;

[@react.component]
let make = () => {
  let ({items}, dispatch) = React.useReducer((state, action) => {
    // We don't have state transitions yet
    state
  }, {
    items: [
      {title: "Write some things to do", completed: false}
    ]
  });
  let numItems = List.length(items);
  <div className="app">
    <div className="title"> (str("What to do")) </div>
    <div className="items"> (str("Nothing")) </div>
    <div className="footer">
      (str(string_of_int(numItems) ++ " items"))
    </div>
  </div>
};