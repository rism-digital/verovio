verovio-test
============

Test runner for verovio npm package.

# Usage

- In `emscripten/npm`, do: `npm build && npm link`
- In `emscripten/npm/test`, do:

```shell
npm install -g typescript
npm install && npm link verovio
npx tsc -p tsconfig.mts.json && node verovio-tests.mjs
npx tsc -p tsconfig.cts.json && node verovio-tests.cjs
```
