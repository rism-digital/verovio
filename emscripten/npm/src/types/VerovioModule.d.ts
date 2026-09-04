export interface VerovioModule {
    onRuntimeInitialized: () => void;
    FS_unlink: (path: string) => void;
    FS_createDataFile: (
        parent: string,
        name: string,
        data: string,
        canRead: boolean,
        canWrite: boolean,
        canOwn: boolean,
    ) => void;
}
