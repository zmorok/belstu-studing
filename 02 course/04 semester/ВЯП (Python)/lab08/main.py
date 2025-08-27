import time
import sys
from pathlib import Path

def uppercase(func):
    def wrapper(*args, **kwargs):
        result = func(*args, **kwargs)
        return result.upper()
    return wrapper


def count_calls(func):
    def wrapper(*args, **kwargs):
        wrapper.call_count += 1
        return func(*args, **kwargs)

    wrapper.call_count = 0
    return wrapper

def html_tag(tag):
    def decorator(func):
        def wrapper():
            return f"<{tag}>{func()}</{tag}>"
        return wrapper
    return decorator


def timer(func):
    def wrapper(*args, **kwargs):
        start = time.perf_counter()
        result = func(*args, **kwargs)
        elapsed_ms = (time.perf_counter() - start) * 1_000
        print(f"[timer] {func.__name__} заняла {elapsed_ms:.2f} мс")
        return result
    return wrapper


def log(path = "function_calls.log"):
    def decorator(func):
        def wrapper(*args, **kwargs):
            result = func(*args, **kwargs)

            with Path(path).open("w", encoding="utf-8") as f:
                timestamp = time.strftime("%Y-%m-%d %H:%M:%S")
                f.write(f"{timestamp} | {func.__name__}"
                        f"({', '.join(map(repr, args))}"
                        f"{', ' if kwargs else ''}"
                        f"{', '.join(f'{k}={v}' for k,v in kwargs.items())})"
                        f" -> {result}\n")
            return result
        return wrapper
    return decorator


def memory_usage(func):
    def wrapper(*args, **kwargs):
        size_in  = sum(sys.getsizeof(obj) for obj in args) + sum(sys.getsizeof(v)   for v in kwargs.values())
        result   = func(*args, **kwargs)
        size_out = sys.getsizeof(result)
        size   = (size_in + size_out) / 1024**2

        print(f"[sys.getsizeof()] Оценка (args + result): {size:.4f} MiB")
        return result
    return wrapper

@uppercase
def farewell(name):
    return f"До свидания, {name}!"

@count_calls
def greet(name):
    print(f"Приветствую, {name}!")

@html_tag("div")
@html_tag("p")
def get_div():
    return "Hello, World!"

@timer
def some_function(n):
    return sum(i * i for i in range(n))

@log()
@memory_usage
def heavy(n, m):
    return [i ** 2 for i in range(n, m)]


if __name__ == "__main__":
    print(farewell("Анна"))
    greet("Боб")
    greet("Генадий")
    print(f"Функция greet вызвана {greet.call_count} раз(а).")
    print(get_div())

    some_function(10**6)
    heavy(n=1_000, m=10_000)
