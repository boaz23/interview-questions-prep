public class LeetCode_Q5 {
    public static class Solution1 {
        class IntRange {
            public IntRange(int start, int end) {
                this.start = start;
                this.end = end;
            }

            public int start;
            public int end;

            public int length() {
                return end - start + 1;
            }

            @Override
            public String toString() {
                return "[" + start + ", " + end + "]";
            }
        }

        public String longestPalindrome(String s) {
            int n = s.length();
            IntRange[][] dp = new IntRange[n][];

            IntRange emptyRange = new IntRange(0 ,-1);
            IntRange maxRange = emptyRange;
            for (int diff = 0; diff < n; diff++){
                for (int start = 0, end = start + diff; end < n; start++, end++) {
                    IntRange range = null;
                    IntRange currentRange = new IntRange(start, end);
                    if (start == end) {
                        range = new IntRange(start, end);
                    }
                    else if (s.charAt(start) == s.charAt(end)) {
                        if (diff == 1) {
                            range = currentRange;
                        }
                        else  {
                            IntRange innerRange = dp[start + 1][end - 1];
                            if (innerRange.start == start + 1 && innerRange.end == end - 1) {
                                range = currentRange;
                            }
                        }
                    }
                    if (range == null) {
                        range = emptyRange;
                    }
                    if (maxRange.length() < range.length()) {
                        maxRange = range;
                    }

                    if (dp[start] == null) {
                        dp[start] = new IntRange[n];
                    }
                    dp[start][end] = range;
                }
            }

            return s.substring(maxRange.start, maxRange.end + 1);
        }

        public String longestPalindrome_recursion(String s) {
            IntRange[][] mem = new IntRange[s.length()][];
            IntRange range = longestPalindrome_recursiveAux(s, 0, s.length() - 1, mem);
            assert range != null;
            return s.substring(range.start, range.end + 1);
        }

        private IntRange longestPalindrome_recursiveAux(String s, int start, int end, IntRange[][] mem) {
            if (start < 0 || end >= s.length() || start > end) {
                return null;
            } else if (mem[start] == null) {
                mem[start] = new IntRange[s.length()];
            } else if (mem[start][end] != null) {
                return mem[start][end];
            }

            IntRange maxRange;
            if (start == end) {
                maxRange = new IntRange(start, end);
            } else {
                IntRange range;
                maxRange = new IntRange(0, -1);
                char cStart = s.charAt(start);
                char cEnd = s.charAt(end);
                if (cStart == cEnd) {
                    range = longestPalindrome_recursiveAux(s, start + 1, end - 1, mem);
                    if (range == null || (maxRange.length() < range.length() && start + 1 == range.start && end - 1 == range.end)) {
                        maxRange = new IntRange(start, end);
                    }
                }

                String p;

                range = longestPalindrome_recursiveAux(s, start + 1, end, mem);
                if (range != null && maxRange.length() < range.length()) {
                    maxRange = range;
                }

                range = longestPalindrome_recursiveAux(s, start, end - 1, mem);
                if (range != null && maxRange.length() < range.length()) {
                    maxRange = range;
                }
            }

            mem[start][end] = maxRange;
            return maxRange;
        }
    }
}
