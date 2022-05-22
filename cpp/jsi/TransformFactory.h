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
    SkMatrix parse(jsi::Runtime& rt, const jsi::Object& object);
};


#endif //HELIUM4_TRANSFORMFACTORY_H
