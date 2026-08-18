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

func TestElementsAtTimeUsesRenderedPage(t *testing.T) {
	tk := NewToolkitWithResourcePath("../../data")
	defer tk.Close()
	tk.SetOptions(`{"breaks":"encoded"}`)
	tk.LoadData(`<mei xmlns="http://www.music-encoding.org/ns/mei" meiversion="5.0"><music><body><mdiv><score><scoreDef><staffGrp><staffDef n="1" lines="5" clef.shape="G" clef.line="2" meter.count="4" meter.unit="4"/></staffGrp></scoreDef><section><measure n="1"><staff n="1"><layer n="1"><note dur="4" oct="4" pname="c"/><note dur="4" oct="4" pname="d"/><note dur="4" oct="4" pname="e"/><note dur="4" oct="4" pname="f"/></layer></staff></measure><measure n="2"><staff n="1"><layer n="1"><note dur="4" oct="4" pname="g"/><note dur="4" oct="4" pname="a"/><note dur="4" oct="4" pname="b"/><note dur="4" oct="5" pname="c"/></layer></staff></measure><pb/><measure n="3"><staff n="1"><layer n="1"><note dur="4" oct="5" pname="c"/><note dur="4" oct="4" pname="b"/><note dur="4" oct="4" pname="a"/><note dur="4" oct="4" pname="g"/></layer></staff></measure></section></score></mdiv></body></music></mei>`)
	if got := tk.ElementsAtTime(4500); !strings.Contains(got, `"page":2`) {
		t.Fatalf("expected page 2, got %s", got)
	}
}
