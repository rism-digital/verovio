export const logLevel = {
    off: 0,
    debug: 1,
    info: 2,
    warning: 3,
    error: 4,
};

export function enableLog(level = logLevel.debug, VerovioModule) {
    return VerovioModule.cwrap("enableLog", null, ["number"])(level);
}
