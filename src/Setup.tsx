import { NativeModules, Platform } from "react-native";

export const installHelium = () => {
    const {ReactNativeHelium} = NativeModules;
    if (!global.HeliumCanvasApi) {
        ReactNativeHelium.installModule();
    }
}
