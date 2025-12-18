# <Thesis companion repository>

This repository contains the reproducibility assets, reference implementations, and evaluation scripts accompanying the thesis:

> **Title:** Methodology for the improvement of hardware acceleration techniques in real-time I&C applications for fusion devices
> **Author:** Cesar Gonzalez Brito  
> **Institution:** UPM / I2A2  
> **Year:** 2026

## Preprocess tools

This directory contains preprocessing utilities used to generate HLS-friendly headers (e.g., LUTs and constants) from configurable inputs.

Two equivalent implementations are provided:

- `cpp/`: C++ implementation (fast, easy to integrate with existing C/C++ parsing libraries).
- `python/`: Python implementation (no compilation required; convenient for quick iteration).

---

### Output

Both implementations generate a header containing:
- a metadata banner,
- an include guard derived from `--name`,

Typical output path:
- `src/common/generated/<name>.h`

---

### Python version

The repository template is runnable without installing the Python package. You can invoke the Python implementation directly from the repository root by pointing to the local sources:

```bash
python3 -m tools.preprocess.python.src.preprocess.cli \
  --name <name>\
  --out src/common/generated/<name>.h
```

#### (Optional) Install

From the repository root:

```bash
python3 -m pip install -e tools/preprocess/python
```

This installs a CLI entrypoint called preprocess.  

#### Run

```bash
preprocess \
  --name <name> \
  --out src/common/generated/<name>.h 
```

#### Optional parameters  

```bash
preprocess \
  --name <name> \
  --out src/common/generated/<name>.h \
  --description "" \
  --author "" \
  --copyright "" \
  --license ""
```

#### Help  

```bash
preprocess --help
```

### C++ version  

#### Configure and build

From the repository root:

```bash
cmake -S tools/preprocess/cpp -B build/preprocess-cpp
cmake --build build/preprocess-cpp -j
```

The executable will be:

build/preprocess-cpp/preprocess

#### Run 

```bash
build/preprocess-cpp/preprocess \
  --name trig_tables \
  --out src/common/generated/trig_tables.h 
```

#### Optional parameters

```bash
build/preprocess-cpp/preprocess \
  --name <name> \
  --out src/common/generated/<name>.h \
  --description "" \
  --author "" \
  --copyright "" \
  --license ""
```
#### Help

```bash
build/preprocess-cpp/preprocess --help
```

### Choosing Python vs C++

Use Python when:  

 - you want zero compilation steps,  

 - you iterate quickly on formats and small transformations,  

 - you run in environments where a compiler toolchain is not guaranteed.  

Use C++ when:  

 - you need to leverage existing C/C++ libraries (vendor parsers, legacy config readers, binary formats),  

 - performance of the preprocessing step matters,  

 - you want a single self-contained binary.

### Notes on reproducibility



### Roadmap

Planned extensions :  

 - config-driven generation (YAML/JSON) with a shared schema (spec/)

 - output manifest (hashes + generator version)

 - additional generators (LUTs from measured calibration data, quantized/fixed-point tables, etc.)

