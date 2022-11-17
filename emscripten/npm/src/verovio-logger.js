import { createEmscriptenProxy } from "./emscripten-proxy.js";

export const logLevel = {
    off: 0,
    debug: 1,
    info: 2,
    warning: 3,
    error: 4,
};

export function enableLog(level, VerovioModule) {
    const proxy = createEmscriptenProxy(VerovioModule);
    const ptr = proxy.constructor();
    proxy.enableLog(ptr, level);
}
