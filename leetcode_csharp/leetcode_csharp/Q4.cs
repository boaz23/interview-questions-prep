using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace leetcode_csharp
{
    internal class Q4
    {
        public static void Main(string[] args)
        {
            Console.WriteLine(new Solution().FindMedianSortedArrays(new int[] { 1, }, new int[] { }));
            Console.WriteLine(new Solution().FindMedianSortedArrays(new int[] { 1, 3, }, new int[] { 2, }));
            Console.WriteLine(new Solution().FindMedianSortedArrays(new int[] { 1, 2, }, new int[] { 3, 4, }));
            Console.WriteLine(new Solution().FindMedianSortedArrays(new int[] { 1, 4, }, new int[] { 8, 10, }));
            Console.WriteLine(new Solution().FindMedianSortedArrays(new int[] { 1, }, new int[] { 2, 3, 4, 5, }));
        }

        public class Solution
        {
            public double FindMedianSortedArrays(int[] nums1, int[] nums2)
            {
                int[] a1, a2;
                if (nums1.Length < nums2.Length)
                {
                    a1 = nums2;
                    a2 = nums1;
                }
                else
                {
                    a1 = nums1;
                    a2 = nums2;
                }

                int totalLength = a1.Length + a2.Length;
                if (totalLength == 0)
                {
                    throw new ArgumentException("Empty arrays");
                }

                int start = 0, end = a1.Length;
                int half = totalLength / 2;
                int a1_left = 0, a1_right = 0;
                int a2_left = 0, a2_right = 0;
                while (true)
                {
                    int i1 = start + ((end - start) / 2);
                    int i2 = half - i1;

                    a1_left = ValueAt(a1, i1 - 1);
                    a2_left = ValueAt(a2, i2 - 1);
                    a1_right = ValueAt(a1, i1);
                    a2_right = ValueAt(a2, i2);
                    
                    if (a1_right >= a2_left)
                    {
                        if (a2_right >= a1_left)
                        {
                            break;
                        }
                        else
                        {
                            end = i1;
                        }
                    }
                    else
                    {
                        if (a1_right < a2_left)
                        {
                            start = i1 + 1;
                        }
                        else
                        {
                            throw new Exception("Impossible case: Both strictly smaller than");
                        }
                    }
                }

                int m1 = Math.Min(a1_right, a2_right);
                int m2 = Math.Max(a1_left, a2_left);
                return totalLength % 2 == 0 ? (double)(m1 + m2) / 2 : m1;
            }

            private static int ValueAt(int[] a, int i)
            {
                if (i < 0)
                {
                    return int.MinValue;
                }
                if (i >= a.Length)
                {
                    return int.MaxValue;
                }
                return a[i];
            }
        }
    }
}
