import os
from pathlib import Path

root_dir =Path("C:\\Users\\7862s\\Desktop\\sem5\\CSE311 PARALLEL\\CSE311\\")

# Traverse and filter out .git directories
for path in root_dir.rglob('*'):
    if '.git' not in path.parts:
        if path.name.endswith('.exe'):
            path.unlink()
        # continue