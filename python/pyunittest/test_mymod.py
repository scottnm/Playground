import unittest
import mymod

class TestMyModMethods(unittest.TestCase):

    def setUp(self):
        pass

    def test_f(self):
        self.assertEqual(mymod.some_func(), 3)

    def test_of(self):
        self.assertEqual(mymod.some_other_func(), 7)

if __name__ == '__main__':
    unittest.main()
