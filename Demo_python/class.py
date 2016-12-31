# coding=utf-8
from types import *

# 父类
class A:
    '''this is A Class'''
    X = 1

    def __init__(self, x, y):
        self.x = x
        self.y = y
        self._z= x + y

    def type(self):
        '''this is a function'''
        return 'A'

assert type(A) is ClassType

a = A(100, 200)
assert a.x == 100 and a.y == 200 and a.X == 1 and A.X == 1
assert a._z == 300

a.X = 0
assert a.X == 0 and A.X == 1

#子类
class B(A):
    def __init__(self):
        A.__init__(self, 0, 0)
        pass
    
    def type(self):
        return 'B'

b = B()
assert issubclass(B, A)
assert isinstance(b, B) and isinstance(b, A)
assert b.x == 0 and b.y == 0 and b.X == 1
assert b.type() == 'B'

#静态方法 类方法 类属性
class C:
    _instance = None

    def __init__(self):
        self.x = 10
        self.y = 20

    @staticmethod
    def p1():
        pass

    @classmethod
    def instance(cls):
        if C._instance is None:
            C._instance = cls()
        return C._instance

c1 = C.instance()
c2 = C.instance()
assert c1 is c2
assert type(C.p1) is FunctionType
assert type(c1.__init__) is MethodType

#定义__new__,使用于继承不可变类型
class AbsInt(int):
    def __new__(cls, val):
        return super(AbsInt, cls).__new__(cls, abs(val))

    def __init__(self, val):
        super(AbsInt, self).__init__(self, 100)

i = AbsInt(-1)
assert i == 1

#使用__new__,实现单例
class D(object):
    def __new__(cls):
        if not hasattr(cls, '_instance'):
            cls._instance = super(D, cls).__new__(cls);
        return cls._instance

d1 = D()
d2 = D()
assert d1 is d2

#修饰器
class E(object):
    def __init__(self, x, y):
        self._a= 0
        self.x = x
        self.y = y

    @property
    def z(self):
        return self.x + self.y

    def _get_a(self):
        return self._a
    def _set_a(self, val):
        self._a = val
    a = property(_get_a, _set_a)

    def __getitem__(self, name):
        if name == 'a':
            return self.a
        raise ValueError()

e = E(10, 100)
assert e.z == 110
assert e.a == 0
e.a = -1
assert e.a == -1
assert e['a'] == -1

#自定义修饰器
import time
def perf(func):
    def perf_wrapper(start, end):
        print '[%s] start %s' % (func.__name__, time.ctime())
        func(start, end)
        print '[%s] start %s' % (func.__name__, time.ctime())
    return perf_wrapper

@perf
def add_counter(start, end):
    time.sleep(5)
    while(start < end):
        start += 1

add_counter(1, 1000 * 1000)   #perf(add_counter)(1, 1000 * 1000)



#内建方法
class F:
    def __str__(self):
        return 'this is {0}'.format(type(self))
    def __repr__(self):
        return '<this is {0}>'.format(type(self))
f = F()
f
print f 
print '%r' % f

l = [1, 2, 3, 4]
r = repr(l)
assert eval(r) == l


#上下文
import time
class G:
    counter = 0
    def __init__(self):
        G.counter += 1
    def __del__(self):
        G.counter -= 1
    def __enter__(self):
        self.start = time.time()
    def __exit__(self, exc_type, exc_val, exc_tb):
        self.end = time.time()
        print 'elapsed time is %d seconds' % (self.end - self.start)
        print exc_type, exc_val, exc_tb

with G() as g:
    assert g.counter == 1
    assert G.counter == 1
    time.sleep(1)
assert G.counter == 0

with G() as g:
    time.sleep(1)
    1 / 0
assert G.counter == 0


#上下文,函数的写法
import contextlib

@contextlib.contextmanager
def run_context():
    print 'enter'
    yield 'hello'
    print 'exit'

with run_context() as f:
    assert f == 'hello'