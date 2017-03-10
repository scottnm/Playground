#!/usr/bin/env python3


from sorts import *
import unittest

class TestSorts(unittest.TestCase):
    # sorts go here
    sorts = [ \
            ]

    def test_simple_presorted(self):
        input = [i for i in range(1, 5)]
        preserved_input = input[:]
        expected_output = input[:]
        assert input is not expected_output
        for sort_func in TestSorts.sorts:
            with self.subTest():
                self.assertEquals(expected_output, sort_func(input))
                self.assertEquals(input, preserved_input) # assert no mutation

    def test_simple_reversed(self):
        input = [i for i in range(4, 0, -1)]
        preserved_input = input[:]
        expected_output = [i for i in range(1, 5)]
        for sort_func in TestSorts.sorts:
            with self.subTest():
                self.assertEquals(expected_output, sort_func(input))
                self.assertEquals(input, preserved_input) # assert no mutation

    def test_simple_random_arrangement(self):
        input = [0, 4, 7, 3, 2, 8, 5, 1, 9, 6]
        preserved_input = input[:]
        expected_output = [i for i in range(0, 10)]
        for sort_func in TestSorts.sorts:
            with self.subTest():
                self.assertEquals(expected_output, sort_func(input))
                self.assertEquals(input, preserved_input) # assert no mutation


if __name__ == "__main__":
    unittest.main()
