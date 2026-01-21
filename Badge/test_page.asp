<%
Dim pageKey
Dim path, qs

' Use only the path + query string as the page key.
' This avoids exposing full server URLs and keeps JSON keys simple.
path = Request.ServerVariables("SCRIPT_NAME")
qs   = Request.ServerVariables("QUERY_STRING")

pageKey = path
If qs <> "" Then
    pageKey = pageKey & "?" & qs
End If
%>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="utf-8" />
    <title>Page view counter - test page</title>
</head>
<body>
    <h1>This is a test page</h1>
    <p>Reload this page several times to see the counter increase.</p>

    <iframe
        src="badge_counter.asp?url=<%=Server.URLEncode(pageKey)%>&theme=light&mode=increment"
        style="border:none;width:260px;height:40px;"
        scrolling="no">
    </iframe>
</body>
</html>