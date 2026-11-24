# Vendored JSON++ (jsonxx)

- Upstream: <https://github.com/hjiang/jsonxx>
- Commit: `1a3738d0062053b7fcdc1c0ea58b1675397f75ef` (2021-02-21)
- License: MIT, see `LICENSE`

`jsonxx.h`, `jsonxx.cc`, `LICENSE`, `README.md`, and `Makefile` are present.
The upstream test (`jsonxx_test.cc`) and CI configuration (`.travis.yml`,
`circle.yml`) are not included.

## Local modifications

- `jsonxx.h`, `jsonxx.cc`: `Value` has an additional `precision_` member that
  controls the precision of numbers in the JSON and XML output.
- `jsonxx.cc`: `\uXXXX` escape sequences in strings are decoded to UTF-8,
  including UTF-16 surrogate pairs.
