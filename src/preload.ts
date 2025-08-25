import { contextBridge, ipcRenderer } from "electron";

contextBridge.exposeInMainWorld("ogl", {
    setBounds: (b: { x: number; y: number; width: number; height: number }) =>
        ipcRenderer.invoke("ogl:set-bounds", b),
    setSurfaceId: (id: number) =>
        ipcRenderer.invoke("ogl:set-surface-id", Number(id)),
    destroyView: () => ipcRenderer.invoke("ogl:destroy-view"),
});
