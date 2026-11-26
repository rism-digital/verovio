"""MusicXML export checks for the musicxml-export directory of the test suite."""

from difflib import unified_diff
import os

from lxml import etree


def load_score(toolkit, source, input_format):
    if not toolkit.setInputFrom(input_format) or not toolkit.loadFile(str(source)):
        raise RuntimeError(f"Could not load {source} as {input_format}")


def write_output(path, contents):
    if not contents:
        raise RuntimeError(f"Verovio produced no output for {path}")
    path.write_bytes(contents.encode("utf-8"))


def compare_exports(first, second):
    if first.read_bytes() != second.read_bytes():
        diff = unified_diff(first.read_text(encoding="utf-8").splitlines(keepends=True),
                            second.read_text(encoding="utf-8").splitlines(keepends=True),
                            fromfile=str(first), tofile=str(second))
        raise RuntimeError(f"MusicXML round-trip changed {first.name}\n{''.join(diff)}")


def check_output(path):
    checks = CHECKS.get(path.name)
    if not checks:
        return
    document = etree.parse(path)
    for expression in checks:
        if not document.xpath(f"boolean({expression})"):
            raise RuntimeError(f"Assertion failed in {path}: {expression}")


def round_trip(toolkit, source, output_dir, schema):
    name = source.stem
    first_xml = output_dir / f"{name}.musicxml"
    second_xml = output_dir / f"{name}-roundtrip.musicxml"
    first_mei = output_dir / f"{name}.mei"
    original_svg = output_dir / f"{name}-original.svg"
    imported_svg = output_dir / f"{name}-imported.svg"

    load_score(toolkit, source, "mei")
    write_output(first_xml, toolkit.exportToMusicXML())
    write_output(original_svg, toolkit.renderToSVG(1))

    load_score(toolkit, first_xml, "musicxml")
    write_output(second_xml, toolkit.exportToMusicXML())
    write_output(first_mei, toolkit.getMEI())

    load_score(toolkit, second_xml, "musicxml")
    write_output(output_dir / f"{name}-roundtrip.mei", toolkit.getMEI())

    load_score(toolkit, first_mei, "mei")
    write_output(imported_svg, toolkit.renderToSVG(1))

    if schema is not None:
        for exported in (first_xml, second_xml):
            schema.assertValid(etree.parse(exported))
    compare_exports(first_xml, second_xml)

    if name == "directives-fractional":
        reloaded_xml = output_dir / f"{name}-mei-reload.musicxml"
        write_output(reloaded_xml, toolkit.exportToMusicXML())
        compare_exports(first_xml, reloaded_xml)

    for output in (first_xml, first_mei, original_svg, imported_svg):
        check_output(output)


def check_import(toolkit, source, output_dir, schema):
    if schema is not None:
        schema.assertValid(etree.parse(source))
    output = output_dir / f"{source.stem}.mei"
    log = output.with_suffix(".log")
    # Capture native Verovio diagnostics for the import-only fixture.
    with log.open("wb") as capture:
        stderr = os.dup(2)
        try:
            os.dup2(capture.fileno(), 2)
            load_score(toolkit, source, "musicxml")
            write_output(output, toolkit.getMEI())
        finally:
            os.dup2(stderr, 2)
            os.close(stderr)
    if log.stat().st_size:
        raise RuntimeError(f"Unexpected diagnostics importing {source}:\n{log.read_text(encoding='utf-8')}")
    check_output(output)


def run_musicxml_export_tests(toolkit, fixtures, output_dir, shortlist, schema_path):
    selected = [source for source in sorted(fixtures.iterdir())
                if source.is_file() and not source.name.startswith(".")
                and source.suffix in (".mei", ".musicxml")
                and (not shortlist or f"{fixtures.name}/{source.name}" in shortlist)]
    if not selected:
        return
    if any(source.suffix == ".mei" for source in selected) and not hasattr(toolkit, "exportToMusicXML"):
        raise RuntimeError("The MusicXML export tests require a Verovio Python build with exportToMusicXML()")

    schema = None
    if schema_path is not None:
        schema = etree.XMLSchema(etree.parse(schema_path, etree.XMLParser(no_network=True)))

    toolkit.resetOptions()
    toolkit.setOptions({"breaks": "none", "adjustPageHeight": True})
    try:
        for source in selected:
            print(f"Evaluating MusicXML export fixture {source.name}")
            if source.suffix == ".mei":
                round_trip(toolkit, source, output_dir, schema)
            else:
                check_import(toolkit, source, output_dir, schema)
    finally:
        toolkit.setInputFrom("auto")
        toolkit.resetOptions()


