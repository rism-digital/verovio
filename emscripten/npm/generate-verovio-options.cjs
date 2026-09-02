/**
 * Generate the VerovioOptions and EngravingDefaults type definitions.
 *
 * Usage: node generate-verovio-options.cjs [path/to/output/VerovioOptions.d.ts]
 */
const createVerovioModule = require('./dist/verovio-module.mjs').default;
const { VerovioToolkit } = require('./dist/verovio.cjs');
const fs = require('fs');

const outFile = process.argv.length > 2 ? process.argv[2] : 'src/types/VerovioOptions.d.ts';

const typeMap = {
    bool: "boolean",
    "std::string": "string",
    int: "number",
    double: "number",
    "std::string-list": "string[]",
    array: "string[]",
};

async function main() {
    const VerovioModule = await createVerovioModule();
    const tk = new VerovioToolkit(VerovioModule);
    const availableOptions = tk.getAvailableOptions();
    console.info(tk.getVersion());
    const types = new Set();
    try {
        const optionSegments = Object.entries(availableOptions.groups).map(
            ([key, group]) =>
                `    /**${"*".repeat(group.name.length)}**
     * ${group.name} *
     **${"*".repeat(group.name.length)}**/
` +
                Object.entries(group.options)
                    .map(
                        ([
                            key,
                            {
                                cmdOnly,
                                shortOption,
                                title,
                                type,
                                description,
                                values,
                                ...option
                            },
                        ]) =>
                            cmdOnly
                                ? ""
                                : `
    /**
     * ${["int", "double"].includes(type) ? `(${type}) ` : ""}${
                                      (types.add(type), description.trim())
                                  }${Object.entries(option)
                                      .map(
                                          ([key, value]) => `
     *
     * ${key}: ${JSON.stringify(value)}`
                                      )
                                      .join("")}
     */
    ${key}?: ${
                                      values
                                          ? values
                                                .map((v) => `"${v}"`)
                                                .join(" | ")
                                          : typeMap[type] ||
                                            key[0].toUpperCase() + key.slice(1)
                                  };
`
                    )
                    .join("")
        );
        fs.writeFileSync(
            outFile,
            `export interface TransposeMdiv {
    [xmlId: string]: string;
}
export interface EngravingDefaults {
    arrowShaftThickness: number;
    barlineSeparation: number;
    beamSpacing: number;
    beamThickness: number;
    bracketThickness: number;
    dashedBarlineDashLength: number;
    dashedBarlineGapLength: number;
    dashedBarlineThickness: number;
    hairpinThickness: number;
    legerLineExtension: number;
    legerLineThickness: number;
    lyricLineThickness: number;
    octaveLineThickness: number;
    pedalLineThickness: number;
    repeatBarlineDotSeparation: number;
    repeatEndingLineThickness: number;
    slurEndpointThickness: number;
    slurMidpointThickness: number;
    staffLineThickness: number;
    stemThickness: number;
    subBracketThickness: number;
    textEnclosureThickness: number;
    thickBarlineThickness: number;
    thinBarlineThickness: number;
    tieEndpointThickness: number;
    tieMidpointThickness: number;
    tupletBracketThickness: number;
}

export interface VerovioOptions {
${optionSegments.join("\n").trimEnd()}
}
`
        );
    } catch (e) {
        console.error(e);
    }
}
main();
