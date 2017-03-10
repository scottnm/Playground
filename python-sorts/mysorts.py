#!/usr/bin/env python3

import operator
from typing import Any, Callable, List, TypeVar
T = TypeVar('T')

def bubble(__seq : List[T], *, \
           out_of_order : Callable[[T, T], bool] = operator.gt) -> List[T]:
    seq = __seq[:]
    for unsorted_range in range(len(__seq), 0, -1):
        already_sorted = True
        for i in range(0, unsorted_range - 1):
            if out_of_order(seq[i], seq[i + 1]):
                already_sorted = False
                seq[i], seq[i + 1] = seq[i + 1], seq[i]
        if already_sorted:
            break
    return seq

    
