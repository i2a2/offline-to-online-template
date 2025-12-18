from __future__ import annotations

from dataclasses import dataclass
import datetime
import math
import re
import subprocess
from typing import Iterable, List


@dataclass(frozen=True)
class HeaderMeta:
    filename: str
    description: str
    author: str
    version: str = "1.0"
    copyright: str = ""
    license: str = ""

@dataclass(frozen=True)
class Provenance:
    generator_id: str
    source: str
    git_sha: str
    build_time_iso: str
    params: Iterable[tuple[str, str]] = ()
    config_path: Optional[str] = None
    config_sha: Optional[str] = None
    inputs: Iterable[tuple[str, str], ...] = ()

def _run_cmd(cmd: List[str]) -> str:
    try:
        p = subprocess.run(
            cmd,
            check=False,
            capture_output=True,
            text=True,
        )
        out = (p.stdout or "").strip()
        return out if out else "unknown"
    except Exception:
        return "unknown"

def git_commit_sha() -> str:
    """Get the current git commit SHA, or 'unknown'."""
    return _run_cmd(["git", "rev-parse", "HEAD"])

def build_time_iso() -> str:
    """Get the current build time in ISO 8601 format."""
    return datetime.datetime.now(datetime.timezone.utc).isoformat()

def _format_provenance_block(prov: Provenance) -> str:
    lines = [
        "/**",
        " * @section PROVENANCE",
        " *",
        f" * Generator ID: {prov.generator_id}",
        f" * Source: {prov.source}",
        f" * Git commit: {prov.git_sha}",
        f" * Build Time (ISO 8601): {prov.build_time_iso}",
    ]

    if prov.params:
        lines.append(" * Parameters:")
        for k, v in prov.params:
            lines.append(f" *   - {k}: {v}")

    if prov.config_path or prov.config_sha:
        lines.append(" * Config :" )
        if prov.config_path:
            lines.append(f" *   - Path: {prov.config_path}")
        if prov.config_sha:
            lines.append(f" *   - Git SHA: {prov.config_sha}")

    if prov.inputs:
        lines.append(" * Inputs:")
        for path, sha in prov.inputs:
            lines.append(f" *   - {path}: {sha}")

    lines.append(" */")
    return "\n".join(lines)

def make_include_guard(stem_name: str) -> str:
    """
    Convert a stem name into an include guard:
      <FILE_NAME>_H
    """
    s = re.sub(r"[^0-9A-Za-z]+", "_", stem_name).strip("_").upper()
    if not s:
        raise ValueError("Invalid name for include guard.")
    return f"{s}_H"


# Insert other generation functions as needed
# ...


def render_header(
    name: str,
    meta: HeaderMeta,
    provenance: Provenance,
    # Other content parameters as needed
    # ...
) -> str:
    guard = make_include_guard(name)
    today = datetime.date.today().strftime("%Y-%m-%d")

    prov_block = (_format_provenance_block(provenance) + "\n\n") if provenance else ""

    # Call other content rendering functions as needed
    # ...

    # Keep the banner close to what you wrote, but make it syntactically clean.
    return f"""\
{prov_block}
/**
 *
 *  @file       {meta.filename:<40}
 *  @brief      {meta.description}
 *  @autor      {meta.author}
 *  @date       {today}
 *  @version    {meta.version}
 *
 *  @copyright  {meta.copyright}
 *  @license    {meta.license}
 */

#ifndef {guard}
#define {guard}

// Put other includes as needed
// ...

// Put other generated content as needed
// ...


#endif // {guard}
"""
