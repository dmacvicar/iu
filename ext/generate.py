# generates a subset of words.txt containing the synset 1000 words
# plus all the parent relationship words

# expand recursively synset with all the hierarchy of concept
def expand_synset(synset, identifier, parents):
    synset.add(identifier)
    if identifier in parents:
        expand_synset(synset, parents[identifier], parents)

def expand_words(words, identifier, parents):
    if identifier in parents:
        return words[identifier] + ', ' + expand_words(words, parents[identifier], parents)
    else:
        return words[identifier]

with open('synsets.txt') as f:
    synset = [x.strip() for x in f.readlines()]

with open('words.txt') as f:
    words = dict({x[0].strip():x[1].strip() for x in (line.split('\t') for line in f.readlines())})

with open('wordnet.is_a.txt') as f:
    parents = dict({x[1].strip():x[0].strip() for x in (line.split(' ') for line in f.readlines())})

for identifier in synset:
    print(expand_words(words, identifier, parents))
