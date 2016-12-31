# coding=utf-8
from types import *
import collections
import functools

#空函数
def do_nothing():
    pass

assert type(do_nothing) == FunctionType

#加法
def add(x, y = 100):
    '''x + y'''
    return x + y

assert type(do_nothing) == type(add)
assert add(1, 2) == 3
assert add(1) == 101

def join_triple(x, y, z):
    return "{0}-{1}-{2}".format(x, y, z)
a1 = functools.partial(join_triple, 100)
a2 = functools.partial(join_triple, y=100)
assert a1(1, 2) == '100-1-2'       #add(100, 1, 2)
assert a2(1, z=2) == '1-100-2'     #add(1, y=100, z=2)

#作用域
a = 4
b = 5
def foo():
    b = 0
    return a

def bar():
    global b
    b = 0
    return c + b

assert foo() == 4 and b == 5
a= 5
assert foo() == 5

c = 100
assert bar() == 100 and b == 0

#返回值为元组类型
def exchange(x, y):
    return (y, x)

assert exchange(1, 2) == (2, 1)
assert exchange(y=1, x=2) == (1, 2)


#不定参数
def foo(*args):
    l = []
    for i in args:
        l.append(str(i))
    return len(args), '-'.join(l)

i, j = foo(1, 2, 3)
assert i == 3 and j == '1-2-3'

i, j = foo([1, 2, 3])
assert i == 1 and j == '[1, 2, 3]'

i, j = foo(*[1, 2, 3])
assert i == 3 and j == '1-2-3'

#关键字变量参数
def foo(a, *args, **argskey):
    print 'a', 1
    print args
    print argskey

foo(1, 2, 3, a1='a', a2='b')


#内置函数sorted
Persion = collections.namedtuple('Persion', 'name, age')
#help(Persion)
print Persion.__dict__
assert 'name' in Persion.__dict__['_fields']

p = Persion('zte', 30)
assert p.name == 'zte'
assert p.age == 30

p_list = [Persion('a', 100), Persion('d', 50), Persion('c', 30), Persion('b', 10)]
l1 = sorted(p_list)
print l1
assert l1[3].name == 'd' and l1[3].age == 50
l2 = sorted(p_list, cmp=lambda l, r : cmp(l.age, r.age))
print l2
assert l2[3].name == 'a' and l2[3].age == 100
l3 = sorted(p_list, key=lambda p:p.age)
assert l2 == l3


#内置函数filter, map, reduce
m = map(lambda x, y: (x, y), ['a', 'b', 'c'], ['1', '2', '3', '4', '5'])
print m

r = reduce(lambda x, y: x + y, range(11), 0)
assert r == 155
#a + b + c + d
#(((base + a) + b) + c) + b

assert filter(lambda x : x % 2 == 0, range(11)) == [0, 2, 4, 6, 8, 10]


#实现生成器
def iter_func(start, end, step = 1):
    print 'iter_func'
    while start < end:
        yield start
        start += step

iter_f = iter_func(0, 10, 3)
assert type(iter_f) == GeneratorType
assert list(iter_f) == range(0, 10, 3)


#另外一种生成器
l = (i + 1 for i in range(0, 10) if i % 3 == 0)
assert type(l) is types.GeneratorType
assert list(l) == [1, 4, 7, 10]