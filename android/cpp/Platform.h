//
// Created by Vittorio Cellucci on 2022-11-19.
//

#ifndef QLIKSENSEMOBILE_PLATFORM_H
#define QLIKSENSEMOBILE_PLATFORM_H

#include <jsi/jsi.h>
#include <ReactCommon/CallInvoker.h>
#include <memory>

using namespace facebook;

class Platform {
public:
  Platform(jsi::Runtime& _rt)
          : rt(_rt){}
public:
  jsi::Runtime& rt;
};

#endif //QLIKSENSEMOBILE_PLATFORM_H
