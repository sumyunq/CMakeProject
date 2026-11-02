#!/usr/bin/env python3
"""Report terminology coverage of the five vocabulary tables."""
import csv
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SECTIONS = ('图像与 Mat 的核心词汇', '内存与所有权', 'Qt Widgets 常用词', '环境与构建词汇', '随后续课程逐步学习的视觉词汇')
ALIASES = {'matrices': 'matrix', 'rows': 'row', 'cols': 'column', 'roi': 'region of interest',
           'rect': 'rectangle', 'step': 'stride', 'cow': 'copy-on-write', 'copy on write': 'copy-on-write',
           'gaussian': 'gaussian blur', 'median': 'median filter', 'intrinsic': 'intrinsic parameters',
           'nms': 'non-maximum suppression', 'qss': 'stylesheet'}


def normalize(value):
    value = value.strip().lower()
    return ALIASES.get(value, value)


def main():
    manual = (ROOT / 'docs/qt-opencv-vocabulary.md').read_text(encoding='utf-8-sig')
    with (ROOT / 'docs/terminology/critical-terms.csv').open(encoding='utf-8-sig', newline='') as f:
        terms = {normalize(row['english']) for row in csv.DictReader(f)}
    missing = []
    count = 0
    for section in SECTIONS:
        body = manual.split('## ' + section, 1)[1].split('\n## ', 1)[0]
        for line in body.splitlines():
            if not line.startswith('| ') or line.startswith('|---'):
                continue
            cells = line.split('|')
            cell = cells[2 if section == SECTIONS[-1] else 1].strip()
            if '英文' in cell:
                continue
            for word in re.split(r'\s*/\s*|；', cell):
                word = word.strip()
                if not word:
                    continue
                count += 1
                if normalize(word) not in terms:
                    missing.append((section, word))
    for section, word in missing:
        print(f'{section}: {word}')
    print(f'速查表词项 {count}，缺少词项 {len(missing)}')
    if missing:
        raise SystemExit(1)

if __name__ == '__main__':
    main()
