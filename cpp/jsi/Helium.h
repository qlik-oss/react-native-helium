//
// Created by Vittorio Cellucci on 2022-05-02.
//

#ifndef HELIUM4_HELIUM_H
#define HELIUM4_HELIUM_H

#include <jsi/jsi.h>
#include <string>
#include <sstream>
#include "CanvasViewManger.h"

using namespace facebook;
namespace Helium {
    template<typename T, typename U>
    static T toPx(T dp, U s) {
        return dp * s;
    }

    template<typename T>
    static T toPx(T dp) {
        return dp * TheCanvasViewManager::instance()->scale;
    }

    template<typename T, typename U>
    static T toDB(T px, U s) {
        return (px / s );
    }

    template<typename T>
    static T toDB(T px) {
        return (px / TheCanvasViewManager::instance()->scale);
    }

    class VirtualContext {
    public:
        std::string id;
        std::string vid;
        VirtualContext() = default;
        VirtualContext(jsi::Runtime& rt, const jsi::Value& val) {
            auto obj = val.asObject(rt);
            id = obj.getProperty(rt, "id").asString(rt).utf8(rt);
            vid = obj.getProperty(rt, "vid").asString(rt).utf8(rt);
        }
    };

    template<typename T>
    std::vector<T> split(const std::string& s)
    {
        std::vector<T> tokens;
        std::string token;
        std::istringstream tokenStream(s);
        while (std::getline(tokenStream, token, ',')) {
            tokens.push_back(std::stof(token));
        }
        return tokens;
    }
}

#endif //HELIUM4_HELIUM_H
