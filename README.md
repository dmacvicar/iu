
# iu

"iu" is a tool to search and browse your image collection.

The name comes from "mu", which is a mail indexer that inspired this project.
"mu" means maildir utils, so I guess "iu" means "image utils".

# Feature Plan

## Reverse geocoding index

Take the GPS coordinates from EXIF, reverse geocode it completely offline, and add the place name to the index.

Probably using a database like this one:

* https://github.com/richardpenman/reverse_geocode
* https://raw.githubusercontent.com/richardpenman/reverse_geocode/master/geocode.csv
* https://raw.githubusercontent.com/richardpenman/reverse_geocode/master/countries.csv

## Misc

- [ ] Collect files that failed to index
