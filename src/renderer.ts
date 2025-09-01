export {};
declare global {
    interface Window {
        ogl: {
            setBounds(b: {
                x: number;
                y: number;
                width: number;
                height: number;
            }): Promise<boolean>;
            setSurfaceId?(id: number): Promise<boolean>;
            destroyView?(): Promise<void>;
            logMessage(message: string): void;
        };
    }
}
function mountNativeViewToElement(el: HTMLElement) {
    let last = { x: -1, y: -1, w: -1, h: -1 };
    let raf = 0;

    const updateNow = () => {
        const r = el.getBoundingClientRect();
        const x = Math.round(r.left);
        const y = Math.round(r.top);
        const w = Math.max(0, Math.round(r.width));
        const h = Math.max(0, Math.round(r.height));

        if (x !== last.x || y !== last.y || w !== last.w || h !== last.h) {
            last = { x, y, w, h };
            console.log("[Renderer] setBounds ->", last);
            window.ogl
                .setBounds({ x, y, width: w, height: h })
                .then((ok) => console.log("[Renderer] setBounds ok?", ok))
                .catch((err) =>
                    console.error("[Renderer] setBounds failed:", err)
                );
        }
    };

    const schedule = () => {
        cancelAnimationFrame(raf);
        raf = requestAnimationFrame(updateNow);
    };

    const ro = new ResizeObserver(schedule);
    ro.observe(el);

    window.addEventListener("resize", schedule);
    window.addEventListener("scroll", schedule, { passive: true });

    // initial attach
    updateNow();

    return () => {
        ro.disconnect();
        cancelAnimationFrame(raf);
    };
}

window.addEventListener("DOMContentLoaded", () => {
    const target = document.getElementById("oglMount");
    if (!target) return console.warn("[Renderer] #oglMount not found");
    mountNativeViewToElement(target);
});
