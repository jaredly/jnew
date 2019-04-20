type item = {
  title: string,
  completed: bool
};
type state = {
  items: list(item)
};

let newItem = () => {title: "Click a button", completed: true};

let str = React.string;
[@react.component]
let make = () => {
  let ({items}, dispatch) = React.useReducer((state, action) => {
    state
  }, {
    items: [{
      title: "Write some things to do",
      completed: false
    }]
  });
  let numItems = List.length(items);
  <div className="app">
    <div className="title">
      (str("What to do"))
      <button
        onClick=((_evt) => Js.log("didn't add something"))
      >
        (str("Add something"))
      </button>
    </div>
    <div className="items"> (str("Nothing")) </div>
    <div className="footer">
      (str(string_of_int(numItems) ++ " items"))
    </div>
  </div>
};