#ifndef LEAST_RECENTLY_USED_HPP
#define LEAST_RECENTLY_USED_HPP
#include <map>
#include<iostream>
#include<assert.h>

namespace LRU {

template<class K, class V>
struct node {
  K key;
  V value;
  node<K,V>* next;
  node<K,V>* prev;
  node(K key, V value,
      node<K,V>* next = nullptr,
      node<K,V>* prev = nullptr);
};

template<class K, class V>
class lru {
 private:
  node<K,V>* head;
  node<K,V>* tail;
  std::map<K,node<K,V>*> map;
  std::size_t capacity;
  void bump(node<K,V>*);
  void pop();
 public:
  explicit lru(std::size_t);
  void put(const K &key, const V &value);
  V get(const K &key);
};

template<class K, class V>
inline node<K,V>::node(K key, V value, node<K,V>* next, node<K,V>* prev)
    : key {std::move(key)}
    , value {std::move(value)}
    , next(next)
    , prev(prev)
{}

template<class K, class V>
inline lru<K,V>::lru(std::size_t capacity)
    : capacity(capacity)
    , head(nullptr)
    , tail(nullptr) {
        assert(capacity > 1);
}

template<class K, class V>
void lru<K,V>::put(const K &key, const V &value) {
  auto it = map.find(key);
  if (it != map.end()) {
    auto temp = it->second;
    temp->value = value;
    bump(temp);
    return;
  }

  if (capacity == map.size())
    pop();

  auto temp = new node<K,V>(key, value, head);
  if (head != nullptr)
    head->prev = temp;
  else
    tail = temp;
  head = temp;
  map.insert(std::pair<K,node<K,V>*> {key, temp});
}

template<class K, class V>
void lru<K,V>::pop() {
  tail->prev->next = tail->next;
  map.erase(tail->key);
  tail = tail->prev;
  delete tail->next;
  tail->next = nullptr;
}

template<class K, class V>
V lru<K,V>::get(const K &key) {
  auto it = map.find(key);
  if (it != map.end()) {
    bump(it->second);
    return it->second->value;
  }
  return V {};
}

template<class K, class V>
void lru<K,V>::bump(node<K,V>* temp) {
  if (temp->prev != nullptr)
    temp->prev->next = temp->next;
  if (temp->next != nullptr)
    temp->next->prev = temp->prev;
  if (tail == temp)
    tail = temp->prev != nullptr ? temp->prev : temp;
  if (head != temp) {
    head->prev = temp;
    temp->next = head;
    head = temp;
  }
}

}
#endif
