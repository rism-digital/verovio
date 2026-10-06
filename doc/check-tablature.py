import argparse
import copy
import re
import shutil
import subprocess
import tempfile
import xml.etree.ElementTree as ET
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
FIXTURES = REPO / 'doc/tests/musicxml/tablature'
ROOT = None
MEI = '{http://www.music-encoding.org/ns/mei}'
SVG = '{http://www.w3.org/2000/svg}'
XML_ID = '{http://www.w3.org/XML/1998/namespace}id'


def render(binary, root, name, fmt='mei', breaks='none'):
    source = ROOT / (name + '.musicxml')
    ET.ElementTree(root).write(source, encoding='utf-8', xml_declaration=True)
    out = ROOT / name
    args = [str(binary), '-r', str(REPO / 'data'), '--xml-id-seed', '1',
            '-t', fmt, '-o', str(out)]
    if fmt == 'svg':
        args += ['--breaks', breaks, '--header', 'none', '--footer', 'none',
                 '--page-width', '1000', '--adjust-page-height', '--svg-view-box',
                 '--svg-bounding-boxes']
    result = subprocess.run(args + [str(source)], capture_output=True, text=True)
    assert result.returncode == 0, result.stderr
    return ET.parse(out.with_suffix('.' + fmt)).getroot()


def check_ties(binary, prefix):
    original = ET.parse(ROOT / 'tab-chord-ties.musicxml').getroot()
    variants = {'original': original}
    reversed_end = copy.deepcopy(original)
    measure = reversed_end.find('part/measure')
    notes = measure.findall('note')
    for note in notes[2:]:
        measure.remove(note)
        chord = note.find('chord')
        if chord is not None:
            note.remove(chord)
    notes[2].insert(0, ET.Element('chord'))
    measure.extend([notes[3], notes[2]])
    variants['reversed-end'] = reversed_end

    no_pitch = copy.deepcopy(original)
    for note in no_pitch.findall('.//note'):
        note.remove(note.find('pitch'))
    variants['no-pitch'] = no_pitch

    across = copy.deepcopy(original)
    measure = across.find('part/measure')
    measure.find('attributes/time/beats').text = '1'
    second = ET.SubElement(across.find('part'), 'measure', number='2')
    for note in measure.findall('note')[2:]:
        measure.remove(note)
        second.append(note)
    variants['across-measures'] = across

    chain = copy.deepcopy(original)
    measure = chain.find('part/measure')
    measure.find('attributes/time/beats').text = '3'
    for note in list(measure.findall('note')[2:]):
        third = copy.deepcopy(note)
        ET.SubElement(note, 'tie', type='start')
        ET.SubElement(note.find('notations'), 'tied', type='start')
        measure.append(third)
    variants['chain'] = chain

    conventional = copy.deepcopy(original)
    attrs = conventional.find('part/measure/attributes')
    attrs.find('clef/sign').text = 'G'
    ET.SubElement(attrs.find('clef'), 'line').text = '2'
    attrs.remove(attrs.find('staff-details'))
    for note in conventional.findall('.//note'):
        note.find('notations').remove(note.find('notations/technical'))
    variants['conventional'] = conventional

    # Same pitch on two strings must still preserve each string's identity.
    unison = copy.deepcopy(original)
    for note in unison.findall('.//note'):
        if note.find('notations/technical/string').text == '1':
            pitch = note.find('pitch')
            pitch.find('step').text = 'B'
            pitch.remove(pitch.find('alter'))
            pitch.find('octave').text = '2'
            note.find('notations/technical/fret').text = '0'
    variants['different-strings-unison'] = unison

    different_voice = copy.deepcopy(original)
    for note in different_voice.findall('.//note')[2:]:
        note.find('voice').text = '2'
    variants['different-voice'] = different_voice

    results = []
    for name, source in variants.items():
        mei = render(binary, source, prefix + '-ties-' + name)
        notes = {n.get(XML_ID): n for n in mei.iter(MEI + 'note')}
        ties = list(mei.iter(MEI + 'tie'))
        expected = 4 if name == 'chain' else 2
        errors = []
        if len(ties) != expected:
            errors.append(f'{len(ties)} ties instead of {expected}')
        for tie in ties:
            start = notes[tie.get('startid', '')[1:]]
            end = notes.get(tie.get('endid', '')[1:])
            if end is None:
                errors.append('tie has no endpoint')
                continue
            keys = ['tab.course', 'tab.fret'] if name != 'conventional' else ['pname', 'oct']
            if any(start.get(k) != end.get(k) for k in keys):
                errors.append(f'{[(k, start.get(k), end.get(k)) for k in keys]}')
        results.append((name, not errors, '; '.join(errors)))
    return results


