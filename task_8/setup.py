from setuptools import setup, Extension

setup(
    name="cjson",
    version="0.1",
    ext_modules=[Extension("cjson", ["cjson.c"])],
) 