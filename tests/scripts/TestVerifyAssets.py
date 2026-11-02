"""Regression tests for sample corruption and missing manifest entries."""
import importlib.util
import json
from pathlib import Path
import shutil
import tempfile
import unittest


ROOT = Path(__file__).resolve().parents[2]
SPEC = importlib.util.spec_from_file_location("verify_assets", ROOT / "scripts/VerifyAssets.py")
MODULE = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(MODULE)


class TestVerifyAssets(unittest.TestCase):
    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory()
        self.folder = Path(self.temporary.name) / "real"
        shutil.copytree(ROOT / "tests/fixtures/real", self.folder)

    def tearDown(self):
        self.temporary.cleanup()

    def test_valid_assets_pass(self):
        MODULE.verify(self.folder)

    def test_corrupt_image_is_rejected(self):
        (self.folder / "cell.png").write_bytes(b"corrupt")
        with self.assertRaisesRegex(ValueError, "integrity mismatch"):
            MODULE.verify(self.folder)

    def test_missing_manifest_entry_is_rejected(self):
        manifest_path = self.folder / "manifest.json"
        manifest = json.loads(manifest_path.read_text(encoding="utf-8"))
        manifest["assets"] = manifest["assets"][:-1]
        manifest_path.write_text(json.dumps(manifest), encoding="utf-8")
        with self.assertRaisesRegex(ValueError, "complete fixed sample set"):
            MODULE.verify(self.folder)

    def test_path_traversal_is_rejected(self):
        manifest_path = self.folder / "manifest.json"
        manifest = json.loads(manifest_path.read_text(encoding="utf-8"))
        manifest["assets"][0]["path"] = "../astronaut.png"
        manifest_path.write_text(json.dumps(manifest), encoding="utf-8")
        with self.assertRaisesRegex(ValueError, "invalid or duplicate"):
            MODULE.verify(self.folder)


if __name__ == "__main__":
    unittest.main()
