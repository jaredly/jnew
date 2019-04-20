[@react.component]
let make = (~title) => {
  <div className="app">
    <div className="title">
      (ReasonReact.string(title))
    </div>
    <div className="items">
      (ReasonReact.string("Nothing"))
    </div>
  </div>
};

let hidden = "cool";
/* in future snippets there will be real code hidden */