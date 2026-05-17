# Distributed Cache System

> An in-memory distributed cache in C++ combining **LRU eviction** with **Consistent Hashing** to route keys across multiple nodes — with automatic data migration on node removal.

---

## Table of Contents

- [Overview](#overview)
- [Architecture](#architecture)
- [Components](#components)
- [How Consistent Hashing Works](#how-consistent-hashing-works)
- [Build & Run](#build--run)
- [Example Session](#example-session)
- [Design Decisions](#design-decisions)
- [Limitations & Future Improvements](#limitations--future-improvements)

---

## Overview

This project simulates a distributed caching layer where multiple cache nodes share the load of storing key-value pairs. Keys are routed to the correct node using a consistent hash ring, and each node manages its own bounded LRU cache. When a node is removed, its data is automatically rehashed and redistributed to surviving nodes — minimizing data loss and disruption.

---

## Architecture

```
┌──────────────────────────────────────────┐
│           distributed_cache              │
│         (top-level interface)            │
└───────────────────┬──────────────────────┘
                    │
        ┌───────────▼───────────┐
        │     consistent_hash   │
        │   (std::map hash ring)│
        └──────┬────────────────┘
               │              │
     ┌─────────▼──┐    ┌──────▼─────────┐
     │ cache_node │    │   cache_node   │  . . .
     │   Node A   │    │    Node B      │
     └─────┬──────┘    └──────┬─────────┘
           │                  │
     ┌─────▼──────┐    ┌──────▼─────────┐
     │  LRUcache  │    │   LRUcache     │
     │ (DLL + map)│    │  (DLL + map)   │
     └────────────┘    └────────────────┘
```

---

## Components

### `lru.cpp` — LRU Cache

The core eviction engine. Uses a **doubly linked list** paired with an `unordered_map` to achieve O(1) get and put operations.

| Method | Description |
|---|---|
| `get(key)` | Returns the value or `-1` if not found. Moves the node to the front (most recently used). |
| `put(key, val)` | Inserts or updates. Evicts the least recently used entry when at capacity. |
| `getAllData()` | Returns all stored key-value pairs — used during node removal for data migration. |
| `display()` | Prints the current cache contents in order (MRU → LRU). |

---

### `cache_node.cpp` — Cache Node

A logical cache server — wraps an `LRUcache` with a unique ID and a fixed capacity.

| Method | Description |
|---|---|
| `put(key, val)` | Delegates to the internal LRU cache. |
| `get(key)` | Delegates to the internal LRU cache. |
| `getNodeData()` | Exports all stored pairs for migration on removal. |
| `display()` | Prints the node ID and its cache state. |

---

### `consistent_hash.cpp` — Hash Ring

Manages node placement on a conceptual ring using a `std::map<unsigned long long, cache_node*>`, sorted by hash value.

| Method | Description |
|---|---|
| `add_node(node)` | Hashes the node ID and inserts it into the ring. |
| `remove_node(id)` | Erases the node from the ring by its hash. |
| `get_node(key)` | Hashes the key, runs `lower_bound` to find the first clockwise node, wraps to ring start if needed. |
| `display_ring()` | Prints all nodes on the ring in hash order. |

---

### `distributed_cache.cpp` — Distributed Cache

The top-level interface that ties everything together.

| Method | Description |
|---|---|
| `add_node(id, capacity)` | Creates a new node and registers it on the hash ring. |
| `remove_node(id)` | Removes a node, retrieves its data, and **rehashes all keys** to surviving nodes. |
| `put(key, val)` | Routes a write to the correct node via the hash ring. |
| `get(key)` | Routes a read to the correct node via the hash ring. |
| `display()` | Prints the full system state — ring layout and all node caches. |

---

### `main.cpp` — CLI Interface

Interactive menu for manually testing all operations.

```
=========================================
 DISTRIBUTED CACHE SYSTEM
 LRU + CONSISTENT HASHING
=========================================

1. Add Cache Node
2. Remove Cache Node
3. PUT Key-Value
4. GET Value
5. Display System
6. Exit
```

---

## How Consistent Hashing Works

In a standard hash map, adding or removing a bucket remaps nearly all keys. Consistent hashing solves this:

1. Both **nodes** and **keys** are hashed onto a conceptual ring spanning `0 → 2^64`.
2. Each key is owned by the **first node clockwise** from its position on the ring.
3. When a node is added or removed, **only keys in that node's segment** are affected — all others remain stable.

```
Ring (simplified):

  0 ──── NodeC(200) ──── NodeA(450) ──── NodeB(780) ──── 2^64
                                │
              key(hash=520) ────┘ → owned by NodeB (first clockwise)
              key(hash=900) wraps → owned by NodeC (back to start)
```

This keeps data movement minimal during scaling events.

---

## Build & Run

### Prerequisites

- C++17 or later
- `g++` or any compatible compiler (no external dependencies)

### Compile

```bash
g++ -std=c++17 main.cpp distributed_cache.cpp consistent_hash.cpp cache_node.cpp lru.cpp -o cache_system
```

### Run

```bash
./cache_system
```

---

## Example Session

```
> Add Node: id="NodeA"  capacity=3
  Node Added — NodeA  hash=7823649201...

> Add Node: id="NodeB"  capacity=3
  Node Added — NodeB  hash=2941830047...

> PUT  user:1 → 100        # routed to NodeA via hash ring
> PUT  user:2 → 200        # routed to NodeB via hash ring
> GET  user:1  →  100      # served by NodeA

> Remove Node: NodeA
  Migrating NodeA data to surviving nodes...
  Node Removed Successfully.

> GET  user:1  →  100      # still returns 100, now served by NodeB
```

---

## Design Decisions

| Decision | Rationale |
|---|---|
| `std::map` for the hash ring | Keeps nodes sorted by hash; `lower_bound` gives O(log n) clockwise lookup with automatic ordering. |
| `std::unordered_map` in LRU | O(1) average key lookup; pairs with the linked list for O(1) eviction. |
| Doubly linked list in LRU | O(1) node removal and front-insertion without list traversal. |
| Dummy head/tail nodes | Eliminates all edge-case checks for empty list and single-element operations. |
| Data migration on removal | Preserves cache consistency when a node goes offline by rehashing its keys to survivors. |

---

## Limitations & Future Improvements

- **No virtual nodes (vnodes):** A production consistent hashing system places each physical node at multiple ring positions for even load distribution. This implementation uses one point per node, which can cause uneven key distribution.

- **Integer values only:** The cache stores `int` values. Templating the `LRUcache` class would support arbitrary value types.

- **No thread safety:** Concurrent reads and writes are unprotected. A production system would use per-node `std::mutex` or reader-writer locks.

- **Hash collisions between nodes:** Two nodes with the same hash would silently overwrite each other on the ring. Node IDs should be enforced as unique.

- **No persistence:** All data lives in memory and is lost on exit. A production cache layer would integrate a write-ahead log or snapshot mechanism.
