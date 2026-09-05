export interface CommitAction {
    action: "commit";
}
export interface ChainAction {
    action: "chain";
    param: EditorAction[];
}
export interface ContextAction {
    action: "context";
    param: {
        elementId: string;
        document?: "scores";
    };
}
export interface DeleteAction {
    action: "delete";
    param: {
        elementId: string;
        backspace?: boolean;
    };
}
export interface DragAction {
    action: "drag";
    param: {
        elementId: string;
        x: number;
        y: number;
    };
}
export interface InsertAction {
    action: "insert";
    param: {
        elementName: string;
        elementId: string;
        insertMode: "appendChildNoDuplicate" | "insertBefore" | "insertAfter"
    };
}
export interface InsertControlAction {
    action: "insertControl";
    param: {
        elementName: string;
        startId: string;
        endId: string;
    }
}
export interface InsertCursorByDurAction {
    action: "insertCursorByDur";
    param: {
        dur: string;
        dots?: number;
    }
}
export interface InsertCursorByPitchAction {
    action: "insertCursorByPitch";
    param: (
        | { pname: string, midi?: never }
        | { midi: number, pname?: never }
    ) & {
        oct?: number;
        accid?: string;
    }
}
export interface InsertCursorByTypeAction {
    action: "insertCursorByType";
    param: {
        type: "rest" | "tie" | "copy";
    }
}
export interface InsertCursorContainerAction {
    action: "insertCursorContainer";
    param: {
        container: "tuplet" | "graceGrp";
    }
}
export interface InsertMeasureAction {
    action: "insertMeasure";
    param: {
        elementId?: string;
        number: number;
        insertBefore?: boolean;
    }
}
export interface InsertNoteAction {
    action: "insertNote";
    param: {
        elementId: string;
        pname: string;
        oct?: number;
        accid?: string;
        accidGes?: string;
        dots?: number;
    } & (
        | { dur: string; chordMode?: never; }
        | { chordMode: boolean; dur?: never; }
    )
}
export interface InsertRestAction {
    action: "insertRest";
    param: {
        elementId: string;
        dur: string;
        dots?: number;
    }
}
export interface KeyDownAction {
    action: "keyDown";
    param: {
        elementId: string;
        key: number;
        shiftKey?: boolean;
        ctrlKey?: boolean;
    };
}
export interface NavigateAction {
    action: "navigate";
    param: {
        elementId: string;
        direction: number;
    };
}
export interface PropertiesAction {
    action: "properties";
    param: {
        scoreDef?: string;
    };
}
export interface RedoAction {
    action: "redo";
}
export interface ResetCursorAction {
    action: "resetCursor";
    param: {
        maintainChordMode?: boolean;
    }
}
export interface ResetCursorContainerAction {
    action: "resetCursorContainer";
}
export interface SelectAction {
    action: "select";
    param: {
        elementId: string;
        secondary?: boolean;
        custom?: "note" | "textParent";
    }
}
export interface SetAction {
    action: "set";
    param: {
        elementId: string;
        attribute: string;
        value: string;
    };
}
export interface SetCursorAction {
    action: "setCursor";
    param: {
        elementId?: string;
        inputMode: "pitchFirst" | "durationFirst";
        chordMode: boolean;
    }
}
export interface UndoAction {
    action: "undo";
}
export interface UpdateCursorAction {
    action: "updateCursor";
    param: (
        | { chordMode: boolean; restMode?: never; tieMode?: never }
        | { restMode: boolean; chordMode?: never; tieMode?: never }
        | { tieMode: "tie" | "copy"; chordMode?: never; restMode?: never }
    );
}
export interface UpdatePitchAction {
    action: "updatePitch";
    param: {
        elementId: string;
        pname?: string;
        oct?: number;
        accid?: string;
        midi?: number;
    }
}
export type EditorAction =
    | CommitAction
    | ChainAction
    | ContextAction
    | DeleteAction
    | DragAction
    | InsertAction
    | InsertControlAction
    | InsertCursorByDurAction
    | InsertCursorByPitchAction
    | InsertCursorByTypeAction
    | InsertCursorContainerAction
    | InsertMeasureAction
    | InsertNoteAction
    | InsertRestAction
    | KeyDownAction
    | NavigateAction
    | PropertiesAction
    | RedoAction
    | ResetCursorAction
    | ResetCursorContainerAction
    | SelectAction
    | SetAction
    | SetCursorAction
    | UndoAction
    | UpdateCursorAction
    | UpdatePitchAction;

export type EditorResponse = object;

export type EditorStatus = object;
