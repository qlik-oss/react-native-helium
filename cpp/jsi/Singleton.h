//
// Created by Vittorio Cellucci on 2022-05-01.
//

#ifndef HELIUM4_SINGLETON_H
#define HELIUM4_SINGLETON_H

#include <memory>

template<typename T>
class Singleton {
public:
    static T* instance();
    Singleton(const Singleton&) = delete;
    Singleton& operator= (const Singleton) = delete;

};

template<typename T>
T* Singleton<T>::instance() {
    static const std::unique_ptr<T> instance(new T());
    return instance.get();
}


#endif //HELIUM4_SINGLETON_H
