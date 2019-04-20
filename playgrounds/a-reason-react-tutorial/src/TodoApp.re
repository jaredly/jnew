[@react.component]
let make = (~children) => {
  <div className="app">
    <div className="title">
      (ReasonReact.string("What to do"))
    </div>
    <div className="items">
      (ReasonReact.string("Nothing"))
    </div>
  </div>
};

let hidden = "cool";
/* in future snippets there will be real code hidden */