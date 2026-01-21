# Page view badge in classic asp

[![Codacy Badge](https://app.codacy.com/project/badge/Grade/f5fdb6e8a5df47f385f2b4d5778c06b2)](https://app.codacy.com/gh/R0mb0/Page_view_badge_classic_asp/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade)

[![Maintenance](https://img.shields.io/badge/Maintained%3F-yes-green.svg)](https://github.com/R0mb0/Page_view_badge_classic_asp)
[![Open Source Love svg3](https://badges.frapsoft.com/os/v3/open-source.svg?v=103)](https://github.com/R0mb0/Page_view_badge_classic_asp)
[![MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/license/mit)

[![Donate](https://img.shields.io/badge/PayPal-Donate%20to%20Author-blue.svg)](http://paypal.me/R0mb0)

Simple page view counter badge for Classic ASP applications. Stores counts in a JSON file, uses per-page keys, and can be embedded via iframe to show a light or dark themed visit counter on any legacy site, including static HTML pages.

## ⚙️ How it works

- `counter_json.inc` provides a tiny JSON-based storage for page view counters.
- `badge_counter.asp` renders a GitHub-style "Visits" badge and reads/updates the counter.
- `test_page.asp` is a minimal example page that embeds the badge via `<iframe>`.

Counters are stored in a JSON file on disk (`page_counters.json`).  
Each page is identified by a simple **page key** (for example, `/test_page.asp`).

## 📦 Installation

1. Copy these files into a Classic ASP application:

   - `counter_json.inc`
   - `badge_counter.asp`
   - `test_page.asp` (optional, for testing)

2. Edit `COUNTER_JSON_PATH` in `counter_json.inc`:

   ```asp
   Const COUNTER_JSON_PATH = "C:\inetpub\wwwroot\data\page_counters.json"
   ```

   Make sure:

   - The directory exists (`C:\inetpub\wwwroot\data\` in this example).
   - The IIS app pool user has read/write permissions.

3. Ensure `badge_counter.asp` can include the INC file:

   ```asp
   <!-- #include file="counter_json.inc" -->
   ```

   This works when both files are in the same folder.  
   Otherwise, adjust the path accordingly (for example, `../includes/counter_json.inc`).

## 🚀 Usage

### 🧩 From a Classic ASP page

Example (`test_page.asp` shows this):

```asp
<%
Dim pageKey
pageKey = Request.ServerVariables("SCRIPT_NAME")
%>

<iframe
  src="badge_counter.asp?url=<%=Server.URLEncode(pageKey)%>&theme=light&mode=increment"
  style="border:none;width:260px;height:40px;"
  scrolling="no">
</iframe>
```

- `url` (required): page key to count.  
  Use a relative path (e.g. `/articles/details.asp?id=42`) or any unique string.
- `theme` (optional): `light` (default) or `dark`.
- `mode` (optional):
  - `increment` — increment and display the counter (default);
  - `view` — display the current value without incrementing.

### 📄 From a static HTML page

You can embed the badge from **any** technology (static HTML, PHP, ASP.NET, CMS, etc.) using an iframe that points to your ASP badge endpoint:

```html
<iframe
  src="https://example.com/badge_counter.asp?url=/static/page.html&theme=light&mode=increment"
  style="border:none;width:260px;height:40px;"
  scrolling="no">
</iframe>
```

Here the key `/static/page.html` is counted each time the page is loaded.

## 🗂️ JSON storage format

The JSON file uses a very simple structure:

```json
{
  "/test_page.asp": 12,
  "/static/page.html": 34
}
```

- Keys are page identifiers (paths or custom strings).
- Values are numeric visit counters.

## 📝 Notes

- The implementation uses `Application.Lock`/`Application.UnLock` to avoid race conditions when multiple requests update the JSON file.
- The JSON parser is intentionally simple and assumes keys do not contain commas.  
  Using short path-like keys (e.g. `/path/to/page`) is recommended.
