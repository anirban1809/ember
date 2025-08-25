import React, { useEffect, useRef } from "react";

// A React wrapper that positions/sizes the native NSView to match this <div>
type Props = {
    className?: string;
    style?: React.CSSProperties;
};

export default function OglMount({ className, style }: Props) {
    const ref = useRef<HTMLDivElement | null>(null);
    const last = useRef({ x: -1, y: -1, w: -1, h: -1 });
    const raf = useRef<number | null>(null);

    useEffect(() => {
        const el = ref.current!;
        if (!el) return;

        const send = () => {
            const r = el.getBoundingClientRect();
            const x = Math.round(r.left);
            const y = Math.round(r.top);
            const w = Math.max(1, Math.round(r.width));
            const h = Math.max(1, Math.round(r.height));
            const L = last.current;
            if (x !== L.x || y !== L.y || w !== L.w || h !== L.h) {
                last.current = { x, y, w, h };
                window.ogl
                    .setBounds({ x, y, width: w, height: h })
                    .catch(() => {});
            }
        };

        const schedule = () => {
            if (raf.current) cancelAnimationFrame(raf.current);
            raf.current = requestAnimationFrame(send);
        };

        // Observe element size changes
        const ro = new ResizeObserver(schedule);
        ro.observe(el);

        // Window resize/scroll affects viewport coords
        window.addEventListener("resize", schedule);
        window.addEventListener("scroll", schedule, { passive: true });

        // Initial double-RAF so styles/layout settle
        requestAnimationFrame(() => requestAnimationFrame(send));

        return () => {
            ro.disconnect();
            window.removeEventListener("resize", schedule);
            window.removeEventListener("scroll", schedule as any);
            if (raf.current) cancelAnimationFrame(raf.current);
        };
    }, []);

    // Important: no CSS transforms on this or parent nodes (native NSView won’t follow)
    return (
        <div
            ref={ref}
            className={className}
            style={{
                position: "relative",
                ...style,
            }}
        />
    );
}
