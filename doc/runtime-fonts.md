# Runtime fonts

Verovio now reads static OpenType fonts while a Toolkit is running. Bravura is
the bundled music default and fallback; the regular, italic, bold, and bold
italic Tinos faces form the bundled text default. SVG output represents both
music and text with deduplicated paths and `use` elements, so it does not
depend on fonts installed in the browser or viewer.

## Registering fonts

Native callers can register a file or a byte buffer. String-only bindings can
use the base64 methods. JavaScript accepts a `Uint8Array` or `ArrayBuffer`,
Python accepts `bytes`, Go accepts `[]byte`, and Swift accepts `Data`.
Successful registration returns the canonical family name; failure returns an
empty string.

Every registration form also has an alias-aware overload. The alias is the
exact, case-sensitive family name used by MEI and does not rename or copy the
font. For example, this registers the OpenType family `Quicksand` for an MEI
file that uses `<rend fontname="QS">`:

```js
const response = await fetch("/fonts/Quicksand-Regular.woff2");
const bytes = new Uint8Array(await response.arrayBuffer());
const canonicalFamily = toolkit.registerTextFont(bytes, "QS");
// canonicalFamily === "Quicksand"
```

Browsers cannot expose arbitrary installed-font bytes to WebAssembly, so web
applications must fetch or otherwise provide the font data explicitly. The
same final alias argument is available for music, base64, and native-file
registration. Python and Java use the corresponding overloads, Swift uses an
`alias:` argument, and Go provides `RegisterTextFontWithAlias` and matching
music/base64/file methods.

The command-line equivalents are repeatable:

```sh
verovio --font-add-text MyText.woff2 \
  --font-add-music MyMusic.otf input.mei
```

Use the explicit alias options when the MEI family differs from the font's
canonical OpenType or SMuFL name:

```sh
verovio --font-add-text-as "QS=/fonts/Quicksand-Regular.woff2" \
  --font-add-music-as "HouseMusic=/fonts/MyMusic.otf" input.mei
```

Both options are repeatable and are also available as the `fontAddTextAs` and
`fontAddMusicAs` string-array Toolkit options. Values split on the first `=`,
so the file path may itself contain `=`. Aliases may not contain `=` or consist
only of whitespace.

A music font requires SMuFL metadata. Put `MyMusic.json`,
`MyMusic_metadata.json`, or `metadata.json` beside the font, or install the
metadata in a standard SMuFL `Fonts/<family>` location.

Use `font` for the default music family, `fontFallback` for the music fallback,
and `fontText` for the default text family. MEI `fontname` is resolved per
element and per text run; for example, a nested
`<rend fontname="My Text">` affects only that run. Text faces are selected by
family, weight, and style. Registering a new face after loading a document
invalidates layout; the next rendering call lays out the document again. An
alias is family-wide, so one alias covers all registered weight/style faces.
Adding an alias after document load likewise invalidates layout.

The same alias may exist independently in the music and text namespaces.
Re-registering identical bytes and aliases is idempotent, and multiple aliases
may point to the same canonical family. Registration fails without changing
the registry if an alias is already claimed by another family or collides with
another canonical family in the same namespace.

## Supported formats and limits

Static OTF, TTF, WOFF1, and WOFF2 files are supported. Font collections,
variable fonts, color or bitmap fonts, AAT, and bidirectional layout are not.
Text is shaped with LTR semantics; an RTL script produces a warning. Inputs
over 32 MiB and decoded fonts over 64 MiB are rejected, as are malformed table
directories and compressed data that exceed the decode limits.

Missing music glyphs fall back to Bravura. Missing text clusters fall back to
the matching Tinos face and then to `.notdef`.

## WebAssembly resources

Production WebAssembly builds consist of the JavaScript module plus an
adjacent `verovio.data` file containing the bundled runtime resources. Both
files are required. The default loader resolves the data file beside the
module in Node and beside the script URL in browsers. Modularized callers can
override this with Emscripten's `locateFile` option when an asset pipeline
publishes `verovio.data` under a different URL.

The data sidecar is intentional: putting already-compressed WOFF2 bytes inside
Emscripten's single-file JavaScript encoding increased the compressed release
by substantially more than the fonts themselves. Release size reports count
both files.

## Migration

`fontAddCustom` remains as a deprecated ZIP adapter for one compatibility
release. `fontTextLiberation` and `smuflTextFont` are also deprecated and emit
warnings. Applications should register any non-bundled family explicitly and
select it with `font`, `fontText`, or MEI `fontname`.

Leipzig, Gootville, Leland, Petaluma, Liberation, and Times metric resources
are no longer bundled. They remain usable when supplied as runtime fonts.
