"""Minimal installed-wheel smoke test for the Python bindings.

This validates that the extension imports, packaged resource files are
discoverable, and a trivial render returns SVG on the active interpreter.
"""

import importlib.resources as resources

import verovio


def run_smoke_test() -> None:
    toolkit = verovio.toolkit()
    print(toolkit.getVersion())
    toolkit.setResourcePath(str(resources.files("verovio") / "data"))
    toolkit.loadData(
        """<?xml version="1.0" encoding="UTF-8"?>
<mei xmlns="http://www.music-encoding.org/ns/mei" meiversion="5.1">
  <meiHead>
    <fileDesc>
      <titleStmt>
        <title>Verovio Python smoke test</title>
      </titleStmt>
      <pubStmt/>
    </fileDesc>
  </meiHead>
  <music>
    <body>
      <mdiv>
        <score>
          <scoreDef meter.count="4" meter.unit="4">
            <staffGrp>
              <staffDef n="1" lines="5" clef.shape="G" clef.line="2"/>
            </staffGrp>
          </scoreDef>
          <section>
            <measure n="1">
              <staff n="1">
                <layer n="1">
                  <note pname="c" oct="4" dur="4"/>
                </layer>
              </staff>
            </measure>
          </section>
        </score>
      </mdiv>
    </body>
  </music>
</mei>
"""
    )
    svg = toolkit.renderToSVG(1)
    assert "<svg" in svg, "renderToSVG did not return SVG output"


def main() -> None:
    try:
        run_smoke_test()
    except Exception as exc:
        print(f"SMOKE TEST FAIL: {exc}")
        raise

    print("SMOKE TEST PASS")


if __name__ == "__main__":
    main()
