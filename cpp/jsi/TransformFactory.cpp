//
// Created by Vittorio Cellucci on 2022-05-04.
//

#include "TransformFactory.h"
#include <string>
#include <regex>
#include <sstream>
#include "Helium.h"

inline std::vector<float> split(const std::string& s)
{
    std::vector<float> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, ',')) {
        tokens.push_back(std::stof(token));
    }
    return tokens;
}

TransformFactory::TransformFactory(const SkMatrix& from) : matrix(from){}

SkMatrix TransformFactory::parse(jsi::Runtime &rt, const jsi::Object &object) {
    std::string s = object.getProperty(rt, "transform").toString(rt).utf8(rt);
    std::regex rgx("\\s(?=\\S+\\(.*?\\))");
    std::sregex_token_iterator begin(s.begin(), s.end(), rgx, -1);
    std::sregex_token_iterator end;

    for(auto it = begin; it != end; ++it) {
        std::string operation = *it;
        std::regex ogx("(\\w+)\\((.+?)\\)");
        std::smatch matches;
        if ( std::regex_search(operation, matches, ogx) ) {
           if(matches[1] == "rotate") {
               auto values = split(matches[2]);
               // skia does not accept negative values
               auto deg = fmod(360.0f + values[0], 360.0f);
               SkMatrix tmp;
               if(values.size() == 3) {
                   tmp.preRotate(deg, Helium::toPx(values[1]), Helium::toPx(values[2]));
                   tmp.preConcat(matrix);
                   matrix = tmp;
               } else {
                   tmp.preRotate(deg);
                   tmp.preConcat(matrix);
                   matrix = tmp;
               }
           } else if(matches[1] == "translate") {
             auto values = split(matches[2]);
             auto T = SkMatrix::Translate(Helium::toPx(values[0]), Helium::toPx(values[1]));
             matrix = SkMatrix::Concat(matrix, T);
           } else if(matches[1] == "scale") {
             auto values = split(matches[2]);
             auto S = SkMatrix::Scale(Helium::toPx(values[0]),  Helium::toPx(values[0]));
             matrix = SkMatrix::Concat(matrix, S);

           }
        }
    }
    return matrix;
}
