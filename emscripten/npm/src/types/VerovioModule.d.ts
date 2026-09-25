/**
 * The emscripten module object passed to VerovioToolkit, and exported as `verovio.module`.
 *
 * The wasm exports are not meant to be called directly - use VerovioToolkit, which
 * wraps them with cwrap(). The `FS_*` helpers write to the module's own in-memory
 * filesystem, so a file created with them is visible to the toolkit.
 */
export interface VerovioModule {
    /**
     * Called when the runtime is fully initialized and the compiled code is safe to run.
     */
    onRuntimeInitialized: () => void;

    /**
     * View of the module memory as unsigned bytes. Replaced when the memory grows, so it must be re-read after any allocation.
     */
    HEAPU8: Uint8Array;

    /**
     * Wraps an exported function in a JavaScript function that takes and returns JavaScript values rather than pointers. Pass null as the return type for a void function.
     */
    cwrap: <T extends (...args: any[]) => any = (...args: any[]) => any>(
        ident: string,
        returnType?: "number" | "string" | "boolean" | "array" | null,
        argTypes?: ("number" | "string" | "array")[] | null,
        opts?: { async?: boolean },
    ) => T;

    /**
     * Creates a file in the in-memory filesystem with the given contents.
     */
    FS_createDataFile: (
        parent: string,
        name: string,
        data: string | ArrayLike<number>,
        canRead: boolean,
        canWrite: boolean,
        canOwn: boolean,
    ) => void;

    /**
     * Deletes a file or directory from the in-memory filesystem.
     */
    FS_unlink: (path: string) => void;

    /**
     * Releases memory previously allocated with _malloc.
     */
    _free: (ptr: number) => void;

    /**
     * Allocates the given number of bytes and returns a pointer to them.
     */
    _malloc: (size: number) => number;
}

/**
 * Attributes read from the module argument passed to createVerovioModule().
 *
 * This is the emscripten -s INCOMING_MODULE_JS_API list of emscripten/buildToolkit,
 * so attributes absent from it have no effect when passed.
 */
export interface VerovioModuleOptions {
    /**
     * Called when the runtime is fully initialized and the compiled code is safe to run.
     */
    onRuntimeInitialized?: () => void;
}

/** Signature of the module factory exported by verovio/wasm and verovio/wasm-hum. */
export type VerovioModuleFactory = (options?: VerovioModuleOptions) => Promise<VerovioModule>;
