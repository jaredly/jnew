type item = {
  id: int,
  title: string,
  completed: bool
};

let str = ReasonReact.string;

module TodoItem = {
  let component = ReasonReact.statelessComponent("TodoItem");
  let make = (~item, children) => {
    ...component,
    render: (_) =>
      <div className="item">
        <input
          type_="checkbox"
          checked=((item.completed))
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
  | AddItem;

let component = ReasonReact.reducerComponent("TodoApp");

let lastId = 0;
let newItem = () => {
  let lastId = lastId + 1;
  {id: lastId, title: "Click a button", completed: true}
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
        (ReasonReact.array(
          Array.of_list(
            List.map((item) => <TodoItem item />, items)
          )
        ))
      </div>
      <div className="footer">
        (str(string_of_int(numItems) ++ " items"))
      </div>
    </div>
  }
};