def check_tuplets(binary, prefix):
    original = ET.parse(ROOT / 'tab-tuplet.musicxml').getroot()
    results = []
    for lines in [4, 5, 6, 7]:
        for bracket in ['no', 'yes']:
            for placement in ['default', 'above', 'below']:
                root = copy.deepcopy(original)
                root.find('.//staff-lines').text = str(lines)
                tuplet = root.findall('.//tuplet')[2]
                tuplet.set('bracket', bracket)
                if placement != 'default':
                    tuplet.set('placement', placement)
                name = f'{lines}-lines-{bracket}-{placement}'
                svg = render(binary, root, prefix + '-tuplet-' + name, 'svg')
                staffs = [g for g in svg.iter(SVG + 'g') if g.get('class') == 'staff']
                staff = staffs[1]
                ys = []
                for path in staff.findall(SVG + 'path'):
                    match = re.fullmatch(r'M\S+ (\S+) L\S+ (\S+)', path.get('d', ''))
                    if match and match[1] == match[2]:
                        ys.append(float(match[1]))
                assert len(set(ys)) == lines
                number = next(g for g in staff.iter(SVG + 'g')
                              if g.get('class') == 'tupletNum bounding-box')
                rect = number.find(SVG + 'rect')
                top = float(rect.get('y'))
                bottom = top + float(rect.get('height'))
                outside = bottom < min(ys) if placement == 'above' else top > max(ys)
                results.append((name, outside,
                                f'number=[{top}, {bottom}], staff=[{min(ys)}, {max(ys)}]'))
    return results


def check_beamed_and_conventional(binary, prefix):
    original = ET.parse(ROOT / 'tab-tuplet.musicxml').getroot()
    results = []
    for placement in ['above', 'below']:
        root = copy.deepcopy(original)
        tuplet = root.findall('.//tuplet')[2]
        tuplet.set('bracket', 'yes')
        tuplet.set('placement', placement)
        for note, beam in zip(root.findall('.//note')[3:], ['begin', 'continue', 'end']):
            note.find('stem').text = 'up' if placement == 'above' else 'down'
            ET.SubElement(note, 'beam', number='1').text = beam
        name = 'beamed-' + placement
        svg = render(binary, root, prefix + '-' + name, 'svg')
        staff = [g for g in svg.iter(SVG + 'g') if g.get('class') == 'staff'][1]
        ys = []
        for path in staff.findall(SVG + 'path'):
            match = re.fullmatch(r'M\S+ (\S+) L\S+ (\S+)', path.get('d', ''))
            if match and match[1] == match[2]:
                ys.append(float(match[1]))
        rect = next(g for g in staff.iter(SVG + 'g')
                    if g.get('class') == 'tupletNum bounding-box').find(SVG + 'rect')
        top = float(rect.get('y'))
        bottom = top + float(rect.get('height'))
        outside = bottom < min(ys) if placement == 'above' else top > max(ys)
        results.append((name, outside, f'number=[{top}, {bottom}], staff=[{min(ys)}, {max(ys)}]'))

    # A conventional five-line score exercises both the beam and unbeamed layout paths.
    for bracket in ['yes', 'no']:
        root = copy.deepcopy(original)
        attrs = root.find('part/measure/attributes')
        attrs.findall('clef')[1].find('sign').text = 'G'
        ET.SubElement(attrs.findall('clef')[1], 'line').text = '2'
        attrs.remove(attrs.find('staff-details'))
        for note in root.findall('.//note')[3:]:
            note.find('notations').remove(note.find('notations/technical'))
            note.find('stem').text = 'down'
        root.findall('.//tuplet')[2].set('bracket', bracket)
        name = 'conventional-' + bracket
        svg = render(binary, root, prefix + '-' + name, 'svg')
        if prefix == 'before':
            results.append((name, True, 'baseline recorded'))
        elif (ROOT / ('before-' + name + '.svg')).exists():
            before = ET.parse(ROOT / ('before-' + name + '.svg')).getroot()
            # The toolkit version text changes; SVG engraving must otherwise remain identical.
            for tree in [before, svg]:
                for desc in list(tree.findall(SVG + 'desc')):
                    tree.remove(desc)
            results.append((name, ET.tostring(before) == ET.tostring(svg), 'compared with baseline'))
        else:
            results.append((name, True, 'rendered; use --baseline to compare engraving'))
    return results


