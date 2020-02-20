import Deferred from './Deferred';
import { createUuidProperty } from './utils';

// Internal HashMap to store all method calls on the verovio toolkit worker
const workerTasks = {};

const VerovioToolkitProxy = class {

    constructor(src) {
        this.worker = new Worker(src);
        // Listen to response of the service worker
        this.worker.addEventListener('message', function (event) {
            const { id, result } = event.data;
            // Check if there is a Deferred instance in workerTasks HashMap with
            // passed id of the Worker
            const deferred = workerTasks[id];
            if(deferred) {
                // If so resolve deferred promise and pass verovio toolkit return value
                deferred.resolve(result);
            }
        }, false);

        // Retrun a Proxy so it is possible to catch all property and method calls
        // of a VerovioToolkitProxy instance
        return new Proxy(this, {
            get: (target, method) => {
                return function () {
                    const args = Array.prototype.slice.call(arguments);
                    const id = createUuidProperty(workerTasks);

                    // Post a message to service worker with UUID, method name of the
                    // verovio toolkit function and passed arguments
                    target.worker.postMessage({
                        id,
                        method,
                        arguments: args,
                    });

                    // Create a new Deferred instance and store it in workerTasks HashMap
                    const deferred = new Deferred();
                    workerTasks[id] = deferred;

                    // Return the (currently still unresolved) Promise of the Deferred instance
                    return deferred.promise;
                };
            },
        });
    }

};

export default VerovioToolkitProxy;
