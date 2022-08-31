import DefaultVerovioModule from "../../build/verovio.js"
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

export default {
    module: DefaultVerovioModule,
    toolkit: VerovioToolkitDefaultModule,
};
