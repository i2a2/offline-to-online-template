from __future__ import annotations

from dataclasses import dataclass
from datetime import date
import math
import re
from typing import Iterable, List


@dataclass(frozen=True)
class HeaderMeta:
    filename: str
    description: str
    author: str
    version: str = "1.0"
    copyright: str = "<>"
    license: str = "<>"


def make_include_guard(stem_name: str) -> str:
    """
    Convert a stem name into an include guard:
      <FILE_NAME>_H
    """
    s = re.sub(r"[^0-9A-Za-z]+", "_", stem_name).strip("_").upper()
    if not s:
        raise ValueError("Invalid name for include guard.")
    return f"{s}_H"

def render_header(
    name: str,
    meta: HeaderMeta,
) -> str:
    guard = make_include_guard(name)
    today = date.today().strftime("%Y-%m-%d")

    # Keep the banner close to what you wrote, but make it syntactically clean.
    return f"""\
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

// Includes
//#include <hls_stream.h>
//#include <hls_vector.h>
//#include <hls_math.h>

#endif // {guard}
"""
