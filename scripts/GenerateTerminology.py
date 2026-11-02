#!/usr/bin/env python3
"""Generate or check the human-readable terminology from its CSV source."""
import argparse
import csv
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
CSV = ROOT / 'docs/terminology/critical-terms.csv'
MARKDOWN = ROOT / 'docs/terminology/critical-terms.md'
FIELDS = ('id', 'english', 'part_of_speech', 'domain', 'preferred_zh', 'definition',
          'context', 'avoid', 'example_en', 'example_zh', 'version', 'source_title',
          'source_url', 'review_status', 'checked_on')


def render():
    with CSV.open(encoding='utf-8-sig', newline='') as stream:
        reader = csv.DictReader(stream)
        if set(reader.fieldnames or ()) != set(FIELDS):
            raise ValueError('CSV fields do not match the required schema')
        rows = list(reader)
    ids = set()
    for row in rows:
        if any(not row[field].strip() for field in FIELDS):
            raise ValueError(f"Missing field: {row['id']}")
        if not re.fullmatch(r'TERM-\d{3}', row['id']) or row['id'] in ids:
            raise ValueError(f"Invalid or duplicate ID: {row['id']}")
        if not row['source_url'].startswith('https://'):
            raise ValueError(f"Invalid source URL: {row['id']}")
        ids.add(row['id'])
    if not {f'TERM-{n:03d}' for n in range(1, 32)} <= ids:
        raise ValueError('Original 31 concepts must be retained')
    lines = ['# Qt 与 OpenCV 关键术语辨析', '',
             '本文件由 CSV 生成；请修改 CSV 后运行生成脚本。中文为项目推荐译法，例句为自编；来源和核对状态见各词条。', '',
             '[速查手册](../qt-opencv-vocabulary.md) · [CSV 术语库](critical-terms.csv)', '', '## 词条索引', '',
             '| 编号 | 英文 | 领域 | 推荐译法 |', '|---|---|---|---|']
    def safe(value):
        return value.replace('|', '&#124;').replace('\n', '<br>')
    for row in rows:
        lines.append(f"| {row['id']} | [{safe(row['english'])}](#{row['id'].lower()}) | {safe(row['domain'])} | {safe(row['preferred_zh'])} |")
    labels = [('part_of_speech','词性'),('domain','领域'),('preferred_zh','推荐译法'),
              ('definition','定义'),('context','语境'),('avoid','易混义或误译'),('example_en','英文例句'),
              ('example_zh','中文例句'),('version','版本'),('review_status','校核状态'),('checked_on','核对日期')]
    for row in rows:
        lines.extend(['', f"<a id=\"{row['id'].lower()}\"></a>", f"## {row['id']} · {row['english']}", ''])
        lines.extend(f"- **{label}**：{row[key]}" for key, label in labels)
        lines.append(f"- **来源**：[{row['source_title']}]({row['source_url']})")
    return '\n'.join(lines) + '\n'


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--check', action='store_true')
    args = parser.parse_args()
    result = render()
    if args.check:
        if not MARKDOWN.exists() or MARKDOWN.read_text(encoding='utf-8') != result:
            raise SystemExit('Markdown differs from CSV; run the generator')
        print('Terminology CSV and Markdown agree')
    else:
        MARKDOWN.write_text(result, encoding='utf-8', newline='\n')
        print('Generated', MARKDOWN)

if __name__ == '__main__':
    main()
