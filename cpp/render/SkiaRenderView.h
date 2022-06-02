#pragma once
#include <include/core/SkCanvas.h>
#include <include/core/SkMatrix.h>
#include "Rect.h"
#include <map>
#include <set>
#include <vector>
#include <memory>
#include "QuadTree.h"
#include "Shape.h"
#include "SelectionsEngine.h"
#include "SelectionBrush.hpp"
#include "LassoLayer.hpp"

class SkiaRenderView {
public:
  SkiaRenderView(const SkRect& bounds);
  SkiaRenderView(const SkRect& bounds, const SkMatrix& matrix);
  void draw(SkCanvas* canvas);
  std::shared_ptr<SkiaRenderView> createVirtualRender(const std::string& vid, const SkRect& bounds,  const SkMatrix& matrix);
  void addShapes(const std::string& vid, std::vector<std::shared_ptr<Shape>> shapes);
  void clear();
  void resize(const std::string& vid, const SkRect& bounds);
  void beginSelections(const SkPoint& point);
  void go();
  void setSelectionBrush(std::shared_ptr<SelectionBrush> brush);
  void syncBrush();
  void startLasso(float x, float y);
  void endLasso(float x, float y);
  void updateLasso(float x, float y);

  void destroy(const std::string& vid) {
    auto it = virtualRenders.find(vid);
    if(it != virtualRenders.end()) {
      virtualRenders.erase(it);
    }
  }

  void resetBound(const SkRect& bounds) {
    clipBounds = bounds;
  }

  void removeAll() {
    virtualRenders.clear();
  }

  std::shared_ptr<SkiaRenderView> get(const std::string& vid) const {
    auto it = virtualRenders.find(vid);
    if( it != virtualRenders.end()) {
      return it->second;
    }
    return std::shared_ptr<SkiaRenderView>();
  }

  SkISize size() const {
    return SkISize::Make(clipBounds.width(), clipBounds.height());
  }

  bool inSelections() const {
    return selectionsEngine.isInSelections();
  }


protected:
  void add(std::shared_ptr<Shape>& shape) {
    shapes.push_back(shape);
  }

  SkRect getSelectionHitTestRect(float x, float y, const LassoLayer::Line& line);
  void updateLasso(float x, float y, LassoLayer::Line& line, std::unordered_set<std::shared_ptr<Shape>>& pending);
  void _endLasso(const SkRect& rect,  std::vector<SkPoint>&& verts);

protected:
  std::vector<std::shared_ptr<Shape>> shapes;
  std::map<std::string, std::shared_ptr<SkiaRenderView>> virtualRenders;
  SkMatrix worldTransform;
  SkRect clipBounds;
  SelectionsEngine selectionsEngine;
  Quadtree<std::shared_ptr<Shape>> quadTree;
  std::shared_ptr<SelectionBrush> selectionBrush;
  std::unique_ptr<LassoLayer> lassoLayer;
  SkPoint lastLassoPoint;
  std::unordered_set<std::shared_ptr<Shape>> pendingLassoSelected;
};
