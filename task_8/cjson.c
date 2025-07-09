#define PY_SSIZE_T_CLEAN

#include <Python.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

static void skip_ws(const char **p) {
    while (**p && isspace(**p)) (*p)++;
}

static PyObject* cjson_loads(PyObject* self, PyObject* args) {
    const char* json_str;
    if (!PyArg_ParseTuple(args, "s", &json_str))
        return NULL;

    PyObject* dict = PyDict_New();
    if (!dict) return NULL;

    const char* p = json_str;
    skip_ws(&p);
    if (*p != '{') {
        PyErr_SetString(PyExc_ValueError, "JSON must start with '{'");
        Py_DECREF(dict);
        return NULL;
    }
    p++;
    skip_ws(&p);
    while (*p && *p != '}') {
        skip_ws(&p);
        if (*p != '"') {
            PyErr_SetString(PyExc_ValueError, "Key must start with \"");
            Py_DECREF(dict);
            return NULL;
        }
        p++;
        const char* key_start = p;
        while (*p && *p != '"') p++;
        if (!*p) {
            PyErr_SetString(PyExc_ValueError, "Unterminated key string");
            Py_DECREF(dict);
            return NULL;
        }
        PyObject* key = PyUnicode_FromStringAndSize(key_start, p - key_start);
        p++;
        skip_ws(&p);
        if (*p != ':') {
            PyErr_SetString(PyExc_ValueError, "Expected ':' after key");
            Py_DECREF(key); Py_DECREF(dict);
            return NULL;
        }
        p++;
        skip_ws(&p);
        PyObject* value = NULL;
        if (*p == '"') {
            p++;
            const char* val_start = p;
            while (*p && *p != '"') p++;
            if (!*p) {
                PyErr_SetString(PyExc_ValueError, "Unterminated value string");
                Py_DECREF(key); Py_DECREF(dict);
                return NULL;
            }
            value = PyUnicode_FromStringAndSize(val_start, p - val_start);
            p++;
        } else if (isdigit(*p) || *p == '-') {
            char* endptr;
            long num = strtol(p, &endptr, 10);
            value = PyLong_FromLong(num);
            p = endptr;
        } else {
            PyErr_SetString(PyExc_ValueError, "Value must be string or number");
            Py_DECREF(key); Py_DECREF(dict);
            return NULL;
        }
        if (PyDict_SetItem(dict, key, value) < 0) {
            Py_DECREF(key); Py_DECREF(value); Py_DECREF(dict);
            return NULL;
        }
        Py_DECREF(key); Py_DECREF(value);
        skip_ws(&p);
        if (*p == ',') {
            p++;
            skip_ws(&p);
        } else if (*p == '}') {
            break;
        } else {
            PyErr_SetString(PyExc_ValueError, "Expected ',' or '}'");
            Py_DECREF(dict);
            return NULL;
        }
    }
    if (*p != '}') {
        PyErr_SetString(PyExc_ValueError, "JSON must end with '}'");
        Py_DECREF(dict);
        return NULL;
    }
    return dict;
}

static PyObject* cjson_dumps(PyObject* self, PyObject* args) {
    PyObject* dict;
    if (!PyArg_ParseTuple(args, "O!", &PyDict_Type, &dict))
        return NULL;

    PyObject* items = PyDict_Items(dict);
    if (!items) return NULL;
    Py_ssize_t n = PyList_Size(items);
    PyObject* result = PyUnicode_FromString("{");
    for (Py_ssize_t i = 0; i < n; ++i) {
        PyObject* pair = PyList_GetItem(items, i);
        PyObject* key = PyTuple_GetItem(pair, 0);
        PyObject* value = PyTuple_GetItem(pair, 1);
        PyObject* key_str = PyObject_Str(key);
        PyObject* value_str = NULL;
        if (PyUnicode_Check(value)) {
            value_str = PyUnicode_FromFormat("\"%U\"", value);
        } else if (PyLong_Check(value)) {
            value_str = PyObject_Str(value);
        } else {
            PyErr_SetString(PyExc_TypeError, "Only str and int values supported");
            Py_DECREF(key_str); Py_DECREF(items); Py_DECREF(result);
            return NULL;
        }
        PyUnicode_AppendAndDel(&result, PyUnicode_FromFormat("%s\"%U\": %U", i ? ", " : "", key, value_str));
        Py_DECREF(key_str); Py_DECREF(value_str);
    }
    Py_DECREF(items);
    PyUnicode_AppendAndDel(&result, PyUnicode_FromString("}"));
    return result;
}

static PyMethodDef CjsonMethods[] = {
    {"loads", cjson_loads, METH_VARARGS, "Parse JSON string to dict."},
    {"dumps", cjson_dumps, METH_VARARGS, "Serialize dict to JSON string."},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef cjsonmodule = {
    PyModuleDef_HEAD_INIT, "cjson", NULL, -1, CjsonMethods
};

PyMODINIT_FUNC PyInit_cjson(void) {
    return PyModule_Create(&cjsonmodule);
} 