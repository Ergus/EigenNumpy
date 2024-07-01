#!/usr/bin/env python

import unittest
import EigenNumpy as enp
import numpy as np

class TestStringMethods(unittest.TestCase):

    def test_sizes(self):
        v = enp.RowMatrix(8, 12)
        self.assertEqual(v.rows(), 8)
        self.assertEqual(v.cols(), 12)
        self.assertEqual(v.size(), 8 * 12)

    def test_zero(self):
        v = enp.RowMatrix.zero(8, 12)
        self.assertEqual(v.rows(), 8)
        self.assertEqual(v.cols(), 12)
        self.assertEqual(v.sum(), 0.0)

    def test_bracket_one(self):
        v = enp.RowMatrix.zero(4, 4)
        self.assertEqual(v[0, 0], 0.0)
        self.assertEqual(v[1, 1], 0.0)
        self.assertEqual(v[2, 2], 0.0)
        self.assertEqual(v[3, 3], 0.0)
        # Test setitem
        v[0, 0] = 1
        v[1, 1] = 2
        v[2, 2], v[3, 3] = 3, 4
        # Test getitem
        self.assertEqual(v[0, 0], 1.0)
        self.assertEqual(v[1, 1], 2.0)
        self.assertEqual(v[2, 2], 3.0)
        self.assertEqual(v[3, 3], 4.0)

    def test_random(self):
        v = enp.RowMatrix.random(8, 8)
        self.assertEqual(v.rows(), v.cols())
        self.assertEqual(v.cols(), 8)

    def test_compare(self):
        n1 = enp.RowMatrix(np.arange(0, 16).reshape(4, 4).astype(np.double))
        n2 = enp.RowMatrix(np.arange(0, 16).reshape(4, 4).astype(np.double))
        n3 = enp.RowMatrix.random(4, 4)
        self.assertEqual(n1, n2)
        self.assertNotEqual(n1, n3)

    def test_bracket_range(self):
        n = np.arange(0, 16).reshape(4, 4).astype(np.double)
        v = enp.RowMatrix(n)
        self.assertTrue((v[0, :] == n[0, :]).all())
        self.assertTrue((v[1, 1:3] == n[1, 1:3]).all())

    def test_fromNumpy(self):
        n = np.random.rand(3, 4)
        v = enp.RowMatrix(n)
        # Test the constructor from numpy worked
        self.assertTrue((n == v).all())
        self.assertTrue((v == n).all())
        # Test by index
        self.assertEqual(v[1, 2], n[1, 2])
        self.assertEqual(v[2, 1], n[2, 1])

    def test_toNumpy(self):
        v = enp.RowMatrix.random(8, 8)
        n = np.array(v)
        # Test the numpy copy worked
        self.assertTrue((n == v).all())
        self.assertTrue((v == n).all())
        # Test by index
        self.assertEqual(v[1, 2], n[1, 2])
        self.assertEqual(v[2, 1], n[2, 1])

    def test_multMxM(self):
        v1 = enp.RowMatrix.random(8, 8)
        v2 = enp.RowMatrix.random(8, 8)
        # Copy to compare
        n1 = np.array(v1)
        n2 = np.array(v2)
        self.assertTrue((n1 == v1).all())
        self.assertTrue((n2 == v2).all())

        mulV = v1 * v2
        mulN = n1.dot(n2)
        self.assertTrue((mulV == mulN).all())

    def test_multMxV(self):
        v = enp.RowMatrix.random(8, 8)
        n = np.array(v)
        self.assertTrue((n * 3 == 3 * v).all())
        self.assertTrue((3 * n == 3 * v).all())
        self.assertEqual(3 * v, v * 3)


if __name__ == '__main__':
    unittest.main()
