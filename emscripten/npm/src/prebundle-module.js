import DefaultVerovioModule from "../../build/verovio.js";
import { VerovioToolkit } from "./verovio-toolkit.js";
import { logLevel, enableLog, enableLogToBuffer } from "./verovio-logger.js";

class VerovioToolkitDefaultModule extends VerovioToolkit {
    constructor(VerovioModule = DefaultVerovioModule) {
        super(VerovioModule);
    }
}

function enableLogDefaultModule(level = logLevel.debug, VerovioModule = DefaultVerovioModule) {
    return enableLog(level, VerovioModule);
}

function enableLogToBufferDefaultModule(level = logLevel.debug, VerovioModule = DefaultVerovioModule) {
    return enableLogToBuffer(level, VerovioModule);
}

// Assign Module to window to prevent breaking changes.
// Deprecated, use verovio.module instead.
if (typeof window !== "undefined") {
    window.Module = DefaultVerovioModule;
}

export default {
    module: DefaultVerovioModule,
    toolkit: VerovioToolkitDefaultModule,
    logLevel,
    enableLog: enableLogDefaultModule,
    enableLogToBuffer: enableLogToBufferDefaultModule,
};
