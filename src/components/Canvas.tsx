import React from 'react';
import { requireNativeComponent, ViewProps, ViewStyle, View } from 'react-native';
import uuid from 'react-native-uuid';
import CanvasApi from './CanvasApi';

export interface CanvasViewProps extends ViewProps {
  style: ViewStyle;
  onReady: () => void;
  onResized: () => void;
  onBeganSelections?: () => void;
  onLongPressBegan?: (params: any) => void;
  onLongPressEnded?: () => void;
  lasso: boolean;
};

export const CanvasView = requireNativeComponent<CanvasViewProps>(
  'CanvasView'
);

export interface CanvasProps extends ViewProps {
  onCanvas?: (canvas: CanvasApi) => void;
  onResized?: () => void;
  onBeganSelections?: () => void;
  onLongPressBegan?: (params: any) => void;
  onLongPressEnded?: () => void;
}

export interface CanvasState {
  lasso: boolean;
}


export default class Canvas extends React.Component<CanvasProps, CanvasState> {
  generatedId: string ;
  vid: string;
  canvas: CanvasApi;
  onCanvasCallback?: (canvas: CanvasApi) => void;
  onResized?: () => void;
  onBeganSelections?: () => void;
  onLongPressBegan?: (params: any) => void;
  onLongPressEnded?: () => void;

  constructor(props: CanvasProps) {
    super(props);
    this.generatedId = uuid.v4().toString();
    this.vid = uuid.v4().toString();
    this.canvas = new CanvasApi(this.generatedId, this.vid);
    this.onCanvasCallback = props.onCanvas;
    this.onResized = props.onResized;
    this.onBeganSelections = props.onBeganSelections;
    this.onLongPressBegan = props.onLongPressBegan;
    this.onLongPressEnded = props.onLongPressEnded;
  }

  render() {
    return (<View style={{flex: 1, overflow: 'hidden'}}>
        <CanvasView nativeID={this.generatedId} style={{flex:1}}
        lasso={this.props.lasso}
        onReady={() => {
            this.onCanvasCallback?.(this.canvas);
        }}
        onResized={() => {
          this.onResized?.();
        }}
        onLongPressBegan={(params: any) => this.onLongPressBegan?.(params)}
        onLongPressEnded={() => this.onLongPressEnded?.()}
        onBeganSelections={() => {
          this.onBeganSelections?.();
        }}/>
      </View>);
  }
}