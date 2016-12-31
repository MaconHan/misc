#! /usr/bin/python
# coding=utf-8
import types
from types import * 

assert None == None
t = None
assert t is None   #id(t) == id(None)

#整形
i = 1
j = int('1')
assert type(i) is types.IntType
assert i == j

k = int('0xFF', 16)
assert k == 255

b1 = 10000
b2 = int('10000')
assert i is j     #id(i) == id(j)
assert b1 is not b2
assert b1 == b2

#浮点
f1 = 1.123
f2 = float('1.123')
f3 = f1

assert type(f1) is FloatType
assert f1 == f3
assert f1 is f3
assert f1 == f2
assert f1 is not f2

f4 = 4.2E-2
assert f4 == 0.042


#字符串
s = 'welcome to zte'
assert type(s) is StringType
assert len(s) == 14
assert s.count('e') == 3
assert s[0] == 'w'
assert s[-1] == 'e'
assert s[:3] == 'wel'
assert s[11:] == 'zte'

s2 = '"'
assert s2 == chr(34)
s3 = "'"
assert s3 == chr(39)
s3 = '\n'
assert len(s3) == 1
s4 = r'\n'
assert len(s4) == 2
s5 = '''welcome to zte
welcome to china'''

s1 = '%d-%s' % (123, 'abc')
s2 = '{0}-{1}'.format(123, 'abc')
assert s1 == s2

s3 = '{c.real}-{c.imag}'.format(c=complex(1, 2))
assert s3 == '1.0-2.0'

#list
l = [1]
assert type(l) == ListType
l.append(2)
l.append(3)    #l == [1, 2, 3]
assert len(l) == 3
l.pop(0)
l.pop()
assert len(l) == 1
assert l[0] == 2

l = [1, 2, 3]
assert [3, 2, 1] == l[::-1]

#tuple, readonly
t = (1,)
assert type(t) == TupleType
assert len(t) == 1
assert t[0] == 1


#dict
d1 = {}
assert type(d1) == DictType
d2 = {'a':1, 'b':2}     #__hash__()
d3 = dict(zip(('a', 'b'), (1, 2)))
assert d2 == d3
assert 'a' in d2  #d2.has_key('a')
assert 'c' not in d2

d2['c'] = 3
for k, v in d2.iteritems():
    print k, v

d2.pop('a')
assert sorted(d2.keys()) == ['b', 'c']
assert sorted(d2.values()) == [2, 3]

d2.update({'z':1})
assert sorted(d2.keys()) == ['b', 'c', 'z']

#集合
s1 = set('abcdefg')  #可变性
assert 'g' in s1
s2 = frozenset('abcdefg')   #不可变性
assert 'g' in s2

assert s1 == s2

s1.add('0')
assert '0' in s1


#if语句，for语句，while语句
if 1 < 0 or 100 > 101:
    assert 1 == 0
elif 1 < 2 > 3:
    assert 1 == 0
else:
    assert 1 == 1


l = []
for i in range(0, 10, 2):   #区分xrange
    l.append(i)
assert len(l) == 5 and l == [0, 2, 4, 6, 8]

b = False
for _ in range(10):
    pass
else:
    b = True
assert b

b = False
for _ in range(10):
    break
else:
    b = True
assert not b


for i, value in enumerate(range(10, 20)):
    print i, value


l = [i + 1 for i in range(0, 10) if i % 3 == 0]
assert type(l) is ListType
assert l == [1, 4, 7, 10]