def check_tie_geometry(binary, prefix):
    original = ET.parse(ROOT / 'tab-chord-tie-geometry.musicxml').getroot()
    results = []
    for size in [100, 150, 200]:
        for placement in ['above', 'below']:
            for multiple_digits in [False, True]:
                root = copy.deepcopy(original)
                root.find('.//staff-size').text = str(size)
                for note in root.findall('.//note'):
                    if note.findtext('staff') != '2':
                        continue
                    if multiple_digits:
                        fret = note.find('notations/technical/fret')
                        fret.text = str(int(fret.text) + 12)
                        octave = note.find('pitch/octave')
                        octave.text = str(int(octave.text) + 1)
                    for tie in note.findall('notations/tied'):
                        tie.set('orientation', 'over' if placement == 'above' else 'under')
                name = f'geometry-{size}-{placement}-digits-{int(multiple_digits) + 1}'
                mei = render(binary, root, prefix + '-' + name)
                svg = render(binary, root, prefix + '-' + name, 'svg')
                staff = [g for g in svg.iter(SVG + 'g') if g.get('class') == 'staff'][1]
                notes = {n.get(XML_ID): n for n in mei.iter(MEI + 'note')}
                graphics = {g.get('id'): g for g in svg.iter(SVG + 'g') if g.get('class') == 'note'}
                tie_graphics = {g.get('id'): g for g in svg.iter(SVG + 'g') if g.get('class') == 'tie'}
                tab_rects = [g.find(SVG + 'rect').attrib for g in staff.iter(SVG + 'g')
                             if g.get('class') == 'note bounding-box']
                errors = []
                count = 0
                for tie in mei.iter(MEI + 'tie'):
                    start = notes[tie.get('startid')[1:]]
                    end = notes[tie.get('endid')[1:]]
                    if not start.get('tab.course'):
                        continue
                    count += 1
                    if any(start.get(attr) != end.get(attr) for attr in ['tab.course', 'tab.fret']):
                        errors.append('tie connects different strings or frets')
                    start_graphic = graphics[start.get(XML_ID)]
                    end_graphic = graphics[end.get(XML_ID)]
                    start_x = float(start_graphic.find(SVG + 'text').get('x'))
                    end_x = float(end_graphic.find(SVG + 'text').get('x'))
                    path = tie_graphics[tie.get(XML_ID)].find(SVG + 'path')
                    values = list(map(float, re.findall(r'-?\d+(?:\.\d+)?', path.get('d'))))
                    assert len(values) == 14
                    if not start_x <= values[0] < values[6] <= end_x:
                        errors.append('endpoints extend past fret centers or run backwards')
                    if abs((values[0] + values[6]) / 2 - (start_x + end_x) / 2) > 1:
                        errors.append('curve is shifted away from the matching fret digits')
                    if float(path.get('stroke-width')) != int(9 * size / 100):
                        errors.append('stroke scales with string spacing instead of notation size')
                    y_sign = -1 if placement == 'above' else 1
                    if any(y_sign * (values[i] - values[1]) <= 0 for i in [3, 5]):
                        errors.append('curve ignores the encoded orientation')
                    # Check both sides of the filled Bezier against every fret, including adjacent courses.
                    for points in [values[:8], values[6:]]:
                        for step in range(101):
                            t = step / 100
                            weights = [(1-t)**3, 3*(1-t)**2*t, 3*(1-t)*t**2, t**3]
                            x, y = (sum(weight * points[2*i+axis] for i, weight in enumerate(weights))
                                    for axis in [0, 1])
                            margin = float(path.get('stroke-width')) / 2
                            for rect in tab_rects:
                                left, top = float(rect['x']), float(rect['y'])
                                if (left-margin < x < left+float(rect['width'])+margin
                                        and top-margin < y < top+float(rect['height'])+margin):
                                    errors.append('curve overlaps a fret glyph')
                                    break
                if count != 5:
                    errors.append(f'{count} TAB ties instead of 5')
                results.append((name, not errors, '; '.join(sorted(set(errors)))))
    return results


