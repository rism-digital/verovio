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

for fixture in staff-instrument group-instrument layer-instruments beam beam-two-voices tuplet tie tie-chords tie-chord-notes \
    slur-chord slurs-overlap dynam-startid dynam-tstamp hairpin-cross-measure hairpins-overlap fermata ornaments \
    measure-barlines measure-repeats directives directives-fractional tempo; do
    first="$output/$fixture.musicxml"
    second="$output/$fixture-roundtrip.musicxml"
    "$verovio" -r data -f mei -t musicxml -o "$first" "$fixtures/$fixture.mei"
    "$verovio" -r data -f musicxml -t musicxml -o "$second" "$first"
    if [ -n "$schema" ]; then
        xmllint --nonet --noout --schema "$schema" "$first" "$second"
    fi
    "$verovio" -r data -f musicxml -t mei -o "$output/$fixture.mei" "$first"
    "$verovio" -r data -f musicxml -t mei -o "$output/$fixture-roundtrip.mei" "$second"
    diff -u "$first" "$second"
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

check beam.musicxml '//measure[attributes/divisions=2 and not(backup) and count(note[voice=1 and type="eighth" and duration=1])=4]'
check beam.musicxml '//measure[note[1]/beam[@number="1" and .="begin"] and note[2]/beam[@number="1" and .="continue"] and note[3]/beam[@number="1" and .="continue"] and note[4]/beam[@number="1" and .="end"]]'
check beam.mei '//*[local-name()="beam"][count(*[local-name()="note" and @dur="8"])=4]'
check beam-two-voices.musicxml '//measure[backup/duration=4 and count(note[voice=1]/beam[@number="1"])=4 and count(note[chord and voice=1])=2 and not(note[chord or voice=2]/beam)]'
check beam-two-voices.musicxml '//note[voice=1 and rest]/beam[@number="1" and .="continue"]'
check beam-two-voices.mei '//*[local-name()="layer" and @n="1"]/*[local-name()="beam"][count(*[local-name()="chord"])=2 and *[local-name()="rest"]]'
check beam-two-voices.mei '//*[local-name()="layer" and @n="2"]/*[local-name()="note" and @dur="2"]'

check tuplet.musicxml '//measure[@number="1" and not(@implicit) and attributes/divisions=30]
    /note[1][duration=15 and type="eighth" and dot and count(notations)=1
    and notations[slur/@type="start" and tuplet[@type="start" and @bracket="yes" and @show-number="both" and @placement="above"]]]'
check tuplet.musicxml '//measure[@number="1"][count(note[time-modification[actual-notes=3 and normal-notes=2]])=4
    and note[2][rest and duration=5 and type="16th"] and note[3][duration=10 and notations/tuplet/@type="stop"]
    and note[4][chord and duration=10 and not(notations/tuplet)]
    and note[5][duration=90 and not(time-modification)] and direction[direction-type/dynamics/f and offset=30]]'
check tuplet.musicxml '//measure[@number="2" and not(@implicit)]
    [count(note[duration=12 and time-modification[actual-notes=5 and normal-notes=4]])=5
    and note[1][beam="begin" and notations/tuplet[@type="start" and @bracket="no" and @show-number="none"]]
    and note[5][beam="end" and notations/tuplet/@type="stop"]]'
check tuplet.mei '//*[local-name()="tuplet" and @num="3" and @numbase="2" and @num.format="ratio"
    and @bracket.visible="true" and @num.place="above" and @bracket.place="above"]
    [*[local-name()="note" and @dots="1"] and *[local-name()="rest"] and *[local-name()="chord"]]'
check tuplet.mei '//*[local-name()="beam"]/*[local-name()="tuplet" and @num="5" and @numbase="4"
    and @num.visible="false" and @bracket.visible="false"][count(*[local-name()="note"])=5]'

check tie.musicxml '//measure[@number="1"]/note[tie/@type="start" and notations/tied/@type="start"]'
check tie.musicxml '//measure[@number="2"]/note[1][tie[1]/@type="stop" and tie[2]/@type="start" and notations/tied[1]/@type="stop" and notations/tied[2]/@type="start"]'
check tie.musicxml '//measure[@number="2"]/note[2][tie/@type="stop" and notations/tied/@type="stop"]'
check tie.mei 'count(//*[local-name()="tie"])=2'
check tie.mei '(//*[local-name()="tie"])[1][@startid=concat("#", (//*[local-name()="note"])[1]/@xml:id) and @endid=concat("#", (//*[local-name()="note"])[2]/@xml:id)]'
check tie.mei '(//*[local-name()="tie"])[2][@startid=concat("#", (//*[local-name()="note"])[2]/@xml:id) and @endid=concat("#", (//*[local-name()="note"])[3]/@xml:id)]'
check tie-chords.musicxml '//measure[@number="1"]/note[chord and pitch/step="G" and tie/@type="start" and notations/tied/@type="start"]'
check tie-chords.musicxml '//measure[@number="2"]/note[chord and pitch/step="G" and tie/@type="stop" and notations/tied/@type="stop"]'
check tie-chords.musicxml 'count(//note[tie or notations/tied])=2'
check tie-chords.mei 'count(//*[local-name()="tie"])=1 and //*[local-name()="tie"][@startid=concat("#", (//*[local-name()="chord"])[1]/*[local-name()="note" and @pname="g"]/@xml:id) and @endid=concat("#", (//*[local-name()="chord"])[2]/*[local-name()="note" and @pname="g"]/@xml:id)]'

