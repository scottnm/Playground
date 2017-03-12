
#!/usr/bin/env python3

import operator
from typing import Any, Callable, List, TypeVar
T = TypeVar('T')

def bubble(__seq : List[T], *, \
           order_gt : Callable[[T, T], bool] = operator.gt) -> List[T]:
    seq = __seq[:]
    for unsorted_range in range(len(__seq), 0, -1):
        already_sorted = True
        for i in range(0, unsorted_range - 1):
            if order_gt(seq[i], seq[i + 1]):
                already_sorted = False
                seq[i], seq[i + 1] = seq[i + 1], seq[i]
        if already_sorted:
            break
    return seq

def selection(__seq : List[T], *, \
              order_gt : Callable[[T, T], bool] = operator.gt) -> List[T]:
    seq = __seq[:]
    for unsorted_mark in range(0, len(__seq) - 1):
        min_index = unsorted_mark
        for i in range(unsorted_mark + 1, len(__seq)):
            if order_gt(seq[min_index], seq[i]):
                min_index = i
        seq[unsorted_mark], seq[min_index] = seq[min_index], seq[unsorted_mark]
    return seq

def insertion(__seq : List[T], *, \
              order_gt : Callable[[T, T], bool] = operator.gt) -> List[T]:
    seq = __seq[:]
    for unsorted_mark in range(1, len(__seq)):
        ele = seq[unsorted_mark]
        insert_pos = 0
        for i in range(unsorted_mark - 1, -1, -1):
            if order_gt(seq[i], ele):
                seq[i + 1] = seq[i]
            else:
                insert_pos = i + 1
                break
        seq[insert_pos] = ele
    return seq

def quick_1(__seq : List[T], *, \
          order_gt : Callable[[T, T], bool] = operator.gt) -> List[T]:

    seq = __seq[:]
    sublist_stack = [(0, len(__seq))] 
    while len(sublist_stack) > 0:
        sublist = sublist_stack.pop()

        # check that the sublist isn't already sorted
        if (sublist[1] - sublist[0]) < 2:
            continue

        # select the middle as the pivot
        pivot = (sublist[1] + sublist[0]) // 2
        pivot_ele = seq[pivot]

        # count the number of elements to find the final pivot position
        ele_le = 0 # keep track of elements less than or equal to the pivot
        ele_gt = 0 # keep track of elements greater than the pivot

        for i in range(sublist[0], pivot):
            if order_gt(seq[i], pivot_ele):
                ele_gt += 1
            else:
                ele_le += 1
        for i in range(pivot+1, sublist[1]):
            if order_gt(seq[i], pivot_ele):
                ele_gt += 1
            else:
                ele_le += 1

        # swap pivot to final position
        left_0 = sublist[0]
        pivot_final = left_0 + ele_le
        right_0 = pivot_final + 1
        seq[pivot], seq[pivot_final] = seq[pivot_final], seq[pivot]

        # swap elements in sublist until everything on left is le the pivot
        # and everything on the right is gt the pivot
        if ele_le > ele_gt:
            gt_swaps = ele_gt # at most gt_swap out of place gt elements
            le_runner = 0
            gt_runner = 0
            while gt_swaps > 0 and le_runner < ele_le:
                if order_gt(seq[left_0 + le_runner], pivot_ele):
                    while True:
                        if not order_gt(seq[right_0 + gt_runner], pivot_ele):
                            break
                        else:
                            gt_runner += 1
                    assert gt_runner < ele_gt
                    lpos = left_0 + le_runner
                    rpos = right_0 + gt_runner
                    seq[lpos], seq[rpos] = seq[rpos], seq[lpos]
                    gt_swaps -= 1 
                le_runner += 1
        else:
            le_swaps = ele_le # at most gt_swap out of place gt elements
            le_runner = 0
            gt_runner = 0
            while le_swaps > 0 and gt_runner < ele_gt:
                if not order_gt(seq[right_0 + gt_runner], pivot_ele):
                    while True:
                        if order_gt(seq[left_0 + le_runner], pivot_ele):
                            break
                        else:
                            le_runner += 1
                    assert le_runner < ele_le
                    lpos = left_0 + le_runner
                    rpos = right_0 + gt_runner
                    seq[lpos], seq[rpos] = seq[rpos], seq[lpos]
                    le_swaps -= 1 
                gt_runner += 1

        sublist_stack.append((left_0, pivot_final))
        sublist_stack.append((pivot_final+1, sublist[1]))
    return seq

def quick_2(__seq : List[T], *, \
          order_gt : Callable[[T, T], bool] = operator.gt) -> List[T]:
    if len(__seq) < 2:
        return __seq

    pivot = len(__seq) // 2
    pivot_ele = __seq[pivot]
    le_sublist = []
    gt_sublist = []

    for i in range(0, pivot):
        ele = __seq[i]
        if order_gt(ele, pivot_ele):
            gt_sublist.append(ele)
        else:
            le_sublist.append(ele)
    for i in range(pivot+1, len(__seq)):
        ele = __seq[i]
        if order_gt(ele, pivot_ele):
            gt_sublist.append(ele)
        else:
            le_sublist.append(ele)

    return quick_2(le_sublist, order_gt=order_gt) + [pivot_ele] + \
            quick_2(gt_sublist, order_gt=order_gt)
