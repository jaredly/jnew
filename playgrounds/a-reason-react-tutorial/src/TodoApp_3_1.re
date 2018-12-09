type item = {
  id: int,
  title: string,
  completed: bool
};

let str = ReasonReact.string;

module TodoItem = {
  let component = ReasonReact.statelessComponent("TodoItem");
  let make = (~item, ~onToggle, children) => {
    ...component,
    render: (_) =>
      <div className="item" onClick=((_evt) => onToggle())>
        <input
          type_="checkbox"
          checked=((item.completed))
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

let component = ReasonReact.reducerComponent("TodoApp");

let lastId = ref(0);
let newItem = () => {
  lastId := lastId^ + 1;
  {id: lastId^, title: "Click a button", completed: true}
};

let make = (children) => {
  ...component,
  initialState: () => {
    items: [{
      id: 0,
      title: "Write some things to do",
      completed: false
    }]
  },
  reducer: (action, {items}) => switch action {
    | AddItem => ReasonReact.Update({items: [newItem(), ...items]})
    | ToggleItem(id) =>
      let items = List.map(
        (item) =>
          item.id === id ?
            {...item, completed: ! item.completed} : item,
        items
      );
      ReasonReact.Update({items: items})
  },
  render: ({state: {items}, send}) => {
    let numItems = List.length(items);
    <div className="app">
      <div className="title">
        (str("What to do"))
        <button onClick=((_evt) => send(AddItem))>
          (str("Add something"))
        </button>
      </div>
      <div className="items">
        (ReasonReact.array(Array.of_list(
          List.map(
            (item) => <TodoItem
              key=(string_of_int(item.id))
              onToggle=(() => send(ToggleItem(item.id)))
              item
            />, items
          )
        )))
      </div>
      <div className="footer">
        (str(string_of_int(numItems) ++ " items"))
      </div>
    </div>
  }
};