import uuid from "react-native-uuid";

export type CanvasRect = {
  x: number;
  y: number;
  width: number;
  height: number;
  fillColor?: string;
  path?: string;
};

export type CanvasSize = {
  width: number;
  height: number;
};

export type CanvasContext = {
  id: string;
  vid: string;
};

export type HeliumCanvasApiType = {
  paint: (context: CanvasContext) => void;
  getSize: (context: CanvasContext) => CanvasSize;
  createVirtualCanvas: (context: CanvasContext, vid: string, rect: CanvasRect) => void;
  destroyVirtualCanvas: (context: CanvasContext) => void;
  clear: (context: CanvasContext) => void;
  drawShapes: (context: CanvasContext, shapes: Array<any>) => void;
  resize: (context: CanvasContext, rect: CanvasRect) => void;
  setSelectionBrushes: (context: CanvasContext, brushes: Array<any>) => void;
  confirmSelections: (context: CanvasContext) => void;
  clearSelections: (context: CanvasContext) => void;
  setLongPressHandler: (context: CanvasContext, fn: (data: Array<any>) => void) => void;
};

declare let HeliumCanvasApi: HeliumCanvasApiType;

export default class CanvasApi {
  context: CanvasContext;

  constructor(id: string, vid: string) {
    this.context = { id, vid };
  }

  resize(rect: CanvasRect) {
    HeliumCanvasApi.resize(this.context, rect);
  }

  addShapes(shapes: Array<any>) {
    HeliumCanvasApi.drawShapes(this.context, shapes);
  }

  draw() {
    HeliumCanvasApi.paint(this.context);
  }

  getSize() {
    return HeliumCanvasApi.getSize(this.context);
  }

  createVirtualCanvas(rect: CanvasRect) {
    const nvid = uuid.v4().toString();
    HeliumCanvasApi.createVirtualCanvas(this.context, nvid, rect);
    return new CanvasApi(this.context.id, nvid);
  }

  clear() {
    HeliumCanvasApi.clear(this.context);
  }

  destroy() {
    HeliumCanvasApi.destroyVirtualCanvas(this.context);
  }

  setSelectionBrushes(brushes: Array<any>) {
    HeliumCanvasApi.setSelectionBrushes(this.context, brushes[0]);
  }

  confirmSelections() {
    HeliumCanvasApi.confirmSelections(this.context);
  }

  clearSelections() {
    HeliumCanvasApi.clearSelections(this.context);
  }

  setLongPressHandler(fn: (data: Array<any>) => void) {
    HeliumCanvasApi.setLongPressHandler(this.context, fn);
  }
}
