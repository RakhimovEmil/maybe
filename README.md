# maybe<T>
`std::optional` implementation with shorter functionality

This class is a wrapper over a value of type T (`maybe<T>`). At each point in time, `maybe` either stores a value or an indication of its absence. The difference from a smart pointer is that `maybe` does not allocate dynamic memory for an object, because the memory of the required size is already inside `maybe<T>` (if you create an empty `maybe<T> (default ctor)`, no object of type `T` will be created at this point)

```
struct test {
    string field = "some_string";
};

control_03::maybe<test> mb1(test{});
const control_03::maybe<test> mb2(mb1);
control_03::maybe<test> mb3;

assert(mb1);
assert(!mb3);

mb1.value().field = "other_string";

assert(mb2.value().field == "some_string");
assert(mb1.value().field == "other_string");
```

The problem with wrapper classes is that you often have to create temporary objects that have to be copied:

```
maybe<MyClass> mb(MyClass{arg1, arg2, ...});
```

That's why the `emplace` method is implemented internally, which initializes the internal value with directly passed arguments (no temporary objects are created)

```
maybe<my_class> mb;

// one object is created in mb (it's new value)
mb.emplace(arg1, arg2, ...);

// another object is created in mb
// this new value replaces the old one
mb.emplace(arg0);
```