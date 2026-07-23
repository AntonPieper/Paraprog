#!/usr/bin/env python3
"""Download every file resource from the ParaProg Moodle course."""

import argparse
import getpass
import html.parser
import http.cookiejar
import mimetypes
import os
import re
import urllib.parse
import urllib.request
from pathlib import Path

COURSE_URL = "https://elearning.hs-fulda.de/ai/course/view.php?id=1661"
LOGIN_URL = "https://elearning.hs-fulda.de/ai/login/index.php"


class CoursePageParser(html.parser.HTMLParser):
    def __init__(self):
        super().__init__()
        self.login_token = None
        self.resources = []
        self._resource_url = None
        self._resource_label = []

    def handle_starttag(self, tag, attrs):
        attrs = dict(attrs)
        if tag == "input" and attrs.get("name") == "logintoken":
            self.login_token = attrs.get("value")
        if tag == "a" and "/mod/resource/view.php" in attrs.get("href", ""):
            self._resource_url = attrs["href"]
            self._resource_label = []

    def handle_data(self, data):
        if self._resource_url:
            self._resource_label.append(data)

    def handle_endtag(self, tag):
        if tag == "a" and self._resource_url:
            label = " ".join("".join(self._resource_label).split())
            self.resources.append((self._resource_url, label))
            self._resource_url = None


def read_page(opener, url):
    with opener.open(url) as response:
        return response.read().decode("utf-8", errors="replace")


def parse_page(page):
    parser = CoursePageParser()
    parser.feed(page)
    return parser


def safe_filename(name):
    return re.sub(r'[<>:"/\\|?*]', "_", name).strip(" .")


def main():
    cli = argparse.ArgumentParser(description=__doc__)
    cli.add_argument("-u", "--username", default=os.getenv("MOODLE_USERNAME"))
    cli.add_argument("-o", "--output", type=Path, default=Path("course-materials"))
    args = cli.parse_args()

    username = args.username or input("Moodle username: ")
    password = os.getenv("MOODLE_PASSWORD") or getpass.getpass("Moodle password: ")

    cookies = http.cookiejar.CookieJar()
    opener = urllib.request.build_opener(urllib.request.HTTPCookieProcessor(cookies))

    login_page = parse_page(read_page(opener, LOGIN_URL))
    if not login_page.login_token:
        raise SystemExit("Could not find Moodle's login token.")

    credentials = urllib.parse.urlencode(
        {"username": username, "password": password, "logintoken": login_page.login_token}
    ).encode()
    request = urllib.request.Request(LOGIN_URL, data=credentials)
    with opener.open(request) as response:
        response.read()

    course_page = parse_page(read_page(opener, COURSE_URL))
    if not course_page.resources:
        raise SystemExit("Login failed, or the course contains no downloadable files.")

    args.output.mkdir(parents=True, exist_ok=True)
    resources = dict(course_page.resources)  # Remove duplicate links, keep labels.

    for number, (url, label) in enumerate(resources.items(), 1):
        separator = "&" if "?" in url else "?"
        with opener.open(url + separator + "redirect=1") as response:
            filename = response.headers.get_filename()
            if not filename:
                filename = safe_filename(label.removesuffix("Datei").strip()) or f"resource-{number}"
                filename += mimetypes.guess_extension(response.headers.get_content_type()) or ""
            filename = safe_filename(Path(filename).name)

            destination = args.output / filename
            if destination.exists():
                print(f"[{number}/{len(resources)}] skip {filename}")
                continue

            temporary = destination.with_suffix(destination.suffix + ".part")
            with temporary.open("wb") as file:
                while chunk := response.read(1024 * 1024):
                    file.write(chunk)
            temporary.replace(destination)
            print(f"[{number}/{len(resources)}] saved {filename}")


if __name__ == "__main__":
    main()
