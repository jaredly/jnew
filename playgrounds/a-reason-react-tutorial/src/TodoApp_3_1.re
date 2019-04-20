type item = {
  id: int,
  title: string,
  completed: bool
};

let str = React.string;

module TodoItem = {
  [@react.component]
  let make = (~item, ~onToggle) => {
    <div className="item" onClick=((_evt) => onToggle())>
      <input
        type_="checkbox"
        checked=(item.completed)
        /* TODO make interactive */
      />
      (str(item.title))
    </div>
  };
};

type state = {
  items: list(item)
};
type action =
  | AddItem
  | ToggleItem(int);

let lastId = ref(0);
let newItem = () => {
  lastId := lastId^ + 1;
  {id: lastId^, title: "Click a button", completed: true}
};

[@react.component]
let make = () => {
  let ({items}, dispatch) = React.useReducer((state, action) => {
    switch action {
    | AddItem => {items: [newItem(), ...state.items]}
    | ToggleItem(id) =>
      let items = List.map(
        (item) =>
          item.id === id
            ? {...item, completed: !item.completed}
            : item,
        state.items
      );
      {items: items}
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
      (React.array(Array.of_list(
        List.map(
          (item) => <TodoItem
            key=(string_of_int(item.id))
            onToggle=(() => dispatch(ToggleItem(item.id)))
            item
          />, items
        )
      )))
    </div>
    <div className="footer">
      (str(string_of_int(numItems) ++ " items"))
    </div>
  </div>
};