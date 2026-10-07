#ifndef VEROVIO_HB_CONFIG_H
#define VEROVIO_HB_CONFIG_H

// HB_LEAN disables these by default. Verovio needs all three while retaining
// the remainder of the lean/mini feature exclusions.
#undef HB_NO_DRAW
#undef HB_NO_METRICS
#undef HB_NO_NAME

// Keep unsupported OpenType facilities out even if HarfBuzz build defaults
// change in a later audited update.
#define HB_NO_AAT
#define HB_NO_BITMAP
#define HB_NO_COLOR
#define HB_NO_FACE_BUILDER
#define HB_NO_MATH
#define HB_NO_META
#define HB_NO_PAINT
#define HB_NO_STYLE
#define HB_NO_VAR

#endif
