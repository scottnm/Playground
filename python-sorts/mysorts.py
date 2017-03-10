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

def selection(__seq : List[T], *, \
              out_of_order : Callable[[T, T], bool] = operator.gt) -> List[T]:
    seq = __seq[:]
    for unsorted_mark in range(0, len(__seq) - 1):
        min_index = unsorted_mark
        for i in range(unsorted_mark + 1, len(__seq)):
            if out_of_order(seq[min_index], seq[i]):
                min_index = i
        seq[unsorted_mark], seq[min_index] = seq[min_index], seq[unsorted_mark]
    return seq

def insertion(__seq : List[T], *, \
              out_of_order : Callable[[T, T], bool] = operator.gt) -> List[T]:
    seq = __seq[:]
    return seq

