type item = {
  id: int,
  title: string,
  completed: bool
};

let str = ReasonReact.string;

module TodoItem = {
  [@react.component]
  let make = (~item) => {
    <div className="item">
      <input
        type_="checkbox"
        checked=(item.completed)
        /* TODO make interactive */
      />
      (str(item.title))
    </div>
  };
};

type state = {items: list(item)};

type action =
  | AddItem;

let lastId = 0;
let newItem = () => {
  let lastId = lastId + 1;
  {id: lastId, title: "Click a button", completed: true}
};

[@react.component]
let make = () => {
  let ({items}, dispatch) = React.useReducer((state, action) => {
    switch action {
    | AddItem => {items: [newItem(), ...state.items]}
    }
  }, {
    items: [{
      id: 0,
      title: "Write some things to do",
      completed: false
    }]
  });
  let numItems = List.length(items);
  <div className="app">
    <div className="title">
      (str("What to do"))
      <button onClick=((_evt) => dispatch(AddItem))>
        (str("Add something"))
      </button>
    </div>
    <div className="items">
      (
        ReasonReact.array(Array.of_list(
            List.map((item) => <TodoItem item />, items)
        ))
      )
    </div>
    <div className="footer">
      (str(string_of_int(numItems) ++ " items"))
    </div>
  </div>
};