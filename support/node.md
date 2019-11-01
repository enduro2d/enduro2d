# Node

## Properties

- `owner: gobject, RO`

- `transform: t3f, RW`
- `translation: v3f, RW`
- `rotation: q4f, RW`
- `scale: v3f, RW`

- `local_matrix: m4f, RO`
- `world_matrix: m4f, RO`

- `root: node, RO`
- `parent: node, RO`

- `child_count: number, RO`
- `child_count_recursive: number, RO`

- `first_child: node, RO`
- `last_child: node, RO`
- `prev_sibling: node, RO`
- `next_sibling: node, RO`

## Functions

- `remove_from_parent() -> boolean`
- `remove_all_children() -> number`

- `add_child(child: node) -> boolean`
- `add_child_to_back(child: node) -> boolean`
- `add_child_to_front(child: node) -> boolean`
- `add_child_before(before: node, child: node) -> boolean`
- `add_child_after(after: node, child: node) -> boolean`
- `add_sibling_before(sibling: node) -> boolean`
- `add_sibling_after(sibling: node) -> boolean`
- `remove_child(child: node) -> boolean`

- `send_backward() -> boolean`
- `bring_to_back() -> boolean`
- `send_forward() -> boolean`
- `bring_to_front() -> boolean`

