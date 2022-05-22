//
//  QuadTree.hpp
//  qlik-oss-react-native-helium
//
//  Created by Vittorio Cellucci on 2022-05-16.
//

#ifndef QuadTree_hpp
#define QuadTree_hpp

#include <vector>
#include <memory>
#include <unordered_set>
#include <include/core/SkRect.h>

template<class T>
class Quadtree {
private:
  static constexpr int MAX_LEVELS = 5;
  static constexpr int MAX_OBJECTS = 10;
  int level = 0;
  std::vector<T> objects;
  std::vector<std::shared_ptr<Quadtree>> nodes;
  SkRect bounds;
  
public:
  using FoundNodes = std::unordered_set<T>;
  Quadtree(int newLevel, const SkRect& newBounds): level(newLevel), bounds(newBounds) {
    
  }
  
  void clear() {
    objects.clear();
    
    for(auto&& node : nodes) {
      if(node) {
        node->clear();
      }
    }
    nodes.clear();
  }
  
  void split() {
    auto subWidth = bounds.width() / 2;
    auto subHeight = bounds.height() / 2;
    auto x = bounds.x();
    auto y = bounds.y();
    nodes.clear();
    
    nodes.emplace_back(std::make_shared<Quadtree>(level + 1, SkRect::MakeXYWH(x + subWidth, y, subWidth, subHeight)));
    nodes.emplace_back(std::make_shared<Quadtree>(level + 1, SkRect::MakeXYWH(x, y, subWidth, subHeight)));
    nodes.emplace_back(std::make_shared<Quadtree>(level + 1, SkRect::MakeXYWH(x, y + subHeight, subWidth, subHeight)));
    nodes.emplace_back(std::make_shared<Quadtree>(level + 1, SkRect::MakeXYWH(x + subWidth, y + subHeight, subWidth, subHeight)));
  }
  
  std::vector<int> getIndex(const SkRect& rect) {
    std::vector<int> idx;
    idx.reserve(4);
    auto cx = bounds.x() + bounds.width() * 0.5;
    auto cy = bounds.y() + bounds.height() * 0.5;
    
    bool top = rect.y() < cy;
    bool left = rect.x() < cx;
    bool right = rect.x() + rect.width() > cx;
    bool bottom = rect.y() + rect.height() > cy;
    
    if (top && right) {
      idx.push_back(0);
    }
    
    if (top && left) {
      idx.push_back(1);
    }
    
    if (bottom && left) {
      idx.push_back(2);
    }
    
    if (bottom && right) {
      idx.push_back(3);
    }
    
    return idx;
  }
  
  void insert(const SkRect& pRect, T& object) {
    if(!nodes.empty()) {
      std::vector<int> idx;
      idx = getIndex(pRect);
      for(auto i : idx) {
        nodes[i]->insert(pRect, object);
      }
      return;
    }
    
    objects.push_back(object);
    
    if(objects.size() > MAX_OBJECTS && level < MAX_LEVELS) {
      if(nodes.empty()) {
        split();
      }
      for(auto&& object : objects) {
        std::vector<int> idx = getIndex(object->getAAB()->getRect());
        for(int i : idx) {
          nodes[i]->insert(object->getAAB()->getRect(), object);
        }
      }
      objects.clear();
    }
  }
  
   void query(FoundNodes& returnObjects, const SkRect& rect) {
    auto idx = getIndex(rect);
    if(!nodes.empty()) {
      for(int i : idx) {
        nodes[i]->query(returnObjects, rect);
      }
    }
     for(auto o : objects) {
       returnObjects.insert(o);
     }
  }
  
};

#endif /* QuadTree_hpp */
