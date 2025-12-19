// Quick harness to exercise GetPitchPosition in the Verovio toolkit.
// Usage: run after building the npm package (./buildNpmPackage) from the repo root:
//   node emscripten/npm/test/get-pitch-position.mjs
// It loads a tiny MEI with a single measure and asserts the returned measure/page/system.

import createVerovioModule from '../dist/verovio-module.mjs';
import { VerovioToolkit } from '../dist/verovio.mjs';

const MEI = `<?xml version="1.0" encoding="UTF-8"?>
<mei xmlns="http://www.music-encoding.org/ns/mei" meiversion="4.0.0">
  <music>
    <body>
      <mdiv>
        <score>
          <scoreDef meter.count="4" meter.unit="4" key.sig="0">
            <staffGrp>
              <staffDef n="1" clef.shape="G" clef.line="2"/>
            </staffGrp>
          </scoreDef>
          <section>
            <measure xml:id="m1">
              <staff n="1">
                <layer>
                  <note xml:id="n1" pname="c" oct="4" dur="4"/>
                </layer>
              </staff>
            </measure>
          </section>
        </score>
      </mdiv>
    </body>
  </music>
</mei>`;

const assertEqual = (actual, expected, msg) => {
  if (actual !== expected) {
    throw new Error(`${msg}: expected ${expected}, got ${actual}`);
  }
};

const main = async () => {
  const module = await createVerovioModule();
  const tk = new VerovioToolkit(module);
  tk.setOptions({
    adjustPageHeight: false,
    adjustPageWidth: false,
    pageHeight: 1500,
    pageWidth: 1000,
    scale: 50,
  });

  tk.loadData(MEI);
  // Force layout so drawing coords are available
  tk.renderToSVG(1);
  tk.renderToTimemap();

  const result = tk.getPitchPosition(0, 60, 1);
  console.log('GetPitchPosition(0, 60, 1) ->', result);

  assertEqual(result.measureId, 'm1', 'measureId');
  assertEqual(result.page, 1, 'page');
  assertEqual(result.system, 1, 'system');
  assertEqual(result.staff, 1, 'staff');
};

main().catch((err) => {
  console.error(err);
  process.exit(1);
});
