type item = {
  title: string,
  completed: bool
};

type state = {items: list(item)};

type action =
  | AddItem;

let newItem = () => {title: "Click a button", completed: true};

let str = ReasonReact.string;

[@react.component]
let make = () => {
  let ({items}, dispatch) = React.useReducer((state, action) => {
    switch action {
    | AddItem => {items: [newItem(), ...items]}
    }
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
        onClick=((_evt) => dispatch(AddItem))
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