#!/bin/sh
set -eu

# Run from the repository root. The optional second argument is the local MusicXML 4.0 XSD.
# MusicXML, MEI, and SVG outputs are kept in a fresh directory under build/.
verovio=${1:-build/verovio}
schema=${2:-}
fixtures=doc/tests/musicxml
mkdir -p build
output=$(mktemp -d build/musicxml-test.XXXXXXXX)
printf 'MusicXML test outputs: %s\n' "$output"

renderSvg() {
    "$verovio" -r data -f "$1" -t svg --breaks none --adjust-page-height -o "$output/$3.svg" "$2"
}

for fixture in staff-instrument group-instrument layer-instruments; do
    first="$output/$fixture.musicxml"
    second="$output/$fixture-roundtrip.musicxml"
    "$verovio" -r data -f mei -t musicxml -o "$first" "$fixtures/$fixture.mei"
    "$verovio" -r data -f musicxml -t musicxml -o "$second" "$first"
    if [ -n "$schema" ]; then
        xmllint --nonet --noout --schema "$schema" "$first" "$second"
    fi
    diff -u "$first" "$second"
    "$verovio" -r data -f musicxml -t mei -o "$output/$fixture.mei" "$first"
    renderSvg mei "$fixtures/$fixture.mei" "$fixture-original"
    renderSvg mei "$output/$fixture.mei" "$fixture-imported"
done

check() {
    result=$(xmllint --xpath "boolean($2)" "$output/$1")
    if [ "$result" != true ]; then
        printf 'Assertion failed in %s: %s\n' "$1" "$2" >&2
        exit 1
    fi
}

check staff-instrument.musicxml '/score-partwise/part-list/score-part[@id="violin"]'
check staff-instrument.musicxml '//measure[@number="1"]/attributes[divisions=2 and key/fifths=1]'
check staff-instrument.musicxml '//measure[@number="1"][not(@implicit) and backup/duration=8 and forward/duration=2]'
check staff-instrument.musicxml '//note[chord and duration=3 and type="quarter" and dot]'
check staff-instrument.musicxml '//note[pitch/alter=1 and not(accidental)]'
check staff-instrument.musicxml '//note[accidental="natural" and duration=1]'
check staff-instrument.musicxml '//note[@print-object="no" and rest and voice=2 and duration=4]'
check staff-instrument.musicxml '//measure[@number="2"]/note[rest/@measure="yes" and duration=8]'
check staff-instrument.mei '//*[local-name()="staffDef"]/*[local-name()="instrDef" and @xml:id="violin"]'

check group-instrument.musicxml 'count(/score-partwise/part)=2 and /score-partwise/part[@id="P1"]'
check group-instrument.musicxml '//part[@id="piano"]/measure/attributes[staves=2 and clef[@number="2"]/sign="F"]'
check group-instrument.musicxml '//part[@id="piano"]/measure/note[staff=2 and voice=2 and duration=3]'
check group-instrument.mei '//*[local-name()="staffGrp"]/*[local-name()="instrDef" and @xml:id="piano"]'

check layer-instruments.musicxml 'count(/score-partwise/part-list/score-part)=1 and count(/score-partwise/part)=1'
check layer-instruments.musicxml '//score-part[score-instrument[@id="oboe" and instrument-name="Oboe"] and score-instrument[@id="clarinet" and instrument-name="Clarinet"]]'
check layer-instruments.musicxml '//part/measure[@number="0" and @implicit="yes"][not(attributes/staves) and backup/duration=1]'
check layer-instruments.musicxml '//part/measure/note[voice=1 and instrument/@id="oboe" and pitch/step="G"]'
check layer-instruments.musicxml '//part/measure/note[voice=2 and instrument/@id="clarinet" and pitch/step="E"]'
check layer-instruments.mei 'count(//*[local-name()="staffDef"])=1 and count(//*[local-name()="measure"]/*[local-name()="staff"])=1'
check layer-instruments.mei '//*[local-name()="staff"]/*[local-name()="layer" and @n="1"]/*[local-name()="note" and @pname="g"]'
check layer-instruments.mei '//*[local-name()="staff"]/*[local-name()="layer" and @n="2"]/*[local-name()="note" and @pname="e"]'
check layer-instruments.mei '//*[local-name()="staffDef"]/*[local-name()="layerDef" and @n="1"]/*[local-name()="instrDef" and @xml:id="oboe" and @label="Oboe"]'
check layer-instruments.mei '//*[local-name()="staffDef"]/*[local-name()="layerDef" and @n="2"]/*[local-name()="instrDef" and @xml:id="clarinet" and @label="Clarinet"]'
check layer-instruments-original.svg 'count(//*[@class="staff"])=1'
check layer-instruments-imported.svg 'count(//*[@class="staff"])=1'

input="$fixtures/imported-parts.musicxml"
"$verovio" -r data -f musicxml -t mei -o "$output/imported-parts.mei" "$input"
"$verovio" -r data -f musicxml -t musicxml -o "$output/imported-parts.musicxml" "$input"
"$verovio" -r data -f musicxml -t musicxml -o - "$input" | diff -u "$output/imported-parts.musicxml" -
if [ -n "$schema" ]; then
    xmllint --nonet --noout --schema "$schema" "$input" "$output/imported-parts.musicxml"
fi
check imported-parts.mei '//*[local-name()="staffDef"]/*[local-name()="instrDef" and @xml:id="external-violin"]'
check imported-parts.mei '//*[local-name()="staffGrp"]/*[local-name()="instrDef" and @xml:id="external-piano" and @midi.channel=1 and @midi.instrnum=0]'
check imported-parts.mei 'count(//*[@xml:id="external-violin" or @xml:id="external-piano"])=2'
check imported-parts.musicxml '/score-partwise/part-list[score-part[@id="external-violin"] and score-part[@id="external-piano"]]'
renderSvg musicxml "$input" imported-parts-original
renderSvg mei "$output/imported-parts.mei" imported-parts-imported
