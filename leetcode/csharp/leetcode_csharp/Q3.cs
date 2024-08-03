namespace leetcode_csharp
{
    internal class Q3
    {
        public class Solution
        {
            public int LengthOfLongestSubstring(string s)
            {
                var currentNoRepeatCharacters = new Dictionary<char, int>();
                int start = 0, end = 0;
                int maxLen = 0;
                while (end < s.Length)
                {
                    char c = s[end];
                    if (currentNoRepeatCharacters.ContainsKey(c))
                    {
                        maxLen = Math.Max(maxLen, end - start);
                        int newStart = currentNoRepeatCharacters[c] + 1;
                        while (start < newStart)
                        {
                            _ = currentNoRepeatCharacters.Remove(s[start]);
                            start++;
                        }
                    }

                    currentNoRepeatCharacters.Add(c, end);
                    end++;
                }

                maxLen = Math.Max(maxLen, end - start);
                return maxLen;
            }
        }
    }
}
