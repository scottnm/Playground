#!/usr/bin/env python3


from mysorts import *
from typing import Callable, List
import unittest
import operator

class TestSorts(unittest.TestCase):
    # sorts go here
    sorts = [bubble, selection, insertion, quick_inplace, quick]

    def sort_test(self, input, expected_output, order_gt=operator.gt):
        preserved_input = input[:]
        assert input is not expected_output
        for sort_func in TestSorts.sorts:
            with self.subTest(msg=sort_func.__name__):
                self.assertEqual(expected_output, sort_func(input, order_gt=order_gt))
                self.assertEqual(input, preserved_input) # assert no mutation

    def test_1_element(self):
        input = [1]
        expected_output = input[:]
        self.sort_test(input, expected_output)

    def test_presorted(self):
        input = [i for i in range(1, 5)]
        expected_output = input[:]
        self.sort_test(input, expected_output)

    def test_reversed(self):
        input = [i for i in range(4, 0, -1)]
        expected_output = [i for i in range(1, 5)]
        self.sort_test(input, expected_output)

    def test_random_arrangement(self):
        input = [0, 4, 7, 3, 2, 8, 5, 1, 9, 6]
        expected_output = [i for i in range(0, 10)]
        self.sort_test(input, expected_output)

    def test_reverse_sort_presorted(self):
        input = [i for i in range(4, 0, -1)]
        expected_output = input[:]
        self.sort_test(input, expected_output, operator.le)
        pass

    def test_reverse_sort_reversed(self):
        input = [i for i in range(1, 5)]
        expected_output = [i for i in range(4, 0, -1)]
        self.sort_test(input, expected_output, operator.le)
        pass


if __name__ == "__main__":
    unittest.main()
