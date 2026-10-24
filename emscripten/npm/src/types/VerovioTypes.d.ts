export interface MIDIValues {
    time: number;
    pitch: number;
    duration: number;
}
export interface GetMeiOptions {
    /**
     * Page number, 1-based. All pages if none (or 0) specified.
     */
    pageNo?: number;
    /**
     * Score Based, default true
     */
    scoreBased?: boolean;
    /**
     * Basic, default false
     */
    basic?: boolean;
    /**
     * remove all @xml:id not used in the data; default false
     */
    removeIds?: boolean;
}
export type Fraction = [number, number];
export type TimeMapEntry = {
    tstamp: number;
    on?: string[];
    off?: string[];
    tempo?: number;
    restsOn?: string[];
    restsOff?: string[];
    measureOn?: string;
} & (
    | { qstamp: number; qfrac?: never }
    | { qstamp?: never; qfrac: Fraction }
)
export interface RedoLayoutOptions {
    /**
     * true by default
     */
    resetCache?: boolean;
}
export interface TimeMapOptions {
    /**
     * Include measures in the timemap (false by default)
     */
    includeMeasures?: boolean;
    /**
     * Include rests in the timemap (false by default)
     */
    includeRests?: boolean;
    /**
     * Use fractions instead of floats for onset times (false by default)
     */
    useFractions?: boolean;
}
export interface Selection {
    measureRange?: string;
    start?: string;
    end?: string;
}
export interface PAEValidationMessage {
    column: number;
    row: number;
    code: number;
    text: string;
    type: "error" | "warning";
}
export interface PAEValidation {
    keysig?: PAEValidationMessage;
    clef?: PAEValidationMessage;
    timesig?: PAEValidationMessage;
    data?: PAEValidationMessage[];
}
export interface ExpansionMap {
    [key: string]: any;
}
export interface DescriptiveFeatures {
    intervalsChromatic: string[];
    intervalsDiatonic: string[];
    intervalGrossContour: string[];
    intervalRefinedContour: string[];
    intervalsIds: string[];
    pitchesChromaticWithDuration: string[];
    pitchesChromatic: string[];
    pitchesDiatonic: string[];
    pitchesIds: string[];
}
export type DescriptiveFeaturesOptions = unknown;
export interface ElementTimes {
    qfracOn: Fraction;
    qfracOff: Fraction;
    qfracDuration: Fraction;
    qfracTiedDuration: Fraction;
    tstampOn: number;
    tstampOff: number;
}
