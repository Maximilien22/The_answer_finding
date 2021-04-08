# -*- coding: utf-8 -*-
"""General Tree module.


*Warning:* All the functions defined in this module are assumed to receive a non-None
value for their ``ref/T`` parameter.

"""

from . import queue
from .queue import Queue


class Tree:
    """Simple class for general tree.

    Attributes:
        key (Any): Node key.
        children (List[Tree]): Node children.

    """
    def __init__(self, key=None, children=None):
        """Init general tree, ensure children are properly set.

        Args:
            key (Any).
            children (List[Tree]).

        """

        self.key = key

        if children == None:
            self.children = []
        else:
            self.children = children

    @property
    def nbchildren(self):
        """Number of children of node."""

        return len(self.children)


###############################################################################
# load and save
# use the "list" representation: (o A1 A2 ... AN).

def to_list(T):
    s = '(' + str(T.key)
    for child in T.children:
        s += to_list(child)
    s += ')'
    return s

def save(T, filename):
    fout = open(filename, mode='w')
    fout.write(to_list(T))
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

def dot(T):
    """Write simple down dot format of tree.

    Args:
        T (Tree).

    Returns:
        str: String storing dot format of tree.

    """
    s = "graph {\n"
    s += "node [shape=circle, fixedsize=true, height=0.5, width=0.5]\n"
    q = Queue()
    q.enqueue(ref)
    s += str(id(ref)) + '[label = "' + str(ref.key) + '"]\n'
    while not q.isempty():
        ref = q.dequeue()
        for child in ref.children:
            s += str(id(child)) + '[label = "' + str(child.key) + '"]\n'
            s = s + "   " + str(id(ref)) + " -- " + str(id(child)) + "\n"
            q.enqueue(child)
    s += "}"
    return s

def display(T):
    """Render a Tree for in-browser display.

    *Warning:* Made for use within IPython/Jupyter only.

    Args:
        T (Tree).

    Returns:
        Source: Graphviz wrapper object for tree rendering.

    """

    # Ensure all modules are available
    try:
        from graphviz import Source
        from IPython.display import display
    except:
        raise Exception("Missing module: graphviz.")
    # Generate dot and return display object
    dot_source = dot(T)
    display(Source(dot_source))
