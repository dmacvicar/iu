
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
  - [X] Offline automatic tagging: Recognize basic entities (food, guitars, animals, bikes, cars, colors) and index on the object word
  - [ ] Search similar images, to detect duplicates while sorting my collection
  - [ ] Find images with low quality, to be used when curating my camera inbox
  - [ ] OCR, index on words in the image
  - [ ] Recognize people and index them
- [X] Ultra fast searching

# Installation

For now the only method is building from source.

## Building from source

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

or

```
$ cd build
cmake ..
make
```

# Running

## Getting data files

```
cmake --build build --target data
```

or..

```
$ cd build
$ make data
```

## Indexing images

```
$ cd build
$ src/iu index --root ~/Pictures
...
indexed: 15465 files
```

You can use the `--ai` and other flags to extract and index additional information from the image.

## Search

```
$ cd build
$ src/iu find "camera:powershot"
8725 result found
0: docid /home/foo/1.jpg
...
real    0m0.013s
user    0m0.008s
sys     0m0.005s
```

## Configuration

Every option accepted by a command can be configured in `~/.config/iu.conf`. For example for AI options:

``` ini
[index]

ai-base-url = http://localhost:11434/v1/
ai-model = "minicpm-v"
ai-api_key = XXXXXXXXXXXXXX
```

# Performance

* Without many optimizations, I can index 15k files (50G) in 2.7s on a old X230 laptop with SSD (libexif backend).
* Adding offline geolocation over 121k places brings that up to 16s.

# Design and Implementation Notes

## Technologies

* Indexing is built on top of [Xapian](https://xapian.org/), a free and open-source probabilistic information retrieval library.

 The idea of using [SQLite] was considered too.

* Metadata from photos is retrieved using [libexif](https://libexif.github.io).

  exiv2 was tested and while the API and format coverage was wider, it was much slower.

* Examination of images is done with the help of [Open Computer Vision Library](https://opencv.org).

## Caching

This is not implemented yet. Ideally, we should cache data that is expensive to compute. This could be done by implementing a cache based on the file checksum.

## Writing data back to the images

This is not implemented yet. Ideally, we could write some data we extract into the files themselves. This would allow eg. to preserve AI keywords when uploading the files to the cloud (e.g. Nextcloud).

However, the standards  (EXIF, IPTC, XMP) and different terminologies (Tag,Label,Subject, Keyword, Category) does not make it straightforward (see [#891](https://github.com/pulsejet/memories/issues/891))

## Reverse geocoding index

Uses data from [reverse_geocode](https://github.com/richardpenman/reverse_geocode), which is turn, comes from [geonames.org](https://geonames.org). [CC-By licence](http://creativecommons.org/licenses/by/4.0/).

It is a dumb search by distance and it is not optimized yet.

Right now the technique is that we convert the photo location into a label (place name) and add this name to the index as a term. Therefore the place is passed into the query.

An alternative approach I am exploring is to allow to pass the place as part of a command line, separate from the query, and use Xapian geospatial (ie. `LatLongDistancePostingSource`), adding this posting source to the query object.

I will start this exploration by adding the location as a value to the document.

## Automatic labeling

### OpenCV with model

Uses [Berkeley Vision and Learning Center Caffe](https://caffe.berkeleyvision.org/) GoogleNet model, and the word list from [ImageNet](http://www.image-net.org).

I would still like to allow to drop models and labels list in a directory and have the indexer pick it up automatically.

### AI

I called this feature "AI" as everyone uses it for this, but perhaps there is a better name.

Vision capable LLMs are perfect for improving the indexing capabilities, because we can ask them to describe photos with much more level of detail and there are models continuously improving these capabilities.

AI allows extracting image keywords for indexing using an openAI compatible end-point and it is supposed to be the replacement for the GoogleNet model.

Having `iu` support the openAI API means that we can support local models via [ollama](https://ollama.com/), avoiding having to implement model and GPU management and execution inside `iu`. The user will be able to download, update and run models via `ollama`. At the same time, `iu` can be configured to use an external service like openAI or [Mistral Pixtral](https://mistral.ai/news/pixtral-12b/), which provide better models at much higher speeds.

AI can be configured in the command line, or in `$HOME/.config/iu.conf`:

```ini
[index]

ai-base-url = http://localhost:11434/v1/
ai-model = "minicpm-v"
ai-api_key = XXXXXXXXXXXXXX
```

## Quality classification

Uses the BRISQUE (Blind/Referenceless Image Spatial Quality Evaluator), a No Reference Image Quality Assessment (NR-IQA) algorithm as in implemented in OpenCV contrib.

We use the trained model provided in the /samples/ directory, trained on the LIVE-R2 database as in the original implementation.

Right now we don't do anything with this except of adding the word "blurry" to the index. In theory I should add this as a value.

## Browsing photos

Right now if you add "-b" (browse) to a search, it will pass the list of files in the result to `eog`. This does not work well, as there is a limit on the number of files, and if there are no results, `eog` will still show other files. I am looking for a good replacement.

Hopefully I don't need to write my own.

# License

* (C)2020 Duncan Mac-Vicar P.

* "iu" is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

* "iu" is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
