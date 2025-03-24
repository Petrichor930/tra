# How to use

将该目录的文件复制到.git/hooks/下
Linux
```
find tools/hooks -type f ! -name 'README.md' -exec cp {} .git/hooks/ \;
```
Windows
```
Get-ChildItem -Path tools/hooks -File -Exclude README.md | ForEach-Object {
    Copy-Item -Path $_.FullName -Destination .git/hooks
}
```
