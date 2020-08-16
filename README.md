
# iu

![Build](https://github.com/dmacvicar/iu/workflows/Build/badge.svg)

"iu" is an experiment that started with this [tweet](https://twitter.com/dmacvicar/status/1279711325455880193).

The goal is to do research around a tool to index and searching your image collection.

"iu" is not intended for productive use, and perhaps will never be.

The name comes from ["mu"](https://www.djcbsoftware.nl/code/mu/), which is a mail indexer that inspired this project.
"mu" means maildir utils, so I guess "iu" means "image utils".

# What should "iu" be?

- [X] Just a command line tool
- [X] Targetted to the average person collecing lot of photos over the years
- [X] Basic integration with other tools
      eg. query search results opening in some album viewer
- [X] Reasonable fast indexing when re-indexing from scratch
- [ ] Very fast indexing when a couple of new photos are added to the collection
- Some basic features when indexing:
  - [X] Camera model
  - [X] Date
  - [ ] Album (?)
- Some fancy indexing features I expect to add at some point:
  - [X] Offline reverse geo-location: Turn GPS data into places names
  - [X] Offline automatic tagging: Recognize basic objects (food, guitars, animals, bikes, cars, colors) and index on the object word
  - [ ] OCR, index on words in the image
  - [ ] Recognize people and index them
- [X] Ultra fast searching

# Building from source

You need:

* [Xapian](https://xapian.org/)
* [libexif](https://libexif.github.io/)
* [OpenCV](https://opencv.org/)

* Other dependencies like [csv2](https://github.com/p-ranav/csv2), [spdlog](https://github.com/gabime/spdlog), [fmt](https://github.com/fmtlib/fmt) and [CLI11](https://github.com/CLIUtils/CLI11) will be downloaded by the build system.

Once you satisfy those requirements

```
cmake -S . -B build
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

# Performance

Without many optimizations, I can index 15k files (50G) in 2.7s on a old X230 laptop with SSD (libexif backend).
Adding offline geolocation over 121k places brings that up to 16s.

# Implementation Notes

## Reverse geocoding index

Uses data from [reverse_geocode](https://github.com/richardpenman/reverse_geocode), which is turn, comes from [geonames.org](https://geonames.org). [CC-By licence](http://creativecommons.org/licenses/by/4.0/).

It is a dumb search by distance and it is not optimized yet.

# License

* "iu" is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

* "iu" is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
