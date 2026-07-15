package verovio

import (
	"os"
	"strings"
	"testing"
)

func TestVersion(t *testing.T) {
	tk := NewToolkitWithoutResourcePath()
	defer tk.Close()

	version := tk.Version()
	if version == "" {
		t.Fatal("expected a non-empty version string")
	}
}

func TestNewToolkitUsesResourcePathEnv(t *testing.T) {
	t.Setenv("VEROVIO_RESOURCE_PATH", "../../data")

	tk := NewToolkit()
	defer tk.Close()

	if got := tk.ResourcePath(); got != "../../data" {
		t.Fatalf("expected resource path from env, got %q", got)
	}
}

func TestNewToolkitWithoutEnvDoesNotAssumeDataPath(t *testing.T) {
	original, hadOriginal := os.LookupEnv("VEROVIO_RESOURCE_PATH")
	if hadOriginal {
		defer os.Setenv("VEROVIO_RESOURCE_PATH", original)
	} else {
		defer os.Unsetenv("VEROVIO_RESOURCE_PATH")
	}
	os.Unsetenv("VEROVIO_RESOURCE_PATH")

	tk := NewToolkit()
	defer tk.Close()

	if got := tk.ResourcePath(); got == "/data" {
		t.Fatalf("expected NewToolkit to avoid default /data resource path, got %q", got)
	}
}

func TestLoadDataAndRenderSVG(t *testing.T) {
	tk := NewToolkitWithResourcePath("../../data")
	defer tk.Close()

	ok := tk.LoadData(`<?xml version="1.0" encoding="UTF-8"?>
<mei meiversion="4.0.1" xmlns="http://www.music-encoding.org/ns/mei">
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
                  <note oct="4" pname="c" dur="4"/>
                </layer>
              </staff>
            </measure>
          </section>
        </score>
      </mdiv>
    </body>
  </music>
</mei>`)
	if !ok {
		t.Fatal("expected sample MEI to load")
	}

	svg := tk.RenderToSVG(1, false)
	if !strings.Contains(svg, "<svg") {
		t.Fatal("expected SVG output")
	}
}

func TestRegisterFontsWithAliases(t *testing.T) {
	tk := NewToolkitWithResourcePath("../../data")
	defer tk.Close()

	textFont, err := os.ReadFile("../../tests/font-runtime/VerovioTestLigature.ttf")
	if err != nil {
		t.Fatal(err)
	}
	if got := tk.RegisterTextFontWithAlias(textFont, "QS"); got != "Verovio Test Ligature" {
		t.Fatalf("expected canonical text family, got %q", got)
	}

	musicFont, err := os.ReadFile("../../tests/font-runtime/VerovioTestMusic.ttf")
	if err != nil {
		t.Fatal(err)
	}
	metadata, err := os.ReadFile("../../tests/font-runtime/VerovioTestMusic_metadata.json")
	if err != nil {
		t.Fatal(err)
	}
	if got := tk.RegisterMusicFontWithAlias(musicFont, string(metadata), "VM"); got != "Verovio Test Music" {
		t.Fatalf("expected canonical music family, got %q", got)
	}

	if !tk.LoadData(`<?xml version="1.0" encoding="UTF-8"?>
<mei xmlns="http://www.music-encoding.org/ns/mei" meiversion="5.1"><music><body><mdiv><score>
<scoreDef><staffGrp><staffDef n="1" lines="5" clef.shape="G" clef.line="2"/></staffGrp></scoreDef>
<section><measure n="1"><staff n="1"><layer n="1"><note xml:id="n1" pname="c" oct="4" dur="4"/>
</layer></staff><dir startid="#n1"><rend fontname="QS">ffi</rend></dir></measure></section>
</score></mdiv></body></music></mei>`) {
		t.Fatal("expected aliased-font MEI to load")
	}
	if svg := tk.RenderToSVG(1, false); !strings.Contains(svg, "text-8BFEB250B0FDDA0E-4-") {
		t.Fatal("expected QS to render with the registered ligature face")
	}
}
