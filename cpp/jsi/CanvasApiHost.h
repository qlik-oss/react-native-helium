//
// Created by Vittorio Cellucci on 2022-05-01.
//

#ifndef HELIUM4_CANVASAPIHOST_H
#define HELIUM4_CANVASAPIHOST_H

#include <jsi/jsi.h>
#include <memory>

using namespace facebook::jsi;

class CanvasApiHost : public HostObject {
public:
    Value get(Runtime &runtime, const PropNameID &name) override;
    void set(Runtime &runtime, const PropNameID &name, const Value& value) override;

    static void install(Runtime& runtime) {
        std::shared_ptr<CanvasApiHost> binding = std::make_shared<CanvasApiHost>();
        auto object = Object::createFromHostObject(runtime, binding);
        runtime.global().setProperty(runtime, "HeliumCanvasApi", object);
    }
};


#endif //HELIUM4_CANVASAPIHOST_H
