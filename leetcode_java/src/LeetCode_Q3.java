import java.util.*;

public class LeetCode_Q3 {
    class IntRange {
        public IntRange(int start, int end) {
            this.start = start;
            this.end = end;
        }

        public int start;
        public int end;

        @Override
        public String toString() {
            return "[" + start + ", " + end + ")";
        }
    }

    class Value {
        public Value(int index, char character)
        {
            this.index = index;
            this.character = character;
        }

        public int index;
        public char character;
    }

    class LinkedNode<T> {
        public LinkedNode(T value) {
            this(null, value);
        }

        public LinkedNode(LinkedNode<T> next, T value) {
            this.next = next;
            this.value = value;
        }

        public LinkedNode<T> next;
        public T value;
    }

    class Box<T> {
        public Box(T value) {
            this.value = value;
        }

        public T value;

        public T get() {
            return value;
        }

        public void set(T value) {
            this.value = value;
        }
    }

    class LinkedList<T> {
        public LinkedList() {
            headBox = new Box<LinkedNode<T>>(null);
        }

        public Box<LinkedNode<T>> headBox;

        public void addFirst(T value) {
            headBox.set(new LinkedNode<>(headBox.get(), value));
        }
    }

    public int lengthOfLongestSubstring(String s) {
        int i = 0, j = 0, max = 0;
        Set<Character> set = new HashSet<>();

        while (j < s.length()) {
            if (!set.contains(s.charAt(j))) {
                set.add(s.charAt(j++));
                max = Math.max(max, set.size());
            } else {
                set.remove(s.charAt(i++));
            }
        }

        return max;

//        ArrayList<IntRange> ranges = new ArrayList<>();
//        LinkedList<Value> values = new LinkedList<>();
//        Map<Character, LinkedNode<Value>> currentlySeenCharacters = new HashMap<>();
//        int start = 0;
//        for (int i = 0; i < s.length(); i++) {
//            char c = s.charAt(i);
//            if (currentlySeenCharacters.containsKey(c)) {
//                if (start < i) {
//                    ranges.add(new IntRange(start, i));
//                }
//                Iterator<LinkedNode<Value>> currentValueNodes = currentlySeenCharacters.values().iterator();
//                if (currentValueNodes.hasNext()) {
//                    LinkedNode<Value> node = currentValueNodes.next();
//                    start = node.value.index + 1;
//                    currentlySeenCharacters.put(c, i);
//                }
//                else {
//                    throw new RuntimeException("should not get here");
//                }
//            }
//            else {
//                currentlySeenCharacters.put(c, i);
//            }
//        }
//        if (start < s.length()) {
//            ranges.add(new IntRange(start, s.length()));
//        }
//
//        int maxSubstrLengthNoRepeat = 0;
//        for (int i = 0; i < ranges.size(); i++) {
//            IntRange range = ranges.get(i);
//            maxSubstrLengthNoRepeat = Math.max(maxSubstrLengthNoRepeat, range.end - range.start);
//        }
//        maxSubstrLengthNoRepeat = Math.max(maxSubstrLengthNoRepeat, s.length() - start);
//        return maxSubstrLengthNoRepeat;
    }
}
