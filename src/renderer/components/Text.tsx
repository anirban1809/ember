import React from "react";

const Text = () => {
    const logMessage = async () => {
        console.log("Logged Message");
        console.log(
            "return value: ",
            await window.ogl.logMessage("This is another logged message")
        );
    };

    return <div className=" italic text-5xl">Log</div>;
};

export default Text;