check tie-chord-notes.musicxml '//measure[@number="1"][count(note)=3 and count(note[tie/@type="start" and notations/tied/@type="start"])=3]'
check tie-chord-notes.musicxml '//measure[@number="2"][count(note)=3 and count(note[tie/@type="stop" and notations/tied/@type="stop"])=3]'
check tie-chord-notes.mei 'count(//*[local-name()="tie"])=3 and count(//*[local-name()="chord"])=2'
for pitch in c e g; do
    check tie-chord-notes.mei "//*[local-name()='tie'][
        @startid=concat('#', (//*[local-name()='chord'])[1]/*[local-name()='note' and @pname='$pitch']/@xml:id)
        and @endid=concat('#', (//*[local-name()='chord'])[2]/*[local-name()='note' and @pname='$pitch']/@xml:id)
    ]"
done

check slur-chord.musicxml 'count(//slur)=2 and not(//note[chord]/notations/slur)'
check slur-chord.musicxml '//measure[@number="1"]/note[1]/notations/slur[@type="start" and @number="1" and @placement="below" and @line-type="dashed"]'
check slur-chord.musicxml '//measure[@number="2"]/note[1]/notations/slur[@type="stop" and @number="1"]'
check slur-chord.mei 'count(//*[local-name()="slur"])=1 and //*[local-name()="slur"][
    @startid=concat("#", //*[local-name()="chord"]/*[local-name()="note"][1]/@xml:id)
    and @endid=concat("#", //*[local-name()="measure" and @n="2"]//*[local-name()="note"]/@xml:id)
    and @curvedir="below" and @lform="dashed"]'

check slurs-overlap.musicxml '//measure/note[1]/notations/slur[@type="start" and @number="1"]'
check slurs-overlap.musicxml '//measure/note[2][count(notations)=1 and tie/@type="start"]/notations[tied/@type="start" and slur[@type="start" and @number="2"]]'
check slurs-overlap.musicxml '//measure/note[3][count(notations)=1 and tie/@type="stop"]/notations[tied/@type="stop" and slur[@type="stop" and @number="1"]]'
check slurs-overlap.musicxml '//measure/note[4]/notations/slur[@type="stop" and @number="2"]'
check slurs-overlap.mei 'count(//*[local-name()="slur"])=2 and count(//*[local-name()="tie"])=1'
check slurs-overlap.mei '//*[local-name()="slur"][
    @startid=concat("#", (//*[local-name()="note"])[1]/@xml:id)
    and @endid=concat("#", (//*[local-name()="note"])[3]/@xml:id) and @curvedir="above"]'
check slurs-overlap.mei '//*[local-name()="slur"][
    @startid=concat("#", (//*[local-name()="note"])[2]/@xml:id)
    and @endid=concat("#", (//*[local-name()="note"])[4]/@xml:id) and @curvedir="below"]'

check dynam-startid.musicxml '//measure[attributes/divisions=1]/direction[
    @placement="below" and direction-type/dynamics/sfz and offset=1 and staff=1]'
check dynam-startid.mei '//*[local-name()="dynam"][@staff="1" and @tstamp="2" and @place="below" and .="sfz"]'
check dynam-tstamp.musicxml '//measure[attributes/divisions=8 and backup/duration=16
    and count(note[voice=1 and duration=8])=2 and note[voice=2 and duration=16]]/direction[
    @placement="above" and direction-type/dynamics/other-dynamics="cresc. molto" and offset=1 and staff=1
    and not(preceding-sibling::note)]'
check dynam-tstamp.mei '//*[local-name()="dynam"][@staff="1" and @tstamp="1.25" and @place="above" and .="cresc. molto"]'

check hairpin-cross-measure.musicxml '//measure[@number="1" and attributes/divisions=4]/direction[
    @placement="below" and staff=1 and offset=4 and direction-type/wedge[
        @type="crescendo" and @number="1" and @niente="yes" and @line-type="dashed"]]'
check hairpin-cross-measure.musicxml '//measure[@number="2"]/direction[staff=1 and offset=3
    and direction-type/wedge[@type="stop" and @number="1" and @spread=10]]'
check hairpin-cross-measure.mei '//*[local-name()="hairpin"][@form="cres" and @tstamp="3" and @tstamp2="1m+2.5"
    and @staff="1" and @niente="true" and @lform="dashed" and @opening="2vu"]'

check hairpins-overlap.musicxml 'count(//wedge)=6'
check hairpins-overlap.musicxml '//direction[offset=1]/direction-type/wedge[@type="diminuendo" and @number="2" and @spread=15]'
check hairpins-overlap.musicxml '//direction[offset=3]/direction-type/wedge[@type="stop" and @number="2" and @niente="yes"]'
check hairpins-overlap.musicxml '//direction[offset=2 and direction-type/wedge[@type="stop" and @number="1"]]
    /following-sibling::direction[1][offset=2 and direction-type/wedge[@type="crescendo" and @number="1"]]'
check hairpins-overlap.mei 'count(//*[local-name()="hairpin"])=3'
check hairpins-overlap.mei '//*[local-name()="hairpin"][@form="cres" and @tstamp="1" and @tstamp2="0m+3"]'
check hairpins-overlap.mei '//*[local-name()="hairpin"][@form="dim" and @tstamp="2" and @tstamp2="0m+4" and @niente="true"]'
check hairpins-overlap.mei '//*[local-name()="hairpin"][@form="cres" and @tstamp="3" and @tstamp2="0m+5"]'

check fermata.musicxml 'count(//fermata)=5 and count(//caesura)=2 and count(//breath-mark)=2'
check fermata.musicxml '//measure[*[1][self::barline and @location="left"]/fermata and *[last()][self::barline and @location="right"]/fermata]'
check fermata.musicxml '//note[pitch/step="C" and not(chord)]/notations[slur/@type="start" and fermata[@type="upright" and @color="#AABBCC" and .="angled"]]'
check fermata.musicxml '//note[rest and staff=1]/notations[fermata[@type="inverted" and .="square"] and articulations/caesura[@placement="above"]]'
check fermata.musicxml '//note[rest/@measure="yes" and staff=2]/notations/fermata[@type="upright" and .="normal"]'
check fermata.musicxml '//note[pitch/step="G" and tie/@type="start" and count(notations)=1]/notations[
    tied/@type="start" and slur/@type="stop" and count(articulations)=1
    and articulations[breath-mark/@placement="above" and caesura/@placement="below"]]'
check fermata.musicxml '//note[pitch/step="G" and tie/@type="stop"]/notations/articulations/breath-mark[@placement="below"]'
check fermata.musicxml 'not(//note[chord or voice=2]/notations)'

check ornaments.musicxml '//note[pitch/step="C"]/notations/ornaments[
    trill-mark[@placement="above"] and accidental-mark[@placement="above" and .="natural"]
    and wavy-line[@type="start" and @number="1"]]'
check ornaments.musicxml '//note[pitch/step="D"]/notations[
    ornaments/inverted-mordent[@long="yes" and @placement="below"] and ornaments/wavy-line[@type="stop" and @number="1"]]'
check ornaments.musicxml '//note[pitch/step="E"]/notations/ornaments[
    delayed-turn and accidental-mark[@placement="below" and .="natural"]]'
check ornaments.mei '//*[local-name()="trill"][@extender="true" and @tstamp2="0m+3" and @accidupper="n"]'
check ornaments.mei '//*[local-name()="mordent"][@form="upper" and @long="true"]'
check ornaments.mei '//*[local-name()="turn"][@form="upper" and @delayed="true" and @accidlower="n"]'

check measure-barlines.musicxml 'count(//measure[@number="1"][
    *[1][self::barline and @location="left" and bar-style="dashed"]
    and *[last()][self::barline and @location="right" and bar-style="light-light"]])=2'
check measure-barlines.musicxml 'count(//measure[@number="2"]/barline[@location="right" and bar-style="light-heavy"])=2
    and not(//measure[@number="2"]/barline[@location="left"]) and count(//fermata)=1'
check measure-barlines.musicxml '//part[@id="P2"]/measure[@number="2"]/barline[
    bar-style="light-heavy" and fermata[@type="upright"]]'
check measure-barlines.mei '//*[local-name()="measure"][@n="1" and @left="dashed" and @right="dbl"]
    and //*[local-name()="measure"][@n="2" and @right="end"]'

check measure-repeats.musicxml 'count(//measure[*[1][self::barline and @location="left"
    and bar-style="heavy-light" and repeat/@direction="forward"]
    and *[last()][self::barline and @location="right" and bar-style="light-heavy" and repeat/@direction="backward"]])=2'
