from __future__ import annotations

from pathlib import Path
import os
import tempfile

def atomic_write_text(path: Path, content: str, encoding: str = "utf-8") -> None:
    """
    Write atomically to avoid partially-written headers if interrupted.
    """
    path.parent.mkdir(parents=True, exist_ok=True)
    fd, tmp = tempfile.mkstemp(prefix=path.name + ".", dir=str(path.parent))
    try:
        with os.fdopen(fd, "w", encoding=encoding, newline="\n") as f:
            f.write(content)
        os.replace(tmp, path)
    except Exception:
        try:
            os.remove(tmp)
        except OSError:
            pass
        raise
