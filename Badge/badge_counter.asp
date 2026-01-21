<!-- #include file="counter_json.inc" -->
<%
Response.Expires = 0
Response.Buffer = True
Response.ContentType = "text/html; charset=utf-8"

Dim pageKey, theme, mode, count

' 1. Page key to count (usually a path like /path/to/page)
pageKey = Request.QueryString("url")
If pageKey = "" Then
    pageKey = Request.ServerVariables("HTTP_REFERER")
End If
If pageKey = "" Then
    pageKey = "unknown"
End If
pageKey = Trim(pageKey)

' 2. Theme and mode
theme = LCase(Trim(Request.QueryString("theme")))
If theme <> "dark" Then theme = "light"

mode = LCase(Trim(Request.QueryString("mode")))
If mode <> "view" And mode <> "increment" Then mode = "increment"

' 3. Increment or just read
If mode = "increment" Then
    count = IncrementPageCounter(pageKey)
Else
    count = GetPageCounter(pageKey)
End If

' 4. Colors
Dim bgColor, textColor, labelColor, borderColor
If theme = "dark" Then
    bgColor     = "#24292f"
    borderColor = "#57606a"
    textColor   = "#f6f8fa"
    labelColor  = "#8b949e"
Else
    bgColor     = "#e1e4e8"
    borderColor = "#d0d7de"
    textColor   = "#24292f"
    labelColor  = "#57606a"
End If
%>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="utf-8" />
    <title>Page view counter badge</title>
    <style>
        body {
            margin: 0;
            padding: 0;
            font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Arial, sans-serif;
            background: transparent;
        }
        .badge {
            display: inline-flex;
            align-items: center;
            border-radius: 999px;
            padding: 4px 10px;
            font-size: 12px;
            border: 1px solid <%=borderColor%>;
            background-color: <%=bgColor%>;
            color: <%=textColor%>;
            box-sizing: border-box;
        }
        .badge-label {
            margin-right: 6px;
            color: <%=labelColor%>;
            white-space: nowrap;
        }
        .badge-count {
            font-weight: 600;
            white-space: nowrap;
        }
    </style>
</head>
<body>
    <div class="badge">
        <span class="badge-label">Visits</span>
        <span class="badge-count"><%=count%></span>
    </div>
</body>
</html>