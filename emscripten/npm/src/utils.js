// Helper function to create a UUID
export const uuidv4 = function () {
    return 'xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx'.replace(/[xy]/g, function (c) {
        var r = Math.random() * 16 | 0, v = c == 'x' ? r : (r & 0x3 | 0x8);
        return v.toString(16);
    });
};

// Helper function to identify method calls in worker onMessage listener
export const createUuidProperty = function (context) {
    let id;
    do {
        id = uuidv4();
    } while (context[id]);
    return id;
};
