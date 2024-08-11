import sys
import argparse
from pathlib import Path
from typing import Sequence
import tempfile
import webbrowser

import pyodr.core as core


def main(args: Sequence[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description="OpenDocument Reader")
    parser.add_argument("file", type=Path, help="The file to open")
    args = parser.parse_args(args)

    html_config = core.HtmlConfig()

    print(f"Opening file: {str(args.file)}")
    file = core.open(str(args.file))
    if not file.is_valid():
        print(f"Unable to open")
        return 1
    print(f"Type detected as {file.file_type()}")

    tmp_dir = Path(tempfile.mkdtemp(prefix="pyodr-", suffix=".tmp"))
    print(f"Translating html to {str(tmp_dir)}")
    html = core.html.translate(file, str(tmp_dir), html_config)
    print("Done")

    for page in html.pages():
        url = f"file://{str(page.path)}"
        print(f"Opening {page.name} via {url}")
        webbrowser.open(url)

    return 0


if __name__ == '__main__':
    error = main()
    sys.exit(error)
