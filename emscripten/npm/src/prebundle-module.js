import DefaultVerovioModule from "../../build/verovio.js";
import { VerovioToolkit } from "./verovio-toolkit.js";
import {
    LOG_OFF,
    LOG_ERROR,
    LOG_WARNING,
    LOG_INFO,
    LOG_DEBUG,
    enableLog,
    enableLogToBuffer
} from "./verovio-logger.js";

class VerovioToolkitDefaultModule extends VerovioToolkit {
    constructor(VerovioModule = DefaultVerovioModule) {
        super(VerovioModule);
    }
}

function enableLogDefaultModule(level, VerovioModule = DefaultVerovioModule) {
    return enableLog(level, VerovioModule);
}

function enableLogToBufferDefaultModule(value, VerovioModule = DefaultVerovioModule) {
    return enableLogToBuffer(value, VerovioModule);
}

// Assign Module to window to prevent breaking changes.
// Deprecated, use verovio.module instead.
if (typeof window !== "undefined") {
    window.Module = DefaultVerovioModule;
}

export default {
    module: DefaultVerovioModule,
    toolkit: VerovioToolkitDefaultModule,
    LOG_OFF,
    LOG_ERROR,
    LOG_WARNING,
    LOG_INFO,
    LOG_DEBUG,
    enableLog: enableLogDefaultModule,
    enableLogToBuffer: enableLogToBufferDefaultModule,
};
