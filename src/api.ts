import { ogl } from "./main";

export const logMessage = (
    _evt: Electron.IpcMainInvokeEvent,
    message: object
) => {
    return ogl.logMessage(message);
};
