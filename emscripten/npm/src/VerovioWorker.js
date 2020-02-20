import VerovioToolkit from './VerovioToolkit';
import Deferred from './Deferred';

// Check if we are in a web worker
if (typeof WorkerGlobalScope !== 'undefined' && self instanceof WorkerGlobalScope) {

    // Initializing an empty object to prevent if in onMessage listener the toolkit
    // is beeing accessed before Module.onRuntimeInitialized
    let verovioToolkit = {};

    // Global deferred Promise that can be resolved when Module is initialized
    const moduleIsReady = new Deferred();

    // Create a new toolkit instance when Module is ready
    Module.onRuntimeInitialized = function () {
        verovioToolkit = new VerovioToolkit();
        moduleIsReady.resolve();
    };

    // Listen to messages send to this worker
    self.addEventListener('message', function (event) {
        // Destruct properties passed to this message event
        const { id, method, arguments: args } = event.data;

        // postMessage on a `onRuntimeInitialized` method as soon as
        // Module is initialized
        if(method === 'onRuntimeInitialized') {
            moduleIsReady.promise.then(() => {
                self.postMessage({
                    id,
                    method,
                    arguments: args,
                    result: null,
                }, event);
            });
            return;
        }

        // Check if verovio toolkit has passed method
        const fn = verovioToolkit[method || null];
        let result;
        if(fn) {
            // Call verovio toolkit method and pass arguments
            result = fn.apply(verovioToolkit, arguments || []);
        }

        // Always respond to worker calls with postMessage
        self.postMessage({
            id,
            method,
            arguments: arguments,
            result,
        }, event);
    }, false);
}
