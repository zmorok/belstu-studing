using System;
using System.Collections.Generic;
using System.Linq;

namespace CommonSet
{
    public partial class CommonSet<T> : ISet<T> where T : IComparable<T>
    {
        #region ISet realization

        public void Add(T item)
        {
            try
            {
                if (item == null)
                    throw new ArgumentNullException(nameof(item), "Элемент не может быть null.");

                if (!elements.Add(item))
                    throw new InvalidOperationException($"Элемент {item} уже существует в коллекции.");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Ошибка при добавлении {item} (строку ниже игнорировать): {ex.Message}");
            }
            finally
            {
                Console.WriteLine($"\tдобавлено - {item}");
            }
        }

        public void Remove(T item)
        {
            try
            {
                if (item == null)
                    throw new ArgumentNullException(nameof(item), "Элемент не может быть null.");

                if (!elements.Remove(item))
                    throw new InvalidOperationException($"Элемент {item} не найден в коллекции.");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Ошибка при удалении элемента {item}: {ex.Message}");
            }
            finally
            {
                Console.WriteLine("Удаление значения завершено");
            }
        }
        public string Contains(T item) => ItemContains(item) ? $"Элемент {item} содержится." : $"{item} НЕ содержится.";

        private bool ItemContains(T item)
        {
            try
            {
                return elements.Contains(item);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Ошибка при проверке {item}: {ex.Message}");
                return false;
            }
            finally
            {
            }
        }

        

        public IEnumerable<T> Find(Func<T, bool> predicate)
        {
            try
            {
                return elements.Where(predicate).ToList();
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Ошибка при поиске элемента: {ex.Message}");
                return default;
            }
            finally
            {
                Console.WriteLine("Операция поиска завершена.");
            }
        }

        public int Count => elements.Count;

        public IEnumerable<T> GetAll() => elements;

        #endregion
    }
}

