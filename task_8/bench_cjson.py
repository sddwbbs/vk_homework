import cjson, json, ujson, time

big = '{' + ', '.join(f'"k{i}": {i}' for i in range(10000)) + '}'

N = 100

def bench(fn, s):
    t0 = time.time()
    for _ in range(N):
        fn(s)
    return time.time() - t0

print("json:", bench(json.loads, big))
print("ujson:", bench(ujson.loads, big))
print("cjson:", bench(cjson.loads, big)) 