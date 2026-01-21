<%
' ===============================================================================
' BASIC CONFIGURATION
' ===============================================================================

' Physical path of the JSON file that stores page view counters.
' Change this path according to your environment.
Const COUNTER_JSON_PATH = "C:\inetpub\wwwroot\data\page_counters.json"

' ===============================================================================
' FILE AND SIMPLE JSON HELPER FUNCTIONS
' ===============================================================================

Sub EnsureCounterFileExists()
    Dim fso, folderPath, file
    Set fso = Server.CreateObject("Scripting.FileSystemObject")

    folderPath = fso.GetParentFolderName(COUNTER_JSON_PATH)
    If Not fso.FolderExists(folderPath) Then
        fso.CreateFolder folderPath
    End If

    If Not fso.FileExists(COUNTER_JSON_PATH) Then
        Set file = fso.CreateTextFile(COUNTER_JSON_PATH, True)
        file.Write "{}"
        file.Close
        Set file = Nothing
    End If

    Set fso = Nothing
End Sub

Function ReadAllText(path)
    Dim fso, file, text
    Set fso = Server.CreateObject("Scripting.FileSystemObject")

    If Not fso.FileExists(path) Then
        ReadAllText = ""
        Set fso = Nothing
        Exit Function
    End If

    Set file = fso.OpenTextFile(path, 1, False) ' ForReading
    text = file.ReadAll
    file.Close

    Set file = Nothing
    Set fso = Nothing

    ReadAllText = text
End Function

Sub WriteAllText(path, text)
    Dim fso, file, folderPath
    Set fso = Server.CreateObject("Scripting.FileSystemObject")

    folderPath = fso.GetParentFolderName(path)
    If Not fso.FolderExists(folderPath) Then
        fso.CreateFolder folderPath
    End If

    Set file = fso.OpenTextFile(path, 2, True) ' ForWriting (creates if it does not exist)
    file.Write text
    file.Close

    Set file = Nothing
    Set fso = Nothing
End Sub

Function JsonEscape(str)
    Dim s
    s = str
    s = Replace(s, "\", "\\")
    s = Replace(s, """", "\""")
    s = Replace(s, vbCrLf, "\n")
    s = Replace(s, vbCr, "\n")
    s = Replace(s, vbLf, "\n")
    JsonEscape = s
End Function

' ===============================================================================
' SIMPLE PARSER FOR A JSON DICTIONARY { "key": value, ... } WITH NUMERIC VALUES
' ===============================================================================

Function ParseSimpleJsonToDic(jsonText)
    Dim dic, cleaned, items, i, pair, keyPart, valuePart, key, value, pos

    Set dic = Server.CreateObject("Scripting.Dictionary")

    cleaned = Trim(jsonText)
    If cleaned = "" Then
        Set ParseSimpleJsonToDic = dic
        Exit Function
    End If

    If Left(cleaned, 1) = "{" Then cleaned = Mid(cleaned, 2)
    If Right(cleaned, 1) = "}" Then cleaned = Left(cleaned, Len(cleaned) - 1)

    cleaned = Trim(cleaned)
    If cleaned = "" Then
        Set ParseSimpleJsonToDic = dic
        Exit Function
    End If

    items = Split(cleaned, ",")

    For i = 0 To UBound(items)
        pair = Trim(items(i))
        If pair <> "" Then
            keyPart = ""
            valuePart = ""

            pos = InStr(pair, ":")
            If pos > 0 Then
                keyPart = Trim(Left(pair, pos - 1))
                valuePart = Trim(Mid(pair, pos + 1))
            End If

            If Left(keyPart, 1) = """" And Right(keyPart, 1) = """" Then
                key = Mid(keyPart, 2, Len(keyPart) - 2)
            Else
                key = keyPart
            End If

            If valuePart <> "" Then
                If Left(valuePart, 1) = """" And Right(valuePart, 1) = """" Then
                    valuePart = Mid(valuePart, 2, Len(valuePart) - 2)
                End If

                On Error Resume Next
                value = CLng(valuePart)
                If Err.Number <> 0 Then
                    value = 0
                    Err.Clear
                End If
                On Error GoTo 0
            Else
                value = 0
            End If

            If Not dic.Exists(key) Then
                dic.Add key, value
            Else
                dic(key) = value
            End If
        End If
    Next

    Set ParseSimpleJsonToDic = dic
End Function

Function DicToSimpleJson(dic)
    Dim json, k, first
    json = "{"
    first = True

    For Each k In dic.Keys
        If Not first Then
            json = json & ","
        Else
            first = False
        End If
        json = json & """" & JsonEscape(CStr(k)) & """: " & CStr(dic(k))
    Next

    json = json & "}"
    DicToSimpleJson = json
End Function

' ===============================================================================
' HIGH-LEVEL API FOR PAGE VIEW COUNTERS
' ===============================================================================

Function IncrementPageCounter(pageKey)
    Dim jsonText, dic, currentCount

    EnsureCounterFileExists

    Application.Lock

    jsonText = ReadAllText(COUNTER_JSON_PATH)
    Set dic = ParseSimpleJsonToDic(jsonText)

    If dic.Exists(pageKey) Then
        currentCount = CLng(dic(pageKey)) + 1
    Else
        currentCount = 1
    End If

    dic(pageKey) = currentCount

    jsonText = DicToSimpleJson(dic)
    WriteAllText COUNTER_JSON_PATH, jsonText

    Application.UnLock

    IncrementPageCounter = currentCount
End Function

Function GetPageCounter(pageKey)
    Dim jsonText, dic, count

    EnsureCounterFileExists

    jsonText = ReadAllText(COUNTER_JSON_PATH)
    Set dic = ParseSimpleJsonToDic(jsonText)

    If dic.Exists(pageKey) Then
        count = CLng(dic(pageKey))
    Else
        count = 0
    End If

    GetPageCounter = count
End Function
%>