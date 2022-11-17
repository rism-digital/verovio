import DefaultVerovioModule from "../../build/verovio.js";
import { VerovioToolkit } from "./verovio-toolkit.js";

class VerovioToolkitDefaultModule extends VerovioToolkit {
    constructor(VerovioModule = DefaultVerovioModule) {
        super(VerovioModule);
    }
}

// Assign Module to window to prevent breaking changes.
// Deprecated, use verovio.module instead.
if (typeof window !== "undefined") {
    window.Module = DefaultVerovioModule;
}

const logLevel = {
    off: 0, // or none
    debug: 1,
    info: 2,
    warning: 3,
    error: 4,
 };

export default {
    module: DefaultVerovioModule,
    logLevel: logLevel,
    enableLog: DefaultVerovioModule.cwrap( "enableLog", null, ["number"] ),
    enableLogToBuffer: DefaultVerovioModule.cwrap("enableLogToBuffer", null, ["number"]),
    toolkit: VerovioToolkitDefaultModule,
};