CHECKS = {
    "staff-instrument.musicxml": (
        """/score-partwise/part-list/score-part[@id="violin"]""",
        """//measure[@number="1"]/attributes[divisions=2 and key/fifths=1]""",
        """//measure[@number="1"][not(@implicit) and backup/duration=8 and forward/duration=2]""",
        """//note[chord and duration=3 and type="quarter" and dot]""",
        """//note[pitch/alter=1 and not(accidental)]""",
        """//note[accidental="natural" and duration=1]""",
        """//note[@print-object="no" and rest and voice=2 and duration=4]""",
        """//measure[@number="2"]/note[rest/@measure="yes" and duration=8]""",
    ),
    "staff-instrument.mei": (
        """//*[local-name()="staffDef"]/*[local-name()="instrDef" and @xml:id="violin"]""",
    ),
    "group-instrument.musicxml": (
        """count(/score-partwise/part)=2 and /score-partwise/part[@id="P1"]""",
        """//part[@id="piano"]/measure/attributes[staves=2 and clef[@number="2"]/sign="F"]""",
        """//part[@id="piano"]/measure/note[staff=2 and voice=2 and duration=3]""",
    ),
    "group-instrument.mei": (
        """//*[local-name()="staffGrp"]/*[local-name()="instrDef" and @xml:id="piano"]""",
    ),
    "layer-instruments.musicxml": (
        """count(/score-partwise/part-list/score-part)=1 and count(/score-partwise/part)=1""",
        """//score-part[score-instrument[@id="oboe" and instrument-name="Oboe"] and score-instrument[@id="clarinet" and instrument-name="Clarinet"]]""",
        """//part/measure[@number="0" and @implicit="yes"][not(attributes/staves) and backup/duration=1]""",
        """//part/measure/note[voice=1 and instrument/@id="oboe" and pitch/step="G"]""",
        """//part/measure/note[voice=2 and instrument/@id="clarinet" and pitch/step="E"]""",
    ),
    "layer-instruments.mei": (
        """count(//*[local-name()="staffDef"])=1 and count(//*[local-name()="measure"]/*[local-name()="staff"])=1""",
        """//*[local-name()="staff"]/*[local-name()="layer" and @n="1"]/*[local-name()="note" and @pname="g"]""",
        """//*[local-name()="staff"]/*[local-name()="layer" and @n="2"]/*[local-name()="note" and @pname="e"]""",
        """//*[local-name()="staffDef"]/*[local-name()="layerDef" and @n="1"]/*[local-name()="instrDef" and @xml:id="oboe" and @label="Oboe"]""",
        """//*[local-name()="staffDef"]/*[local-name()="layerDef" and @n="2"]/*[local-name()="instrDef" and @xml:id="clarinet" and @label="Clarinet"]""",
    ),
    "layer-instruments-original.svg": (
        """count(//*[@class="staff"])=1""",
    ),
    "layer-instruments-imported.svg": (
        """count(//*[@class="staff"])=1""",
    ),
    "beam.musicxml": (
        """//measure[attributes/divisions=2 and not(backup) and count(note[voice=1 and type="eighth" and duration=1])=4]""",
        """//measure[note[1]/beam[@number="1" and .="begin"] and note[2]/beam[@number="1" and .="continue"] and note[3]/beam[@number="1" and .="continue"] and note[4]/beam[@number="1" and .="end"]]""",
    ),
    "beam.mei": (
        """//*[local-name()="beam"][count(*[local-name()="note" and @dur="8"])=4]""",
    ),
    "beam-two-voices.musicxml": (
        """//measure[backup/duration=4 and count(note[voice=1]/beam[@number="1"])=4 and count(note[chord and voice=1])=2 and not(note[chord or voice=2]/beam)]""",
        """//note[voice=1 and rest]/beam[@number="1" and .="continue"]""",
    ),
    "beam-two-voices.mei": (
        """//*[local-name()="layer" and @n="1"]/*[local-name()="beam"][count(*[local-name()="chord"])=2 and *[local-name()="rest"]]""",
        """//*[local-name()="layer" and @n="2"]/*[local-name()="note" and @dur="2"]""",
    ),
    "beam-levels.musicxml": (
        """//measure[@number="1" and attributes/divisions=8 and not(@implicit)]
            [note[1][beam[@number="1" and .="begin"] and beam[@number="2" and .="forward hook"]]
            and note[2][chord and not(beam)]
            and note[3][type="eighth" and count(beam)=1 and beam[@number="1" and .="continue"]]
            and note[4][beam[@number="1" and .="end"] and beam[@number="2" and .="backward hook"]]]""",
        """//measure[@number="2" and not(@implicit)]
            [count(note[1]/beam[.="begin"])=3
            and note[2][beam[@number="2" and .="continue"] and beam[@number="3" and .="end"]]
            and note[3][rest and count(beam[.="continue"])=2 and not(beam[@number="3"])]
            and note[4]/beam[@number="3" and .="begin"]
            and count(note[5]/beam[.="end"])=3 and note[6][not(beam)]]""",
    ),
    "beam-levels.mei": (
        """//*[local-name()="measure" and @n="2"]//*[local-name()="beam"]
            [count(*[local-name()="note" and @dur="32"])=4 and *[local-name()="rest" and @dur="32"]
            and *[local-name()="note" and @pname="d" and @breaksec="2"]]""",
    ),
    "tuplet.musicxml": (
        """//measure[@number="1" and not(@implicit) and attributes/divisions=30]
            /note[1][duration=15 and type="eighth" and dot and count(notations)=1
            and notations[slur/@type="start" and tuplet[@type="start" and @bracket="yes" and @show-number="both" and @placement="above"]]]""",
        """//measure[@number="1"][count(note[time-modification[actual-notes=3 and normal-notes=2]])=4
            and note[2][rest and duration=5 and type="16th"] and note[3][duration=10 and notations/tuplet/@type="stop"]
            and note[4][chord and duration=10 and not(notations/tuplet)]
            and note[5][duration=90 and not(time-modification)] and direction[direction-type/dynamics/f and offset=30]]""",
        """//measure[@number="2" and not(@implicit)]
            [count(note[duration=12 and time-modification[actual-notes=5 and normal-notes=4]])=5
            and note[1][beam="begin" and notations/tuplet[@type="start" and @bracket="no" and @show-number="none"]]
            and note[5][beam="end" and notations/tuplet/@type="stop"]]""",
    ),
    "tuplet.mei": (
        """//*[local-name()="tuplet" and @num="3" and @numbase="2" and @num.format="ratio"
            and @bracket.visible="true" and @num.place="above" and @bracket.place="above"]
            [*[local-name()="note" and @dots="1"] and *[local-name()="rest"] and *[local-name()="chord"]]""",
        """//*[local-name()="beam"]/*[local-name()="tuplet" and @num="5" and @numbase="4"
            and @num.visible="false" and @bracket.visible="false"][count(*[local-name()="note"])=5]""",
    ),
    "tie.musicxml": (
        """//measure[@number="1"]/note[tie/@type="start" and notations/tied/@type="start"]""",
        """//measure[@number="2"]/note[1][tie[1]/@type="stop" and tie[2]/@type="start" and notations/tied[1]/@type="stop" and notations/tied[2]/@type="start"]""",
        """//measure[@number="2"]/note[2][tie/@type="stop" and notations/tied/@type="stop"]""",
    ),
    "tie.mei": (
        """count(//*[local-name()="tie"])=2""",
        """(//*[local-name()="tie"])[1][@startid=concat("#", (//*[local-name()="note"])[1]/@xml:id) and @endid=concat("#", (//*[local-name()="note"])[2]/@xml:id)]""",
        """(//*[local-name()="tie"])[2][@startid=concat("#", (//*[local-name()="note"])[2]/@xml:id) and @endid=concat("#", (//*[local-name()="note"])[3]/@xml:id)]""",
    ),
    "tie-chords.musicxml": (
        """//measure[@number="1"]/note[chord and pitch/step="G" and tie/@type="start" and notations/tied/@type="start"]""",
        """//measure[@number="2"]/note[chord and pitch/step="G" and tie/@type="stop" and notations/tied/@type="stop"]""",
        """count(//note[tie or notations/tied])=2""",
    ),
    "tie-chords.mei": (
        """count(//*[local-name()="tie"])=1 and //*[local-name()="tie"][@startid=concat("#", (//*[local-name()="chord"])[1]/*[local-name()="note" and @pname="g"]/@xml:id) and @endid=concat("#", (//*[local-name()="chord"])[2]/*[local-name()="note" and @pname="g"]/@xml:id)]""",
    ),
    "tie-chord-notes.musicxml": (
        """//measure[@number="1"][count(note)=3 and count(note[tie/@type="start" and notations/tied/@type="start"])=3]""",
        """//measure[@number="2"][count(note)=3 and count(note[tie/@type="stop" and notations/tied/@type="stop"])=3]""",
    ),
    "tie-chord-notes.mei": (
        """count(//*[local-name()="tie"])=3 and count(//*[local-name()="chord"])=2""",
        """//*[local-name()='tie'][
                @startid=concat('#', (//*[local-name()='chord'])[1]/*[local-name()='note' and @pname='c']/@xml:id)
                and @endid=concat('#', (//*[local-name()='chord'])[2]/*[local-name()='note' and @pname='c']/@xml:id)
            ]""",
        """//*[local-name()='tie'][
                @startid=concat('#', (//*[local-name()='chord'])[1]/*[local-name()='note' and @pname='e']/@xml:id)
                and @endid=concat('#', (//*[local-name()='chord'])[2]/*[local-name()='note' and @pname='e']/@xml:id)
            ]""",
        """//*[local-name()='tie'][
                @startid=concat('#', (//*[local-name()='chord'])[1]/*[local-name()='note' and @pname='g']/@xml:id)
                and @endid=concat('#', (//*[local-name()='chord'])[2]/*[local-name()='note' and @pname='g']/@xml:id)
            ]""",
    ),
    "slur-chord.musicxml": (
        """count(//slur)=2 and not(//note[chord]/notations/slur)""",
        """//measure[@number="1"]/note[1]/notations/slur[@type="start" and @number="1" and @placement="below" and @line-type="dashed"]""",
        """//measure[@number="2"]/note[1]/notations/slur[@type="stop" and @number="1"]""",
    ),
    "slur-chord.mei": (
        """count(//*[local-name()="slur"])=1 and //*[local-name()="slur"][
            @startid=concat("#", //*[local-name()="chord"]/*[local-name()="note"][1]/@xml:id)
            and @endid=concat("#", //*[local-name()="measure" and @n="2"]//*[local-name()="note"]/@xml:id)
            and @curvedir="below" and @lform="dashed"]""",
    ),
    "slurs-overlap.musicxml": (
        """//measure/note[1]/notations/slur[@type="start" and @number="1"]""",
        """//measure/note[2][count(notations)=1 and tie/@type="start"]/notations[tied/@type="start" and slur[@type="start" and @number="2"]]""",
        """//measure/note[3][count(notations)=1 and tie/@type="stop"]/notations[tied/@type="stop" and slur[@type="stop" and @number="1"]]""",
        """//measure/note[4]/notations/slur[@type="stop" and @number="2"]""",
    ),
    "slurs-overlap.mei": (
        """count(//*[local-name()="slur"])=2 and count(//*[local-name()="tie"])=1""",
        """//*[local-name()="slur"][
            @startid=concat("#", (//*[local-name()="note"])[1]/@xml:id)
            and @endid=concat("#", (//*[local-name()="note"])[3]/@xml:id) and @curvedir="above"]""",
        """//*[local-name()="slur"][
            @startid=concat("#", (//*[local-name()="note"])[2]/@xml:id)
            and @endid=concat("#", (//*[local-name()="note"])[4]/@xml:id) and @curvedir="below"]""",
    ),
    "dynam-startid.musicxml": (
        """//measure[attributes/divisions=1]/direction[
            @placement="below" and direction-type/dynamics/sfz and offset=1 and staff=1]""",
    ),
    "dynam-startid.mei": (
        """//*[local-name()="dynam"][@staff="1" and @tstamp="2" and @place="below" and .="sfz"]""",
    ),
    "dynam-tstamp.musicxml": (
        """//measure[attributes/divisions=8 and backup/duration=16
            and count(note[voice=1 and duration=8])=2 and note[voice=2 and duration=16]]/direction[
            @placement="above" and direction-type/dynamics/other-dynamics="cresc. molto" and offset=1 and staff=1
            and not(preceding-sibling::note)]""",
    ),
    "dynam-tstamp.mei": (
        """//*[local-name()="dynam"][@staff="1" and @tstamp="1.25" and @place="above" and .="cresc. molto"]""",
    ),
    "hairpin-cross-measure.musicxml": (
        """//measure[@number="1" and attributes/divisions=4]/direction[
            @placement="below" and staff=1 and offset=4 and direction-type/wedge[
                @type="crescendo" and @number="1" and @niente="yes" and @line-type="dashed"]]""",
        """//measure[@number="2"]/direction[staff=1 and offset=3
            and direction-type/wedge[@type="stop" and @number="1" and @spread=10]]""",
    ),
    "hairpin-cross-measure.mei": (
        """//*[local-name()="hairpin"][@form="cres" and @tstamp="3" and @tstamp2="1m+2.5"
            and @staff="1" and @niente="true" and @lform="dashed" and @opening="2vu"]""",
    ),
    "hairpins-overlap.musicxml": (
        """count(//wedge)=6""",
        """//direction[offset=1]/direction-type/wedge[@type="diminuendo" and @number="2" and @spread=15]""",
        """//direction[offset=3]/direction-type/wedge[@type="stop" and @number="2" and @niente="yes"]""",
        """//direction[offset=2 and direction-type/wedge[@type="stop" and @number="1"]]
            /following-sibling::direction[1][offset=2 and direction-type/wedge[@type="crescendo" and @number="1"]]""",
    ),
    "hairpins-overlap.mei": (
        """count(//*[local-name()="hairpin"])=3""",
        """//*[local-name()="hairpin"][@form="cres" and @tstamp="1" and @tstamp2="0m+3"]""",
        """//*[local-name()="hairpin"][@form="dim" and @tstamp="2" and @tstamp2="0m+4" and @niente="true"]""",
        """//*[local-name()="hairpin"][@form="cres" and @tstamp="3" and @tstamp2="0m+5"]""",
    ),
    "fermata.musicxml": (
        """count(//fermata)=5 and count(//caesura)=2 and count(//breath-mark)=2""",
        """//measure[*[1][self::barline and @location="left"]/fermata and *[last()][self::barline and @location="right"]/fermata]""",
        """//note[pitch/step="C" and not(chord)]/notations[slur/@type="start" and fermata[@type="upright" and @color="#AABBCC" and .="angled"]]""",
        """//note[rest and staff=1]/notations[fermata[@type="inverted" and .="square"] and articulations/caesura[@placement="above"]]""",
        """//note[rest/@measure="yes" and staff=2]/notations/fermata[@type="upright" and .="normal"]""",
        """//note[pitch/step="G" and tie/@type="start" and count(notations)=1]/notations[
            tied/@type="start" and slur/@type="stop" and count(articulations)=1
            and articulations[breath-mark/@placement="above" and caesura/@placement="below"]]""",
        """//note[pitch/step="G" and tie/@type="stop"]/notations/articulations/breath-mark[@placement="below"]""",
        """not(//note[chord or voice=2]/notations)""",
    ),
    "ornaments.musicxml": (
        """//note[pitch/step="C"]/notations/ornaments[
            trill-mark[@placement="above"] and accidental-mark[@placement="above" and .="natural"]
            and wavy-line[@type="start" and @number="1"]]""",
        """//note[pitch/step="D"]/notations[
            ornaments/inverted-mordent[@long="yes" and @placement="below"] and ornaments/wavy-line[@type="stop" and @number="1"]]""",
        """//note[pitch/step="E"]/notations/ornaments[
            delayed-turn and accidental-mark[@placement="below" and .="natural"]]""",
    ),
    "ornaments.mei": (
        """//*[local-name()="trill"][@extender="true" and @tstamp2="0m+3" and @accidupper="n"]""",
        """//*[local-name()="mordent"][@form="upper" and @long="true"]""",
        """//*[local-name()="turn"][@form="upper" and @delayed="true" and @accidlower="n"]""",
    ),
    "measure-barlines.musicxml": (
        """count(//measure[@number="1"][
            *[1][self::barline and @location="left" and bar-style="dashed"]
            and *[last()][self::barline and @location="right" and bar-style="light-light"]])=2""",
        """count(//measure[@number="2"]/barline[@location="right" and bar-style="light-heavy"])=2
            and not(//measure[@number="2"]/barline[@location="left"]) and count(//fermata)=1""",
        """//part[@id="P2"]/measure[@number="2"]/barline[
            bar-style="light-heavy" and fermata[@type="upright"]]""",
    ),
    "measure-barlines.mei": (
        """//*[local-name()="measure"][@n="1" and @left="dashed" and @right="dbl"]
            and //*[local-name()="measure"][@n="2" and @right="end"]""",
    ),
    "measure-repeats.musicxml": (
        """count(//measure[*[1][self::barline and @location="left"
            and bar-style="heavy-light" and repeat/@direction="forward"]
            and *[last()][self::barline and @location="right" and bar-style="light-heavy" and repeat/@direction="backward"]])=2""",
        """//measure[@number="1"]/barline[@location="right"][
            *[1][self::bar-style] and *[2][self::fermata] and *[3][self::repeat]]""",
    ),
    "measure-repeats.mei": (
        """count(//*[local-name()="measure"][@left="rptstart" and @right="rptend"])=2""",
    ),
    "directives.musicxml": (
        """//measure[@number="1" and attributes/divisions=8]/direction[
            @placement="below" and offset=4 and staff=1 and direction-type[1]/words="solo"
            and direction-type[2]/bracket[@type="start" and @number="1" and @line-end="none" and @line-type="dotted"]]""",
        """//measure[@number="1"]/direction[offset=12 and staff=1]/direction-type/bracket[
            @type="stop" and @number="1" and @line-end="none"]""",
        """//measure[@number="1"]/direction[@placement="above" and offset=8 and staff=1
            and direction-type[1]/words="sul pont." and direction-type[2]/dashes[@type="start" and @number="2"]]""",
        """//measure[@number="2"]/direction[offset=6 and staff=1]/direction-type/dashes[@type="stop" and @number="2"]""",
        """//measure[@number="2"]/direction[@placement="below" and offset=1 and staff=1]
            /direction-type/words[@xml:lang="fr" and normalize-space(.)="Très lié sans presser"]""",
    ),
    "directives.mei": (
        """//*[local-name()="dir"][@tstamp="2" and @tstamp2="0m+4" and @extender="true" and @lform="dotted"]""",
        """//*[local-name()="dir"][@tstamp="3" and @tstamp2="1m+2.5" and @extender="true" and @lform="dashed"]""",
        """//*[local-name()="dir"][@tstamp="1.25" and @xml:lang="fr" and not(@extender)]//*[local-name()="lb"]""",
    ),
    "directives-fractional.musicxml": (
        """//measure[attributes/divisions=10 and note/duration=40]/direction[
            offset=1 and direction-type[1]/words="pizz." and direction-type[2]/dashes[@type="start" and @number="1"]]""",
        """//direction[offset=13]/direction-type/dashes[@type="stop" and @number="1"]""",
    ),
    "directives-fractional.mei": (
        """//*[local-name()="dir"][@tstamp="1.1" and @tstamp2="0m+2.3" and @extender="true"]""",
    ),
    "tempo.musicxml": (
        """//measure[@number="1" and attributes/divisions=8]/direction[@placement="above" and not(offset)
            and staff=1 and direction-type[1]/words[@xml:lang="fr" and .="Modéré"]
            and direction-type[2]/metronome[@parentheses="yes" and beat-unit="quarter" and count(beat-unit-dot)=1 and per-minute="72.5"]
            and sound/@tempo="108.75"]""",
        """//measure[@number="2"]/direction[offset[@sound="yes" and .=1] and staff=1 and sound/@tempo="92.125"
            and direction-type/metronome[@print-object="no" and beat-unit="quarter" and per-minute="92.125"]]""",
        """//measure[@number="2"]/direction[offset[@sound="yes" and .=8] and staff=1 and sound/@tempo="108.75"
            and direction-type/words="a tempo" and not(direction-type/metronome)]""",
    ),
    "tempo.mei": (
        """//*[local-name()="tempo"][@tstamp="1" and @mm="72.5" and @mm.unit="4" and @mm.dots="1"
            and @midi.bpm="108.75" and @xml:lang="fr"]""",
        """//*[local-name()="tempo"][@tstamp="1.25" and @midi.bpm="92.125" and not(@mm)]""",
        """//*[local-name()="tempo"][@tstamp="3" and @midi.bpm="108.75" and normalize-space(.)="a tempo"]""",
    ),
    "signature-changes.musicxml": (
        """count(//part[
            measure[@number="1"]/attributes[divisions=4 and key/fifths=1 and time[beats=4 and beat-type=4]]
            and measure[@number="2"]/attributes[key[fifths=-2 and mode="minor" and not(@number)]
            and time[beats=5 and beat-type=8 and not(@number)] and not(divisions)]
            and measure[@number="3"]/attributes[key/fifths=0 and not(time)]
            and measure[@number="4"]/attributes[time[@symbol="cut" and beats=2 and beat-type=2] and not(key)]])=2
            and not(//measure[@implicit])""",
        """//part[@id="P1"]/measure[@number="2"]
            [*[1][self::attributes] and note/pitch[step="B" and alter=-1] and barline[@location="right"]/fermata]
            /direction[offset=6 and direction-type/dashes/@type="stop"]""",
        """//part[@id="P1"]/measure[@number="3"]
            [note[rest/@measure="yes" and duration=10] and direction[direction-type/dynamics/p and offset=4]]""",
        """//part[@id="P2"][count(measure[note[rest/@measure="yes"]])=4
            and count(measure[@number="2" or @number="3"]/note[duration=10])=2 and measure[@number="4"]/note[duration=16]]
            /measure[@number="2"]/direction[offset[@sound="yes" and .=3] and sound/@tempo=90]""",
    ),
    "signature-changes.mei": (
        """//*[local-name()="measure" and @n="2"]
            [preceding-sibling::*[1][local-name()="scoreDef"]
            [*[local-name()="keySig" and @sig="2f" and @mode="minor"] and *[local-name()="meterSig" and @count="5" and @unit="8"]]
            and *[local-name()="tempo" and @staff="2" and @tstamp="2.5"]]""",
        """//*[local-name()="dir" and @tstamp="2" and @tstamp2="1m+4" and @extender="true"]""",
    ),
    "clef-changes.musicxml": (
        """//part[@id="piano"]/measure[@number="1"][
            attributes[1][divisions=2 and clef[@number="1" and sign="G" and line=2 and clef-octave-change=-1]
            and clef[@number="2" and sign="F" and line=4]]
            and attributes[clef[@number="1" and sign="C" and line=3]
            and preceding-sibling::note[1][pitch/step="D" and beam="continue"]
            and following-sibling::note[1][pitch/step="E" and beam="continue"]]
            and attributes[clef[@number="2" and sign="C" and line=4]
            and preceding-sibling::note[1][voice=3 and staff=2] and following-sibling::note[1][voice=3 and staff=2]]
            and count(attributes/clef)=4 and count(note[voice=1]/beam)=4]""",
        """//measure[@number="2"][
            attributes[not(preceding-sibling::note)]/clef[@number="1" and @after-barline="yes"
            and sign="G" and clef-octave-change=1]
            and attributes[clef[@number="1" and sign="F" and line=4 and not(@after-barline)]
            and preceding-sibling::note[1][voice=2 and rest/@measure="yes"]]
            and count(attributes/clef)=2]""",
    ),
    "clef-changes.mei": (
        """//*[local-name()="layer" and @n="2"]/*[local-name()="clef"
            and @sameas=concat("#", //*[local-name()="beam"]/*[local-name()="clef" and @shape="C"]/@xml:id)]""",
        """//*[local-name()="measure" and @n="2"]//*[local-name()="layer" and @n="1"]
            /*[1][local-name()="clef" and @shape="G" and @dis="8" and @dis.place="above"]""",
    ),
    "grace-notes.musicxml": (
        """//measure[@number="1" and attributes/divisions=2 and not(@implicit)][backup/duration=4
            and count(note[grace and not(duration)])=6 and count(note[voice=1 and duration=1])=4
            and note[1][grace/@slash="yes" and stem="up" and notations/slur/@type="start"]
            and note[2][grace[not(@slash)] and type="16th"] and note[3][grace and chord and not(beam)]
            and note[4][duration=1 and beam="begin" and notations[fermata and slur/@type="stop"]]
            and not(note[grace]/notations/fermata)]""",
        """//measure[@number="1"][note[5][beam="continue"]
            and note[6][grace and beam[@number="1" and .="begin"] and beam[@number="2" and .="begin"]]
            and note[7][grace and beam[@number="1" and .="end"] and beam[@number="2" and .="end"]]
            and note[8][beam="continue"] and note[9][grace/@slash="yes" and not(beam)]
            and note[10][beam="end"]]""",
        """//measure[@number="2" and not(@implicit)][count(note[voice=1 and grace and not(duration)])=2
            and note[1]/beam[@number="3" and .="begin"] and note[2]/beam[@number="3" and .="end"]
            and forward/duration=1 and backup/duration=1 and note[3][voice=2 and duration=4]]""",
    ),
    "grace-notes.mei": (
        """//*[local-name()="measure" and @n="1"]//*[local-name()="layer" and @n="1"]
            [*[local-name()="note" and @grace="unacc" and @stem.mod="1slash"]
            and *[local-name()="chord" and @grace="acc" and count(*[local-name()="note"])=2]
            and *[local-name()="beam"]/*[local-name()="beam" and count(*[local-name()="note" and @grace="acc"])=2]]""",
        """//*[local-name()="measure" and @n="2"]//*[local-name()="layer" and @n="2"]
            [*[local-name()="note" and @pname="g" and @dur="2"]]""",
    ),
    "multi-rest.musicxml": (
        """count(//part[count(measure)=6
            and measure[1][@number="1" and barline[@location="left"]/repeat/@direction="forward"]
            /attributes/measure-style/multiple-rest[@use-symbols="no" and .=3]
            and measure[2][@number="2" and not(attributes or barline)]
            and measure[3][@number="3" and barline[@location="right"]/bar-style="light-light"]
            and measure[4][@number="4" and attributes[time/beats=2 and key/fifths=1]
            and attributes/measure-style/multiple-rest[@use-symbols="yes" and .=2]]
            and measure[5][@number="5" and barline[@location="right"]/repeat/@direction="backward"]
            and measure[6][@number="6" and note/pitch]])=2 and count(//multiple-rest)=4 and not(//measure[@implicit])""",
        """//part[@id="P1"]/measure[@number="1"]/direction[direction-type/words="Andante"]
            /following-sibling::attributes/measure-style/multiple-rest""",
        """//part[@id="piano"][count(measure[position()<=3][backup/duration=3
            and count(note[rest/@measure="yes" and duration=3 and not(@print-object)])=2])=3
            and count(measure[position()=4 or position()=5][backup/duration=4
            and count(note[rest/@measure="yes" and duration=4 and not(@print-object)])=2])=2]""",
    ),
    "multi-rest.mei": (
        """count(//*[local-name()="measure"])=3
            and //*[local-name()="measure" and @n="1" and @left="rptstart" and @right="dbl"]
            [count(.//*[local-name()="multiRest" and @num="3" and @block="true"])=2 and not(.//*[local-name()="clef"])]
            and //*[local-name()="measure" and @n="4" and @right="rptend"]
            [count(.//*[local-name()="multiRest" and @num="2" and @block="false"])=2]
            and //*[local-name()="measure" and @n="6"][count(.//*[local-name()="note"])=3]""",
    ),
    "lyrics.musicxml": (
        """//measure[@number="1"][note[1]/lyric[@number="1" and @name="Latin" and @placement="below"]
            [text[1]="1." and elision and syllabic="begin" and text[2][@xml:lang="la" and .="Glo"]]
            and note[2]/lyric[syllabic="middle" and text="ri"]
            and note[3][not(chord) and lyric[@number="1"][syllabic="end" and text="a" and extend/@type="start"]]
            and note[4][chord and not(lyric)]]""",
        """//measure[@number="1"]/note[3]/lyric[@number="2" and @name="French"
            and @placement="above" and @color="#224466"][syllabic[1]="single" and syllabic[2]="begin"
            and text[1][@xml:lang="fr" and @font-style="italic" and .="Dieu"] and elision="‿"
            and text[2][@xml:lang="fr" and @font-weight="bold" and .="cré"]]""",
        """//measure[@number="2"][note[1]/lyric[@number="1" and extend/@type="stop" and not(text)]
            and not(note[2]/lyric) and note[3]/lyric/text[@line-through="1" and .="in"]
            and note[4]/lyric[@number="1" and syllabic="single" and text="pace"]]""",
    ),
    "lyrics.mei": (
        """//*[local-name()="verse" and @n="1" and @label="Latin"][*[local-name()="label" and .="1."]]
            and //*[local-name()="chord"][*[local-name()="verse" and @n="1"]/*[local-name()="syl" and @con="u" and @wordpos="t"]
            and *[local-name()="verse" and @n="2"]/*[local-name()="syl" and @con="b" and @fontstyle="italic"]]
            and //*[local-name()="measure" and @n="2"]//*[local-name()="note" and @pname="f"]
            /*[local-name()="verse" and @n="1"]/*[local-name()="syl" and not(node())]""",
    ),
    "fermata-measure-rests.mei": (
        """//*[local-name()="mRest" and @xml:id="visible-rest"]
            and //*[local-name()="fermata" and @xml:id="visible-fermata" and @startid="#visible-rest"]""",
        """//*[local-name()="mSpace" and @xml:id="hidden-rest"]
            and //*[local-name()="fermata" and @xml:id="hidden-fermata" and @startid="#hidden-rest"]""",
    ),
}
