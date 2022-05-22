//
//  ObjectPool.h
//  Pods
//
//  Created by Vittorio Cellucci on 2022-05-13.
//

#ifndef ObjectPool_h
#define ObjectPool_h

#include <memory>
#include <stack>
#include <cstring>
#include <cstdlib>
#include <mutex>

template<class T>
class ObjectPool {
public:
  ObjectPool() = default;
  ~ObjectPool() {
    std::unique_lock lk(poolMutex);
    // free everything
    while (!memoryPool.empty()) {
      T* ptr = memoryPool.top();
      memoryPool.pop();
      free(ptr);
    }
  }

  template<typename... argsT>
  std::shared_ptr<T> create(argsT&&... args) {
    std::unique_lock lk(poolMutex);
    T* ptr = nullptr;
    if(memoryPool.empty()) {
      ptr = reinterpret_cast<T*>(malloc(sizeof(T)));
    } else {
      ptr = memoryPool.top();
      memoryPool.pop();
    }
    ptr = new(ptr)T(std::forward<argsT>(args)...);
    auto item = createShared(ptr);
    return item;
  }

private:
  std::shared_ptr<T> createShared(T* ptr) {
    return std::shared_ptr<T>(ptr, [this](T* monitored){
      std::unique_lock lk(poolMutex);
      monitored->~T();
      memoryPool.push(monitored);
    });
  }

private:
  std::mutex poolMutex;
  std::stack<T*> memoryPool;
};
#endif /* ObjectPool_h */
