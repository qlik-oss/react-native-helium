/**
 * @format
 */

const CanvasPollyFill = {
  getContext: _c => {
    return {
      measureText: text => {
        const fontFamily = 'Open Sans';
        const fontSize = 12;

        const v = HeliumCanvasApi.measureText({fontFamily, fontSize, text});
        return v;
      },
    };
  },
};

export const enableCarbon = () => {
  if (!global.document) {
    global.document = {
      createElement: _e => {
        return CanvasPollyFill;
      },
    };
  }
};

enableCarbon();

import {AppRegistry} from 'react-native';
import App from './App';
import {name as appName} from './app.json';

AppRegistry.registerComponent(appName, () => App);
