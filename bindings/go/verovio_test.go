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
