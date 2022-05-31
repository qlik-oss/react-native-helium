//
// Created by Vittorio Cellucci on 2022-05-04.
//

#ifndef HELIUM4_TRANSFORMFACTORY_H
#define HELIUM4_TRANSFORMFACTORY_H

#include <jsi/jsi.h>
#include <include/core/SkMatrix.h>

using namespace facebook;

class TransformFactory {
public:
    TransformFactory() = default;
  TransformFactory(const SkMatrix& from);
    SkMatrix parse(jsi::Runtime& rt, const jsi::Object& object);
  
protected:
  SkMatrix matrix;
};


#endif //HELIUM4_TRANSFORMFACTORY_H
