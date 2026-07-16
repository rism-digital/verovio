// Debug harness to compare getPitchPosition against SVG coordinates for a MusicXML file.
// Usage: node emscripten/npm/test/get-pitch-position-xml.mjs
// Optional env:
//   NOTE_INDEX=0            (which timemap note to inspect)
//   NOTE_ID=abcd123         (override note id)
//   WRITE_SVG=1             (write test.svg next to this script)

import fs from 'fs';
import createVerovioModule from '../dist/verovio-module.mjs';
import { VerovioToolkit } from '../dist/verovio.mjs';

const TEST_XML_URL = new URL('./test.xml', import.meta.url);
const NOTE_INDEX = Number.parseInt(process.env.NOTE_INDEX ?? '0', 10);
const NOTE_ID_OVERRIDE = process.env.NOTE_ID ?? null;
const WRITE_SVG = process.env.WRITE_SVG === '1';

const readFile = (url) => fs.readFileSync(url, 'utf8');

const parseDefinitionScale = (svg) => {
  const viewBoxMatch = svg.match(
    /<svg class="definition-scale"[^>]*viewBox="[^"]+ [^"]+ ([^"]+)"/,
  );
  const marginMatch = svg.match(
    /<g class="page-margin"[^>]*transform="translate\((-?\d+(?:\.\d+)?),\s*(-?\d+(?:\.\d+)?)\)"/,
  );
  const viewBoxHeight = viewBoxMatch
    ? Number.parseFloat(viewBoxMatch[1])
    : null;
  const marginX = marginMatch ? Number.parseFloat(marginMatch[1]) : 0;
  const marginY = marginMatch ? Number.parseFloat(marginMatch[2]) : 0;
  return { viewBoxHeight, marginX, marginY };
};

const findSnippet = (svg, id, size = 4000) => {
  const idx = svg.indexOf(`id="${id}"`);
  if (idx === -1) return null;
  return svg.slice(Math.max(0, idx - 200), idx + size);
};

const findFirstTranslate = (chunk) => {
  if (!chunk) return null;
  const match = chunk.match(
    /translate\((-?\d+(?:\.\d+)?),\s*(-?\d+(?:\.\d+)?)\)/,
  );
  if (!match) return null;
  return {
    x: Number.parseFloat(match[1]),
    y: Number.parseFloat(match[2]),
  };
};

const findNoteheadTranslate = (chunk) => {
  if (!chunk) return null;
  const match = chunk.match(
    /class="notehead"[\s\S]*?translate\((-?\d+(?:\.\d+)?),\s*(-?\d+(?:\.\d+)?)\)/,
  );
  if (!match) return null;
  return {
    x: Number.parseFloat(match[1]),
    y: Number.parseFloat(match[2]),
  };
};

const main = async () => {
  const xml = readFile(TEST_XML_URL);
  const module = await createVerovioModule();
  const tk = new VerovioToolkit(module);

  tk.setOptions({
    adjustPageHeight: false,
    adjustPageWidth: false,
    pageHeight: 1871,
    pageWidth: 1323,
    scale: 40,
    svgViewBox: true,
    svgBoundingBoxes: true,
    xmlIdSeed: 1,
  });

  tk.loadData(xml);
  const svg = tk.renderToSVG(1);
  const timemap = tk.renderToTimemap({ includeMeasures: true, includeRests: true });

  if (WRITE_SVG) {
    fs.writeFileSync(new URL('./test.svg', import.meta.url), svg);
  }

  const noteEntries = timemap.filter((entry) => Array.isArray(entry.on) && entry.on.length);
  if (!noteEntries.length) {
    throw new Error('No note entries found in timemap.');
  }

  const selectedEntry =
    NOTE_ID_OVERRIDE
      ? noteEntries.find((entry) => entry.on.includes(NOTE_ID_OVERRIDE))
      : noteEntries[Math.min(Math.max(NOTE_INDEX, 0), noteEntries.length - 1)];

  if (!selectedEntry) {
    throw new Error('Unable to select a note entry from timemap.');
  }

  const noteId = NOTE_ID_OVERRIDE ?? selectedEntry.on[0];
  const qstamp = selectedEntry.qstamp;
  const midiValues = tk.getMIDIValuesForElement(noteId);
  const midi = midiValues?.pitch ?? null;

  if (midi === null) {
    throw new Error(`Unable to resolve MIDI pitch for note ${noteId}.`);
  }

  const pitchPosition = tk.getPitchPosition(qstamp, midi, 1);
  const { viewBoxHeight, marginX, marginY } = parseDefinitionScale(svg);

  const noteChunk = findSnippet(svg, noteId, 2500);
  const noteTranslate =
    findNoteheadTranslate(noteChunk) ?? findFirstTranslate(noteChunk);
  const noteAbs = noteTranslate
    ? { x: marginX + noteTranslate.x, y: marginY + noteTranslate.y }
    : null;

  const contentHeight = viewBoxHeight !== null ? viewBoxHeight - marginY : null;
  const pitchAbs = { x: pitchPosition.x, y: pitchPosition.y };

  console.log('Timemap notes (first 8):');
  console.log(
    noteEntries.slice(0, 8).map((entry) => ({
      qstamp: entry.qstamp,
      noteId: entry.on[0],
      midi: tk.getMIDIValuesForElement(entry.on[0])?.pitch,
    })),
  );
  console.log('');
  console.log('Selected note:');
  console.log({ noteId, qstamp, midi });
  console.log('');
  console.log('Pitch position:');
  console.log(pitchPosition);
  console.log('');
  console.log('SVG context:');
  console.log({ viewBoxHeight, marginX, marginY, contentHeight });
  console.log('');
  console.log('Notehead translate (absolute):');
  console.log(noteAbs);
  console.log('');
  console.log('Pitch position (svg coords):');
  console.log(pitchAbs);
  console.log('');
  if (noteAbs) {
    console.log('Delta (pitchAbs - noteAbs):');
    console.log({
      dx: Math.round(pitchAbs.x - noteAbs.x),
      dy: Math.round(pitchAbs.y - noteAbs.y),
    });
    console.log('');
  }
};

main().catch((err) => {
  console.error(err);
  process.exit(1);
});
