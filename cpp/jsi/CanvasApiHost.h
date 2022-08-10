//
// Created by Vittorio Cellucci on 2022-05-01.
//

#ifndef HELIUM4_CANVASAPIHOST_H
#define HELIUM4_CANVASAPIHOST_H

#include <jsi/jsi.h>
#include <memory>

using namespace facebook::jsi;

class CanvasApiHost {
public:
  static void install(Runtime& runtime);
};


#endif //HELIUM4_CANVASAPIHOST_H
