import React, { useMemo, useState } from "react";
import OglMount from "./OglMount";

export default function App() {
    const [vw, setVw] = useState(70);
    const [vh, setVh] = useState(55);

    const boxStyle = useMemo<React.CSSProperties>(
        () => ({
            width: `min(${vw}vw, 1200px)`,
            height: `min(${vh}vh, 900px)`,
            borderRadius: 12,
            border: "2px dashed #39b36a",
            background: "#143524",
            boxShadow: "0 10px 30px rgba(0,0,0,.35) inset",
        }),
        [vw, vh]
    );

    return (
        <div
            style={{
                display: "grid",
                gridTemplateColumns: "280px 1fr",
                height: "100%",
            }}
        >
            <aside style={{ padding: 16, borderRight: "1px solid #223" }}>
                <h3>Scene Hierarchy</h3>
                <label>Width (% viewport):</label>
                <input
                    type="range"
                    min={20}
                    max={95}
                    value={vw}
                    onChange={(e) => setVw(Number(e.target.value))}
                    style={{ width: "100%" }}
                />
                <label>Height (% viewport):</label>
                <input
                    type="range"
                    min={20}
                    max={90}
                    value={vh}
                    onChange={(e) => setVh(Number(e.target.value))}
                    style={{ width: "100%" }}
                />

                <p style={{ opacity: 0.7 }}>
                    Move the sliders — the OpenGL view resizes live.
                </p>
            </aside>

            <main style={{ padding: 16 }}>
                {/* This element controls the native OpenGL view’s rect */}
                <OglMount style={boxStyle} />
            </main>
        </div>
    );
}
