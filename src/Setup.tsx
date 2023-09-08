import { NativeModules } from "react-native";

export const installHelium = () => {
  const { ReactNativeHelium } = NativeModules;
  // @ts-expect-error don't want to change previous code
  if (!global.HeliumCanvasApi) {
    ReactNativeHelium.installModule();
  }
};
