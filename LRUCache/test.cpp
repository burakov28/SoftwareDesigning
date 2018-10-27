#include "lru-cache.h"

#include <iostream>
#include <cassert>

using namespace std;

int main() {
  int n;
  cin >> n;
  LRUCache<int, int> cache(n);

  for (; ; ) {
    string s;
    cin >> s;
    if (s == "set" || s == "s") {
      int key, val;
      cin >> key >> val;
      cache.set(key, val);
    } else if (s == "get" || s == "g") {
      int key;
      cin >> key;
      auto res = cache.get(key);
      if (res) {
        cout << res.value() << "\n";
      } else {
        cout << "ERROR!!! Value not found\n";
      }
    } else if (s == "print" || s == "p") {
      for (const auto &p : cache.getLruOrder()) {
        cout << p.first << " " << p.second << "  ";
      }
      cout << "\n";
    } else if (s == "erase" || s == "e") {
      int key;
      cin >> key;
      if (cache.erase(key)) {
        cout << "Removed successfully!\n";
      } else {
        cout << "Element not found!\n";
      }
    } else if (s == "exit") {
      cout << "Exiting...\n";
      break;
    } else {
      assert(false);
    }

    cout << "\n";
  }

  return 0;
}
