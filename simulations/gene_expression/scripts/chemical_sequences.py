"""Chemical sequences loaded into simulations"""

import re
from collections import Counter


class Rnas(object):
    parser = re.compile(r'ChemicalSequence ([\w]+) '
                        'product_of [\w]+ [0-9]+ [0-9]+ rnas')

    def __init__(self, file):
        with open(file) as input_:
            self.elements = self.parser.findall(input_.read())


class Proteins(object):
    parser = re.compile(r'ChemicalSequence ([\w]+) '
                        'product_of ([\w]+) [0-9]+ [0-9]+ proteins')

    def __init__(self, file):
        with open(file) as input_:
            self.elements = self.parser.findall(input_.read())
        self.count = Counter(p[0] for p in self.elements)
        self.unique_elements = set(p[0] for p in self.elements)
