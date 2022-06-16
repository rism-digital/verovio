import DefaultVerovioModule from '../../build/verovio.js'
import { VerovioToolkit } from './verovio-toolkit.js';

class VerovioToolkitDefaultModule extends VerovioToolkit {
    constructor(VerovioModule = DefaultVerovioModule) {
        super(VerovioModule);
    }
};

export default {
    module: DefaultVerovioModule,
    toolkit: VerovioToolkitDefaultModule,
};
