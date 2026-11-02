"""Check offline sample integrity and provenance without downloading assets."""
import hashlib
import json
import re
from pathlib import Path


def verify(folder):
    manifest = json.loads((folder / "manifest.json").read_text(encoding="utf-8"))
    if manifest.get("schema_version") != 1:
        raise ValueError("unsupported sample manifest version")
    names = set()
    for item in manifest["assets"]:
        name = item["path"]
        if name in names or Path(name).name != name:
            raise ValueError(f"invalid or duplicate asset name: {name}")
        names.add(name)
        for key in ("source", "version", "license", "license_reference", "type", "limitations"):
            if not item.get(key):
                raise ValueError(f"missing {key}: {name}")
        target = (folder / name).resolve()
        if target.parent != folder.resolve() or not target.is_file():
            raise ValueError(f"missing or outside sample directory: {name}")
        data = target.read_bytes()
        if len(data) != item["size_bytes"] or hashlib.sha256(data).hexdigest() != item["sha256"]:
            raise ValueError(f"sample integrity mismatch: {name}")
        print(f"CHECK {name}: {len(data)} bytes, SHA-256 OK")
    required = {"astronaut.png", "cell.png", "astronaut-motion.avi", "annotations.json",
                "imagenet-synset.txt", "onnx-model-zoo-LICENSE.txt"}
    if names != required:
        raise ValueError("sample manifest must list the complete fixed sample set")
    model = manifest["model"]
    if not all(model.get(k) for k in ("url", "sha256", "license", "license_reference", "version", "input", "output")):
        raise ValueError("model metadata incomplete")
    if not model["url"].startswith("https://") or not re.fullmatch(r"[0-9a-f]{64}", model["sha256"]):
        raise ValueError("invalid model URL or hash")
    annotation = json.loads((folder / "annotations.json").read_text(encoding="utf-8"))
    if len(annotation["source_crop_x"]) != annotation["frames"]:
        raise ValueError("frame annotation count mismatch")
    if len((folder / "imagenet-synset.txt").read_text(encoding="utf-8").splitlines()) != 1000:
        raise ValueError("expected 1000 model labels")
    print("PASS: offline sample provenance, hashes, and annotation count")


def main():
    root = Path(__file__).resolve().parent.parent
    verify(root / "tests/fixtures/real")


if __name__ == "__main__":
    main()
