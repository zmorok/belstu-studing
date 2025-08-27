using System;
using System.Collections.Generic;
using System.Linq;

namespace CommonSet
{
    public partial class CommonSet<T> : ISet<T> where T : IComparable<T>
    {
        private readonly HashSet<T> elements;
        public HashSet<T> Elements
        {
            get => elements;
            set { }
        }

        public CommonSet()
        {
            elements = new HashSet<T>();
        }

        public CommonSet(IEnumerable<T> initialElements)
        {
            elements = new HashSet<T>(initialElements);
        }

        

        public override string ToString()
        {
            return string.Join(", ", elements.OrderBy(e => e));
        }

        public override bool Equals(object obj)
        {
            return obj is CommonSet<T> set &&
                   EqualityComparer<HashSet<T>>.Default.Equals(elements, set.elements) &&
                   EqualityComparer<HashSet<T>>.Default.Equals(Elements, set.Elements) &&
                   Count == set.Count;
        }

        public override int GetHashCode()
        {
            int hashCode = 793449964;
            hashCode = hashCode * -1521134295 + EqualityComparer<HashSet<T>>.Default.GetHashCode(elements);
            hashCode = hashCode * -1521134295 + EqualityComparer<HashSet<T>>.Default.GetHashCode(Elements);
            hashCode = hashCode * -1521134295 + Count.GetHashCode();
            return hashCode;
        }

        public T this[int index]
        {
            get => elements.ElementAt(index);
            set
            {
                if (index >= 0 && index < elements.Count)
                {
                    elements.Remove(elements.ElementAt(index));
                    elements.Add(value);
                }
            }
        }
    }
}
