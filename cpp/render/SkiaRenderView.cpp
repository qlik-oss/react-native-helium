#include "SkiaRenderView.h"
#include <include/core/SkPaint.h>
#include "../jsi/ShapeFactory.h"
#include "Helium.h"
#include <algorithm>

SkiaRenderView::SkiaRenderView(const SkRect& bounds)
: quadTree(0, bounds) {
  clipBounds = bounds;
  lassoLayer = std::make_unique<LassoLayer>();
}

SkiaRenderView::SkiaRenderView(const SkRect& bounds, const SkMatrix& matrix)
: quadTree(0, bounds) {
  worldTransform =(matrix);
  clipBounds = (bounds);
}

void SkiaRenderView::addShapes(const std::string& vid, std::vector<std::shared_ptr<Shape>> shapes) {
  auto it = virtualRenders.find(vid);
  if(it != virtualRenders.end()) {
    it->second->selectionsEngine.resetData(shapes);
    it->second->shapes = std::move(shapes);
    it->second->go();
  }
}

void SkiaRenderView::go() {
  quadTree.clear();
  for(auto&& shape: shapes) {
    auto aab = shape->getAAB();
    if(aab) {
      quadTree.insert(aab->getRect(), shape);
    }
  }
}

void SkiaRenderView::clear() {
  shapes.clear();
  quadTree.clear();
  selectionsEngine.clearSelections();
  for(auto&& v: virtualRenders) {
    v.second->clear();
  }
}

void SkiaRenderView::draw(SkCanvas* canvas) {
  canvas->save();
  canvas->setMatrix(worldTransform);

  canvas->clipRect(clipBounds);
  for(auto&& shape: shapes) {
    shape->draw(canvas);
  }

  canvas->restore();

  for(auto&& it : virtualRenders) {
    it.second->draw(canvas);
  }

  if( lassoLayer) {
    lassoLayer->render(canvas);
  }
}

std::shared_ptr<SkiaRenderView>
SkiaRenderView::createVirtualRender(const std::string &vid, const SkRect& bounds,  const SkMatrix &matrix) {
  auto view = std::make_shared<SkiaRenderView>(bounds, matrix);
  virtualRenders[vid] = view;
  return view;
}

void SkiaRenderView::resize(const std::string& vid, const SkRect& bounds) {
  auto it = virtualRenders.find(vid);
  if( it != virtualRenders.end()) {
    it->second->resetBound(bounds);
  }
}

void SkiaRenderView::beginSelections(const SkPoint& point) {
  selectionsEngine.beginSelections();
  auto x = point.x();
  auto y = point.y();
  auto pp = SkRect::MakeXYWH(x, y, Helium::toPx(4), Helium::toPx(4));
  pp.offset(-pp.width() / 2, -pp.height()/2);
  Quadtree<std::shared_ptr<Shape>>::FoundNodes found;
  quadTree.query(found, pp);
  for(auto f : found) {
    if(f->getAAB()->hitTest(pp)){
      if(f->toggle()) {
        selectionsEngine.add(f);
      } else {
        selectionsEngine.remove(f);
      }
    }
  }
  // Have you checked the children?
  for(auto&& child: virtualRenders) {
    child.second->beginSelections(point);
  }
  selectionsEngine.syncSelections();
}

void SkiaRenderView::setSelectionBrush(std::shared_ptr<SelectionBrush> brush) {
  selectionBrush = brush;
  selectionsEngine.setSelectionBrush(brush);
  for(auto&& child: virtualRenders) {
    child.second->setSelectionBrush(brush);
  }
}

void SkiaRenderView::syncBrush() {
  selectionsEngine.sync();
}

void SkiaRenderView::startLasso(float x, float y) {
  if(lassoLayer) {
    pendingLassoSelected.clear();
    lassoLayer->start(x, y);
  }
}

void SkiaRenderView::endLasso(float x, float y) {
  if(lassoLayer) {
    lassoLayer->end(x, y);
    auto path = lassoLayer->getPath();
    auto rect = path.computeTightBounds();
    auto points = path.countPoints();
    std::vector<SkPoint> verts(points);
    path.getPoints(&verts[0], static_cast<int>(verts.size()));
    _endLasso(rect, std::move(verts));

    if(selectionBrush) {
      Helium::TheCanvasViewManager::instance()->runOnJS([this]{
        selectionBrush->sync();
      });
    }
  }
}

void SkiaRenderView::_endLasso(const SkRect &rect,  std::vector<SkPoint>&& verts) {
  Quadtree<std::shared_ptr<Shape>>::FoundNodes found;
  quadTree.query(found, rect);
  for(auto f : found) {
    if(f->getAAB()->hitTest(verts)){
      f->active();
      if(selectionBrush) {
        selectionBrush->add(f);
      }
    }
  }

  // Have you checked the children?
  for(auto&& child: virtualRenders) {
    child.second->_endLasso(rect, std::move(verts));
  }
}

// this is the root call
void SkiaRenderView::updateLasso(float x, float y) {
  LassoLayer::Line line;
  if(lassoLayer) {
    line = lassoLayer->update(x, y);
    updateLasso(x, y, line, pendingLassoSelected);
  }
}

void SkiaRenderView::updateLasso(float x, float y, LassoLayer::Line &line, std::unordered_set<std::shared_ptr<Shape>>& pending) {
  auto hitTestBox = getSelectionHitTestRect(x, y, line);
  Quadtree<std::shared_ptr<Shape>>::FoundNodes found;
  quadTree.query(found, hitTestBox);
  for(auto f : found) {
    if(f->getAAB()->hitTest(line.p1, line.p2)){
      selectionsEngine.activate(f, pending);
    }
  }

  // Have you checked the children?
  for(auto&& child: virtualRenders) {
    child.second->updateLasso(x, y, line, pending);
  }
}

inline SkRect SkiaRenderView::getSelectionHitTestRect(float x, float y, const LassoLayer::Line& line) {
  auto size = std::max(line.length() / 2.0f, 2.0f);
  auto pp = SkRect::MakeXYWH(x, y, Helium::toPx(size), Helium::toPx(size));
  pp.offset(-pp.width() / 2, -pp.height()/2);
  return pp;
}
