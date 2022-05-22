//
//  Platform.h
//  Pods
//
//  Created by Vittorio Cellucci on 2022-05-17.
//

#ifndef Platform_h
#define Platform_h

#include <jsi/jsi.h>
#include <ReactCommon/CallInvoker.h>
#include <memory>

using namespace facebook;

class Platform {
public:
  Platform(jsi::Runtime& _rt, std::shared_ptr<react::CallInvoker> jci)
  : rt(_rt)
  , jsCallInvoker(jci){}
public:
  jsi::Runtime& rt;
  std::shared_ptr<react::CallInvoker> jsCallInvoker;
};

#endif /* Platform_h */
