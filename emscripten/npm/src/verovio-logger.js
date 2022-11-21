export const LOG_OFF = 0;
export const LOG_ERROR = 1;
export const LOG_WARNING = 2;
export const LOG_INFO = 3;
export const LOG_DEBUG = 4;

export function enableLog(level, VerovioModule) {
    return VerovioModule.cwrap("enableLog", null, ["number"])(level);
}

export function enableLogToBuffer(value, VerovioModule) {
    return VerovioModule.cwrap("enableLogToBuffer", null, ["number"])(value);
}
