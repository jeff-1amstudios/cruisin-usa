"""Run production startup against local HTTP fixtures, without arcade ROM data.

cmake --build build --target startup-probe
python3 tools/test_startup.py build/startup-probe
"""
import hashlib
import http.server
import io
from pathlib import Path
import re
import subprocess
import sys
import tempfile
import threading
import unittest
import zipfile

PROBE = Path(sys.argv.pop(1)).resolve()
ROOT = Path(__file__).resolve().parents[1]


def fixture_zip():
    data = io.BytesIO()
    names = re.findall(r'\{ "([^"]+)", 0x', (ROOT / 'src/portable/rom.c').read_text())
    assert len(names) == 20
    with zipfile.ZipFile(data, 'w', zipfile.ZIP_DEFLATED) as archive:
        for name in names:
            archive.writestr(name, bytes(0x80000))
    return data.getvalue()


VALID = fixture_zip()


class Handler(http.server.BaseHTTPRequestHandler):
    requests = []

    def log_message(self, *args):
        pass

    def do_GET(self):
        self.requests.append(self.path)
        if self.path == '/redirect':
            self.send_response(302)
            self.send_header('Location', '/valid')
            self.end_headers()
            return
        if self.path == '/missing':
            self.send_error(404)
            return
        data = VALID if self.path in ('/valid', '/truncated') else b'<html>not a ROM</html>'
        self.send_response(200)
        self.send_header('Content-Length', str(len(data) + (10 if self.path == '/truncated' else 0)))
        self.end_headers()
        self.wfile.write(data)


class StartupTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.server = http.server.ThreadingHTTPServer(('127.0.0.1', 0), Handler)
        cls.thread = threading.Thread(target=cls.server.serve_forever, daemon=True)
        cls.thread.start()
        cls.url = f'http://127.0.0.1:{cls.server.server_port}'

    @classmethod
    def tearDownClass(cls):
        cls.server.shutdown()
        cls.server.server_close()
        cls.thread.join()

    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.work = Path(self.temp.name)
        (self.work / 'roms').mkdir()
        self.cache = self.work / 'roms/crusnusa.zip'
        self.part = self.work / 'roms/crusnusa.zip.part'
        Handler.requests.clear()

    def run_probe(self, path, imported='', success=True, retry=False):
        arguments = [str(PROBE), self.url + path, str(imported)]
        if retry:
            arguments.append('--retry')
        result = subprocess.run(arguments,
                                cwd=self.work, capture_output=True, text=True, timeout=20)
        self.assertEqual(result.returncode, 0 if success else 1, result.stdout + result.stderr)
        self.assertFalse(self.part.exists())
        return result

    def test_download_and_cache(self):
        self.run_probe('/valid')
        self.assertEqual(self.cache.read_bytes(), VALID)
        self.run_probe('/missing')
        self.assertEqual(Handler.requests, ['/valid'])

    def test_http_redirect(self):
        self.run_probe('/redirect')
        self.assertEqual(Handler.requests, ['/redirect', '/valid'])
        self.assertEqual(self.cache.read_bytes(), VALID)

    def test_http_failure(self):
        self.run_probe('/missing', success=False)
        self.assertFalse(self.cache.exists())

    def test_invalid_download(self):
        self.run_probe('/invalid', success=False)
        self.assertFalse(self.cache.exists())

    def test_incomplete_download(self):
        self.run_probe('/truncated', success=False)
        self.assertFalse(self.cache.exists())

    def test_invalid_import_preserves_cache(self):
        self.cache.write_bytes(VALID)
        before = hashlib.sha256(self.cache.read_bytes()).digest()
        candidate = self.work / 'bad.zip'
        candidate.write_bytes(b'not a zip')
        self.run_probe('/missing', candidate, success=False)
        self.assertEqual(hashlib.sha256(self.cache.read_bytes()).digest(), before)
        self.assertEqual(Handler.requests, [])

    def test_import_replaces_invalid_cache(self):
        self.cache.write_bytes(b'old invalid cache')
        candidate = self.work / 'good.zip'
        candidate.write_bytes(VALID)
        self.run_probe('/missing', candidate)
        self.assertEqual(self.cache.read_bytes(), VALID)
        self.assertEqual(Handler.requests, [])

    def test_failed_retry_preserves_cache(self):
        self.cache.write_bytes(VALID)
        self.run_probe('/invalid', retry=True, success=False)
        self.assertEqual(self.cache.read_bytes(), VALID)
        self.assertEqual(Handler.requests, ['/invalid'])

    def test_retry_downloads_even_with_valid_cache(self):
        self.cache.write_bytes(VALID)
        self.run_probe('/valid', retry=True)
        self.assertEqual(self.cache.read_bytes(), VALID)
        self.assertEqual(Handler.requests, ['/valid'])

    def test_wrong_rom_set(self):
        candidate = self.work / 'wrong.zip'
        with zipfile.ZipFile(candidate, 'w') as archive:
            archive.writestr('wrong-version.u10', bytes(0x80000))
        self.run_probe('/missing', candidate, success=False)
        self.assertFalse(self.cache.exists())


if __name__ == '__main__':
    unittest.main()
