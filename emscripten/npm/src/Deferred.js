// Wrapper class to allow to resolve a Promise outside of its scope
export default class {
    constructor() {
        this.promise = new Promise((resolve, reject) => {
            this.reject = reject;
            this.resolve = resolve;
        });
    }
};
