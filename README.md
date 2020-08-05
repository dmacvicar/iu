
# iu

![Build](https://github.com/dmacvicar/iu/workflows/Build/badge.svg)

"iu" is a tool to search and browse your image collection.

The name comes from "mu", which is a mail indexer that inspired this project.
"mu" means maildir utils, so I guess "iu" means "image utils".

# What should "iu" be?

* Just a command line tool
* Targetted to the average person collecing lot of photos over the years
* Basic integration with other tools
  eg. query search results opening in some album viewer
* Fast indexing when a couple of new photos are added to the collection
* Reasonable fast indexing when re-indexing from scratch
* Some basic features when indexing:
  - Camera model
* Some fancy indexing features I expect to add at some point:
  - Turn GPS data into places names (reverse geo-location), offline
  - Recognize basic objects (food, guitars, animals, bikes, cars, colors) and index on the object word
  - OCR, index on words in the image
  - Recognize people and index them
* Ultra fast searching

# Building from source

You need:

* Xapian
* exiv2

```
cmake -S . -B build
cmake --build build
```

Alternatively, you can build with _libexif_, which is much faster. I haven't settled yet for the metadata library. I will see down the road if I trade _exiv2_ reliability with _libexif_ speed, of if I am doing something wrong. For building with _libexif_:

```
cmake -DIMAGE_METADATA_BACKEND=libexif -S . -B build
cmake --build build
```

# Running

## Indexing images

```
# libexif
$ build/src/iu index --root ~/Pictures
...
indexed: 15465 files
```

### Indexing Performance

Without many optimizations, I can index 15k files (50G) in 2.7s on a old X230 laptop with SSD (libexif backend).
Adding offline geolocation over 121k places brings that up to 16s.

Using exiv2 makes indexing 3 times slower, so it would be around 22s with geolocation (which stays constant).

## Search

```
$ build/src/iu find "camera:powershot"
8725 result found
0: docid /home/foo/1.jpg
...
real    0m0.013s
user    0m0.008s
sys     0m0.005s
```

# Implementation Notes

## Reverse geocoding index

Using a database like this one?:

* https://github.com/richardpenman/reverse_geocode
* https://raw.githubusercontent.com/richardpenman/reverse_geocode/master/geocode.csv
* https://raw.githubusercontent.com/richardpenman/reverse_geocode/master/countries.csv

# License

* "iu" is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

* "iu" is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