def check_split_and_single_ties(binary, prefix):
    original = ET.parse(ROOT / 'tab-chord-ties.musicxml').getroot()
    results = []
    for split in [False, True]:
        for placement in ['above', 'below']:
            root = copy.deepcopy(original)
            measure = root.find('part/measure')
            details = measure.find('attributes/staff-details')
            ET.SubElement(details, 'staff-size').text = '150'
            measure.find('attributes/time/beats').text = '1'
            for note in list(measure.findall('note')):
                if note.findtext('notations/technical/string') == '1':
                    measure.remove(note)
                else:
                    for tie in note.findall('notations/tied'):
                        tie.set('orientation', 'over' if placement == 'above' else 'under')
            if split:
                second = ET.SubElement(root.find('part'), 'measure', number='2')
                ET.SubElement(second, 'print', {'new-system': 'yes'})
                note = measure.findall('note')[1]
                measure.remove(note)
                second.append(note)
            else:
                measure.find('attributes/divisions').text = '2'
                for note in measure.findall('note'):
                    note.find('type').text = 'eighth'
            name = f'{"split" if split else "single"}-{placement}'
            svg = render(binary, root, prefix + '-' + name, 'svg', breaks='encoded' if split else 'none')
            systems = [g for g in svg.iter(SVG + 'g') if g.get('class') == 'system']
            notes = [g for g in svg.iter(SVG + 'g') if g.get('class') == 'note']
            ties = [g for g in svg.iter(SVG + 'g') if 'tie' in g.get('class', '').split()
                    and 'bounding-box' not in g.get('class', '').split()]
            errors = []
            if len(systems) != (2 if split else 1) or len(ties) != (2 if split else 1):
                errors.append('missing system or tie segment')
            start_x, end_x = [float(note.find(SVG + 'text').get('x')) for note in notes]
            first = list(map(float, re.findall(r'-?\d+(?:\.\d+)?', ties[0].find(SVG + 'path').get('d'))))
            last = list(map(float, re.findall(r'-?\d+(?:\.\d+)?', ties[-1].find(SVG + 'path').get('d'))))
            start_rect = notes[0].find(SVG + 'g/' + SVG + 'rect').attrib
            end_rect = notes[-1].find(SVG + 'g/' + SVG + 'rect').attrib
            if not start_x <= first[0] <= float(start_rect['x']) + float(start_rect['width']) + 40:
                errors.append('start anchor is offset from the fret')
            if not float(end_rect['x']) - 40 <= last[6] <= end_x:
                errors.append('end anchor is offset from the fret')
            if not split and abs((first[0] + last[6]) / 2 - (start_x + end_x) / 2) > 1:
                errors.append('single-note tie is shifted')
            results.append((name, not errors, '; '.join(errors)))
    return results


def check_conventional_ties(binary, baseline):
    root = ET.parse(ROOT / 'tab-chord-tie-geometry.musicxml').getroot()
    measure = root.find('part/measure')
    for child in list(measure):
        if child.tag == 'backup' or (child.tag == 'note' and child.findtext('staff') == '2'):
            measure.remove(child)
    attributes = measure.find('attributes')
    attributes.find('staves').text = '1'
    for child in list(attributes):
        if child.tag == 'staff-details' or child.get('number') == '2':
            attributes.remove(child)
    svg = render(binary, root, 'after-conventional-ties', 'svg')
    if not baseline:
        return [('conventional-ties', True, 'rendered; use --baseline to compare engraving')]
    before = render(baseline, root, 'before-conventional-ties', 'svg')
    for tree in [before, svg]:
        for desc in tree.findall(SVG + 'desc'):
            tree.remove(desc)
    return [('conventional-ties', ET.tostring(before) == ET.tostring(svg), 'compared with baseline')]


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Check imported TAB ties, rendered tie geometry, and tuplets.')
    parser.add_argument('binary', type=Path, help='Native Verovio executable')
    parser.add_argument('--baseline', type=Path, help='Compare conventional engraving with this executable')
    parser.add_argument('--output', type=Path, help='Keep generated inputs, MEI, and SVG in this directory')
    args = parser.parse_args()
    with tempfile.TemporaryDirectory(prefix='verovio-tablature-') as temporary:
        ROOT = args.output.resolve() if args.output else Path(temporary)
        ROOT.mkdir(parents=True, exist_ok=True)
        for fixture in FIXTURES.glob('*.musicxml'):
            shutil.copy(fixture, ROOT / fixture.name)
        if args.baseline:
            check_beamed_and_conventional(args.baseline.resolve(), 'before')
        results = []
        for group, check in [('ties', check_ties), ('tuplet', check_tuplets),
                             ('layout', check_beamed_and_conventional), ('geometry', check_tie_geometry),
                             ('segments', check_split_and_single_ties)]:
            results += [(group + '/' + name, ok, detail)
                        for name, ok, detail in check(args.binary.resolve(), 'after')]
        results += check_conventional_ties(args.binary.resolve(), args.baseline.resolve() if args.baseline else None)
        for name, ok, detail in results:
            print(('PASS' if ok else 'FAIL') + ' ' + name + ': ' + detail)
        failures = sum(not ok for _, ok, _ in results)
        print(f'{len(results) - failures}/{len(results)} passed')
        raise SystemExit(bool(failures))
