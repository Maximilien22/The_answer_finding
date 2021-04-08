# -*- coding: utf-8 -*-
"""General Tree module: first child - right sibling representation.

*Warning:* All the functions defined in this module are assumed to receive a non-None
value for their ``ref/B`` parameter.

"""

from . import queue
from .queue import Queue

class TreeAsBin:
    """
    Simple class for (General) Trees 
    represented as Binary Trees (first child - right sibling)
    """

    def __init__(self, key, child=None, sibling=None):
        """
        Init Tree
        """
        self.key = key
        self.child = child
        self.sibling = sibling


###############################################################################
# load and save

def to_list(B):
    s = '(' + str(B.key)
    C = B.child
    while C != None:
        s += to_list(C)
        C = C.sibling
    s += ')'
    return s

def save(B, filename):
    fout = open(filename, mode='w')
    fout.write(to_list(B))
    fout.close()
    
    
def __load(s, typelt, i=0): 
    #FIXME
    pass


def load(path, typelt=int):
    # Open file and get full content
    file = open(path, 'r')
    content = file.read()
    # Remove all whitespace characters for easier parsing
    content = content.replace('\n', '').replace('\r', '') \
                     .replace('\t', '').replace(' ', '')
    file.close()
    # Parse content and return tree
    (T, _) = __load(content, typelt)
    return T


###############################################################################
# Display

def dot(B):
    """Write down simple dot format of tree.

    Args:
        B (TreeAsBin).

    Returns:
        str: String storing dot format of tree.

    """

    s = "graph {\n"
    s += "node [shape=circle, fixedsize=true, height=0.5, width=0.5]\n"
    q = Queue()
    q.enqueue(B)
    s += str(id(B)) + '[label = "' + str(B.key) + '"]\n'
    while not q.isempty():
        B = q.dequeue()
        child = B.child
        while child:
            s += str(id(child)) + '[label = "' + str(child.key) + '"]\n'
            s = s + "   " + str(id(B)) + " -- " + str(id(child)) + "\n"
            q.enqueue(child)
            child = child.sibling
    s += "}"
    return s


def display(B):
    try:
        from IPython.display import display
        from graphviz import Source
    except:
        raise Exception("Missing module: graphviz and/or IPython.")
    display(Source(dot(B)))
    
