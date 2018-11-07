if (Test-Path bin\exe\Debug == false)
{
    md bin\exe\Debug
}

Copy-Item -Path resources\exe_root -Filter *.* -Destination bin\exe\Debug –Recurse
