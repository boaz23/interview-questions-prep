using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace leetcode_csharp
{
    internal class Q10
    {
        public class Solution
        {
            class AutomataState
            {
                public AutomataState(int index_string, int index_pattern)
                {
                    this.index_string = index_string;
                    this.index_pattern = index_pattern;
                    this.keepGoing = true;
                }

                public int index_string;
                public int index_pattern;
                public bool keepGoing;
            }

            public bool IsMatch(string s, string p)
            {
                var states = new List<AutomataState>() {
                    new AutomataState(0, 0),
                };

                int statesCount;
                while ((statesCount = states.Count) > 0)
                {
                    for (int i = 0; i < statesCount; i++)
                    {
                        AutomataState state = states[i];
                        int i_p = state.index_pattern;
                        int i_s = state.index_string;

                        while (i_p < p.Length)
                        {
                            char p_char = p[i_p];
                            if (i_p < p.Length - 1 && p[i_p + 1] == '*')
                            {
                                i_p += 2;
                                states.Add(new AutomataState(i_s, i_p));
                                while (i_s < s.Length && (p_char == '.' || p_char == s[i_s]))
                                {
                                    i_s++;
                                    states.Add(new AutomataState(i_s, i_p));
                                }
                            }
                            else
                            {
                                if (i_s == s.Length || !(p_char == '.' || p_char == s[i_s]))
                                {
                                    state.keepGoing = false;
                                    break;
                                }
                                else
                                {
                                    i_p++;
                                    i_s++;
                                }
                            }
                        }

                        state.index_pattern = i_p;
                        state.index_string = i_s;
                    }

                    var newStates = new List<AutomataState>();
                    foreach (AutomataState state in states)
                    {
                        int i_p = state.index_pattern;
                        int i_s = state.index_string;

                        if (i_p < p.Length)
                        {
                            if (state.keepGoing)
                            {
                                newStates.Add(state);
                            }
                        }
                        else if (i_s == s.Length)
                        {
                            return true;
                        }
                    }
                    states = newStates;
                }

                return false;
            }
        }
    }
}
