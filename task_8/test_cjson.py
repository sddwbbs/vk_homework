import cjson
import json
import ujson

def test_loads():
    s = '{"hello": 10, "world": "value"}'
    assert cjson.loads(s) == json.loads(s) == ujson.loads(s)
    s2 = '{"a": "b", "b": 123, "c": "456"}'
    assert cjson.loads(s2) == json.loads(s2) == ujson.loads(s2)

def test_dumps():
    s = '{"hello": 10, "world": "value"}'
    d = cjson.loads(s)
    assert cjson.dumps(d) == s
    s2 = '{"a": "b", "b": 123, "c": "456"}'
    d2 = cjson.loads(s2)
    assert cjson.dumps(d2) == s2

def main():
    test_loads()
    test_dumps()
    print("All tests passed.")

if __name__ == "__main__":
    main() 