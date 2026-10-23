"""Check local lesson documents and their offline dependencies, without network access."""
from html.parser import HTMLParser
from pathlib import Path
import json
from urllib.parse import unquote, urlsplit


class LessonParser(HTMLParser):
    def __init__(self):
        super().__init__(convert_charrefs=True)
        self.links = []
        self.tags = []
        self.details = 0
        self.summaries = 0
        self.errors = []

    def handle_starttag(self, tag, attrs):
        attributes = dict(attrs)
        for attribute in ("href", "src"):
            if attribute in attributes:
                self.links.append(attributes[attribute])
        if tag == "html" and attributes.get("lang") != "zh-CN":
            self.errors.append("missing Chinese language declaration")
        if tag == "details":
            self.details += 1
        if tag == "summary":
            self.summaries += 1
        if tag not in {"meta", "link", "br", "hr", "img", "input", "source", "wbr"}:
            self.tags.append(tag)

    def handle_endtag(self, tag):
        if not self.tags or self.tags[-1] != tag:
            self.errors.append(f"unbalanced closing tag: {tag}")
        else:
            self.tags.pop()


def main():
    root = Path(__file__).resolve().parent.parent
    failures = []
    documents = sorted((root / "lessons").glob("*.html"))
    catalog = json.loads((root / "docs/course-catalog.json").read_text(encoding="utf-8"))
    if [entry["id"] for entry in catalog] != list(range(1, 31)):
        failures.append("expected a complete 01..30 curriculum")
    if {p.name for p in documents} != {"index.html", *(entry["file"] for entry in catalog)}:
        failures.append("catalog and delivered lessons differ")
    for entry in catalog:
        for variant in ("exercise", "solution"):
            path = root / entry[variant]
            if not path.is_file() or len(path.read_text(encoding="utf-8").strip()) < 100:
                failures.append(f"lesson {entry['id']}: missing substantive {variant}")
    for document in documents:
        parsed = LessonParser()
        parsed.feed(document.read_text(encoding="utf-8"))
        failures.extend(f"{document.name}: {error}" for error in parsed.errors)
        if parsed.tags:
            failures.append(f"{document.name}: unclosed tags {parsed.tags}")
        if parsed.details != parsed.summaries:
            failures.append(f"{document.name}: details without a summary")
        if document.name != "index.html" and parsed.details < 3:
            failures.append(f"{document.name}: missing self checks")
        for link in parsed.links:
            url = urlsplit(link)
            if url.scheme or url.netloc or not url.path:
                continue
            destination = (document.parent / unquote(url.path)).resolve()
            if not destination.is_relative_to(root) or not destination.is_file():
                failures.append(f"{document.name}: broken local link {link}")
        print(f"CHECK {document.name}: {len(parsed.links)} links, {parsed.details} self-checks")
    if failures:
        raise SystemExit("\n".join(failures))
    print("PASS: local documents, styles, navigation and self-check structure")


if __name__ == "__main__":
    main()
