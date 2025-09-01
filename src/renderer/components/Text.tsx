import React from "react";

const Text = () => {
    const logMessage = () => {
        console.log("Logged Message");
        window.ogl.logMessage("This is another logged message");
    };

    return (
        <div>
            <button onClick={logMessage}>Log</button>
        </div>
    );
};

export default Text;
