# 🌳 Tree_DS

A collection of implementations of popular tree-based data structures and heaps in C++.

This project was created for educational purposes and includes classic algorithms for data storage, search, and balancing.

## Description

### Binary Search Tree (BST)

A binary search tree in which, for each node:

* the left subtree contains smaller values;
* the right subtree contains larger values.


Complexity of operations:

| Operation | Average  | Worst |
| -------- | -------- | ------ |
| Search   | O(log n) | O(n)   |
| Insert   | O(log n) | O(n)   |
| Delete   | O(log n) | O(n)   |

---

### AVL Tree

A self-balancing binary search tree.

After each insertion or deletion, the tree is rotated to maintain balance.

Complexity of operations:

| Operation | Complexity |
| -------- | --------- |
| Search   | O(log n)  |
| Insert   | O(log n)  |
| Delete   | O(log n)  |

---

### B+ Tree

A balanced tree structure widely used in:

* DBMS;
* file systems;
* indexing large amounts of data.

Features:

* All data is stored in leaf nodes;
* Leaf nodes are linked to one another;
* Efficient range search.

Complexity of operations:

| Operation | Complexity |
| -------- | --------- |
| Search   | O(log n)  |
| Insert   | O(log n)  |
| Delete   | O(log n)  |

---

### Binary Heap

A complete binary tree that satisfies the heap property.

Used for:

* Priority Queue;
* Dijkstra's algorithm;
* Heap Sort.

Operation Complexity:

| Operation        | Complexity |
| --------------- | --------- |
| Insert          | O(log n)  |
| Extract Min/Max | O(log n)  |
| Peek            | O(1)      |

---

### Fibonacci Heap

An improved heap with more efficient amortized operations.

Used in:

* Dijkstra's algorithm;
* Prim's algorithm;
* graph optimization problems.

Amortized complexity:

| Operation     | Complexity |
| ------------ | --------- |
| Insert       | O(1)      |
| Merge        | O(1)      |
| Decrease Key | O(1)      |
| Extract Min  | O(log n)  |

---

## Project Goal

The project is intended for:

* studying data structers
* studying the implementation of different tree DS