check measure-repeats.musicxml '//measure[@number="1"]/barline[@location="right"][
    *[1][self::bar-style] and *[2][self::fermata] and *[3][self::repeat]]'
check measure-repeats.mei 'count(//*[local-name()="measure"][@left="rptstart" and @right="rptend"])=2'

check directives.musicxml '//measure[@number="1" and attributes/divisions=8]/direction[
    @placement="below" and offset=4 and staff=1 and direction-type[1]/words="solo"
    and direction-type[2]/bracket[@type="start" and @number="1" and @line-end="none" and @line-type="dotted"]]'
check directives.musicxml '//measure[@number="1"]/direction[offset=12 and staff=1]/direction-type/bracket[
    @type="stop" and @number="1" and @line-end="none"]'
check directives.musicxml '//measure[@number="1"]/direction[@placement="above" and offset=8 and staff=1
    and direction-type[1]/words="sul pont." and direction-type[2]/dashes[@type="start" and @number="2"]]'
check directives.musicxml '//measure[@number="2"]/direction[offset=6 and staff=1]/direction-type/dashes[@type="stop" and @number="2"]'
check directives.musicxml '//measure[@number="2"]/direction[@placement="below" and offset=1 and staff=1]
    /direction-type/words[@xml:lang="fr" and normalize-space(.)="Très lié sans presser"]'
