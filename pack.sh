zip tmp.zip */init*.* */*.md
rm -rf dot
mkdir dot
cd dot && unzip ../tmp.zip && cd ..
rm tmp.zip
zip dot.zip -r dot
rm -rf dot