import { app, BrowserWindow, ipcMain } from "electron";
import * as path from "path";

// ---- native addon bindings ----
type OglAddon = {
    attachGLView(
        handle: Buffer,
        x: number,
        y: number,
        w: number,
        h: number
    ): void;
    resizeGLView(x: number, y: number, w: number, h: number): void;
    setIOSurfaceId(id: number): void;
    destroyGLView(): void;
};
// eslint-disable-next-line @typescript-eslint/no-var-requires
const bindings = require("bindings") as (name: string) => any;
const ogl: OglAddon = bindings("oglview");

let win: BrowserWindow | null = null;
let oglAttached = false;

function createWindow() {
    win = new BrowserWindow({
        backgroundColor: "#0b0c10",
        webPreferences: {
            contextIsolation: true,
            nodeIntegration: false,
            preload: path.join(__dirname, "preload.js"),
        },
    });
    win.maximize();
    win.show();
    const devUrl = process.env.VITE_DEV_SERVER_URL || "http://127.0.0.1:5173";
    if (process.env.VITE_DEV_SERVER_URL) {
        win.loadURL(devUrl);
    } else {
        // built index.html from vite build
        win.loadFile(path.join(__dirname, "../renderer/index.html"));
    }

    // (Optional) one-time hard attach; remove after you see React-driven sizing working
    win.webContents.on("did-finish-load", () => {
        const handle = win!.getNativeWindowHandle();
        ogl.attachGLView(handle, 100, 80, 400, 260);
        oglAttached = true;
    });
}

// HTML/React-driven bounds -> native view
ipcMain.handle(
    "ogl:set-bounds",
    (_evt, b: { x: number; y: number; width: number; height: number }) => {
        if (!win) return false;
        const x = Math.max(0, Math.round(b.x));
        const y = Math.max(0, Math.round(b.y));
        const w = Math.max(1, Math.round(b.width));
        const h = Math.max(1, Math.round(b.height));
        if (!oglAttached) {
            const handle = win.getNativeWindowHandle();
            ogl.attachGLView(handle, x, y, w, h);
            oglAttached = true;
        } else {
            ogl.resizeGLView(x, y, w, h);
        }
        return true;
    }
);

ipcMain.handle("ogl:set-surface-id", (_evt, id: number) => {
    ogl.setIOSurfaceId(Number(id));
    return true;
});

ipcMain.handle("ogl:destroy-view", () => {
    ogl.destroyGLView();
    oglAttached = false;
    return true;
});

app.whenReady().then(createWindow);
app.on("window-all-closed", () => {
    if (process.platform !== "darwin") app.quit();
});
app.on("activate", () => {
    if (BrowserWindow.getAllWindows().length === 0) createWindow();
});
