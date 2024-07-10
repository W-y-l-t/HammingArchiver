# HammingArchiver

> Noise-resistant file archiver without compression (console app). \
> For noise-tolerant coding, was used [Hamming code](https://en.wikipedia.org/wiki/Hamming_code).

## Technical specification

-----------------------------
-c, --create - create a new archive

-----------------------------
--f, --file=[ARCNAME] - the name of the archive file \
No more than one parameter of this type per run

-----------------------------
-l, --list - display a list of files in the archive

-----------------------------
-x, --extract - extract files from the archive (if not specified, then all files) \
No more than one parameter of this type per run

-----------------------------
-a, --append - add file to archive \
No more than one parameter of this type per run

-----------------------------
-d, --delete - delete a file from the archive \
No more than one parameter of this type per run \
Simultaneous use of -x, -a, -d queries is prohibited

-----------------------------
-A, --concatenate - merge two archives \
No more than one request of this type per launch \
The encoding and decoding parameters for the two archives must match

-----------------------------

--i, --in=[VALUE] - the number of bits in one block when ENCODING \
The optimal value is 4 (for encoding (7, 4)) \
No more than one parameter of this type per run \
Only if a new archive is created \

    ^
    |
    |   must be in one run
    |
    v

--o, --out=[VALUE] - the number of bits in one block during DECODING \
No more than one parameter of this type per run \
Only if a new archive is created

-----------------------------

## Run examples

- hamarc --create --file=ARCHIVE FILE1 FILE2 FILE3
- hamarc --concantenate ARCHIVE1 ARCHIVE2 -f ARCHIVE3
- hamarc -l -f ARCHIVE
- -f "MyArchive.haf" -x "S.txt" "Homework7.3.jpg" "TripAside.txt" "My favourite photo.docx" -i 11 -o 15

