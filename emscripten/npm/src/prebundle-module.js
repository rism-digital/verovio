import DefaultVerovioModule from "../../build/verovio.js";
import { VerovioToolkit } from "./verovio-toolkit.js";
import { logLevel, enableLog } from "./verovio-logger.js";

class VerovioToolkitDefaultModule extends VerovioToolkit {
    constructor(VerovioModule = DefaultVerovioModule) {
        super(VerovioModule);
    }
}

function enableLogDefaultModule(level = logLevel.debug, VerovioModule = DefaultVerovioModule) {
    enableLog(level, VerovioModule);
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
    logLevel,
    enableLog: enableLogDefaultModule,
};
