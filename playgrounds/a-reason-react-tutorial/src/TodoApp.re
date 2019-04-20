[@react.component]
let make = (~title) => {
  <div className="app">
    <div className="title">
      (React.string(title))
    </div>
    <div className="items">
      (React.string("Nothing"))
    </div>
  </div>
};

let hidden = "cool";
/* in future snippets there will be real code hidden */