check directives.mei '//*[local-name()="dir"][@tstamp="2" and @tstamp2="0m+4" and @extender="true" and @lform="dotted"]'
check directives.mei '//*[local-name()="dir"][@tstamp="3" and @tstamp2="1m+2.5" and @extender="true" and @lform="dashed"]'
check directives.mei '//*[local-name()="dir"][@tstamp="1.25" and @xml:lang="fr" and not(@extender)]//*[local-name()="lb"]'

check directives-fractional.musicxml '//measure[attributes/divisions=10 and note/duration=40]/direction[
    offset=1 and direction-type[1]/words="pizz." and direction-type[2]/dashes[@type="start" and @number="1"]]'
check directives-fractional.musicxml '//direction[offset=13]/direction-type/dashes[@type="stop" and @number="1"]'
check directives-fractional.mei '//*[local-name()="dir"][@tstamp="1.1" and @tstamp2="0m+2.3" and @extender="true"]'
reloaded="$output/directives-fractional-mei-reload.musicxml"
"$verovio" -r data -f mei -t musicxml -o "$reloaded" "$output/directives-fractional.mei"
diff -u "$output/directives-fractional.musicxml" "$reloaded"

check tempo.musicxml '//measure[@number="1" and attributes/divisions=8]/direction[@placement="above" and not(offset)
    and staff=1 and direction-type[1]/words[@xml:lang="fr" and .="Modéré"]
    and direction-type[2]/metronome[@parentheses="yes" and beat-unit="quarter" and count(beat-unit-dot)=1 and per-minute="72.5"]
    and sound/@tempo="108.75"]'
check tempo.musicxml '//measure[@number="2"]/direction[offset[@sound="yes" and .=1] and staff=1 and sound/@tempo="92.125"
    and direction-type/metronome[@print-object="no" and beat-unit="quarter" and per-minute="92.125"]]'
check tempo.musicxml '//measure[@number="2"]/direction[offset[@sound="yes" and .=8] and staff=1 and sound/@tempo="108.75"
    and direction-type/words="a tempo" and not(direction-type/metronome)]'
check tempo.mei '//*[local-name()="tempo"][@tstamp="1" and @mm="72.5" and @mm.unit="4" and @mm.dots="1"
    and @midi.bpm="108.75" and @xml:lang="fr"]'
check tempo.mei '//*[local-name()="tempo"][@tstamp="1.25" and @midi.bpm="92.125" and not(@mm)]'
check tempo.mei '//*[local-name()="tempo"][@tstamp="3" and @midi.bpm="108.75" and normalize-space(.)="a tempo"]'

input="$fixtures/fermata-measure-rests.musicxml"
if [ -n "$schema" ]; then
    xmllint --nonet --noout --schema "$schema" "$input"
fi
"$verovio" -r data -f musicxml -t mei -o - "$input" \
    > "$output/fermata-measure-rests.mei" 2> "$output/fermata-measure-rests.log"
if [ -s "$output/fermata-measure-rests.log" ]; then
    cat "$output/fermata-measure-rests.log" >&2
    exit 1
fi
check fermata-measure-rests.mei '//*[local-name()="mRest" and @xml:id="visible-rest"]
    and //*[local-name()="fermata" and @xml:id="visible-fermata" and @startid="#visible-rest"]'
check fermata-measure-rests.mei '//*[local-name()="mSpace" and @xml:id="hidden-rest"]
    and //*[local-name()="fermata" and @xml:id="hidden-fermata" and @startid="#hidden-rest"]'
