from __future__ import annotations

import argparse
from pathlib import Path

from .gen import HeaderMeta, Provenance, git_commit_sha, build_time_iso, render_header
from .io import atomic_write_text


def build_parser() -> argparse.ArgumentParser:
    p = argparse.ArgumentParser(
        prog="preprocess",
        description="Generate a C/C++ header for HLS.",
    )
    p.add_argument("--name", required=True, help="Base name for include guard (e.g., trig_tables).")
    p.add_argument("--out", required=True, type=Path, help="Output header path, e.g., src/common/generated/trig_tables.h")
    p.add_argument("--description", default="<>", help="Header description line.")
    p.add_argument("--author", default="<>", help="Author field.")
    p.add_argument("--version", default="1.0", help="Version field.")
    p.add_argument("--copyright", default="<>")
    p.add_argument("--license", default="<>")
    return p


def main() -> int:
    args = build_parser().parse_args()

    # Call your generation function here
    # ...

    meta = HeaderMeta(
        filename=args.out.name,
        description=args.description,
        author=args.author,
        version=args.version,
        copyright=args.copyright,
        license=args.license,
    )

    prov = Provenance(
            generator_id="preproc.generator.render_header",
            source=f"{__file__}:main",
            git_sha=git_commit_sha(),
            build_time_iso=build_time_iso(),
            params=(
                ("name", str(args.name)),
                # Put here other params as needed
                #  ...
                ("out", str(args.out)),
                ("version", str(args.version)),
        ),
    )

    hdr = render_header(
        name=args.name,
        provenance=prov,
        meta=meta,
        # Put here other content as needed
        # ...
    )

    atomic_write_text(args.out, hdr)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

